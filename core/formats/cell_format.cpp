#include "cell_format.hpp"


#include <crypto/aes/aes.h>
#include <crypto/aes_cbc_strategy.hpp>
#include <crypto/aes_xts_strategy.hpp>
#include <crypto/aes_cbc_swapped_strategy.hpp>
#include <crypto/aes_xts_swapped_strategy.hpp>
#include <crypto/multi_layer_strategy.hpp>
#include <disk/partition.hpp>
#include <io/data/data_provider.hpp>
#include <vfs/adapters/fat_adapter.hpp>
#include <vfs/adapters/ufs2_adapter.hpp>

#include <iostream>
#include <iomanip>

namespace formats {

static uint8_t encdec_seed_00[] =
{
  0xE2, 0xD0, 0x5D, 0x40, 0x71, 0x94, 0x5B, 0x01, 0xC3, 0x6D, 0x51, 0x51, 0xE8, 0x8C, 0xB8, 0x33,
  0x4A, 0xAA, 0x29, 0x80, 0x81, 0xD8, 0xC4, 0x4F, 0x18, 0x5D, 0xC6, 0x60, 0xED, 0x57, 0x56, 0x86
};

static uint8_t encdec_seed_20[] =
{
  0x02, 0x08, 0x32, 0x92, 0xC3, 0x05, 0xD5, 0x38, 0xBC, 0x50, 0xE6, 0x99, 0x71, 0x0C, 0x0A, 0x3E,
  0x55, 0xF5, 0x1C, 0xBA, 0xA5, 0x35, 0xA3, 0x80, 0x30, 0xB6, 0x7F, 0x79, 0xC9, 0x05, 0xBD, 0xA3
};

static uint8_t sb_indiv_seed_00[] =
{
  0xD9, 0x2D, 0x65, 0xDB, 0x05, 0x7D, 0x49, 0xE1, 0xA6, 0x6F, 0x22, 0x74, 0xB8, 0xBA, 0xC5, 0x08,
  0x83, 0x84, 0x4E, 0xD7, 0x56, 0xCA, 0x79, 0x51, 0x63, 0x62, 0xEA, 0x8A, 0xDA, 0xC6, 0x03, 0x26
};

static uint8_t sb_indiv_seed_20[] =
{
  0xC3, 0xB3, 0xB5, 0xAA, 0xCC, 0x74, 0xCD, 0x6A, 0x48, 0xEF, 0xAB, 0xF4, 0x4D, 0xCD, 0xF1, 0x6E,
  0x37, 0x9F, 0x55, 0xF5, 0x77, 0x7D, 0x09, 0xFB, 0xEE, 0xDE, 0x07, 0x05, 0x8E, 0x94, 0xBE, 0x08
};

void CellDiskFormat::generateKeys(std::vector<char>& eidRootKey)
{
  auto generateKey = [](char* eid_key, char* eid_iv, uint8_t* seed, char* out) {
    aes_context aes_ctxt;
    uint8_t iv[0x10];

    aes_setkey_enc(&aes_ctxt, (uint8_t*)eid_key, 0x100);
    memcpy(iv, eid_iv, 0x10);
    aes_crypt_cbc(&aes_ctxt, AES_ENCRYPT, 0x20, iv, seed, (uint8_t*)out);
  };

  ataKeys.resize(0x40);

  generateKey(eidRootKey.data(), eidRootKey.data() + 0x20, sb_indiv_seed_00, ataKeys.data());
  generateKey(eidRootKey.data(), eidRootKey.data() + 0x20, sb_indiv_seed_20, ataKeys.data() + 0x20);

  encDecKeys.resize(0x40);

  generateKey(eidRootKey.data(), eidRootKey.data() + 0x20, encdec_seed_00, encDecKeys.data());
  generateKey(eidRootKey.data(), eidRootKey.data() + 0x20, encdec_seed_20, encDecKeys.data() + 0x20);
}

void CellDiskFormat::addVflashPartitions(disk::Disk* disk, io::data::DataProvider* dataProvider, d_partition* vflash)
{
    // vflash has multiple partitions of its own..
    // [0] = ?
    // [1] = dev_flash  FAT16
    // [2] = dev_flash2 FAT16
    // [3] = dev_flash3 FAT12
    // [4] = dev_flash4

    // Save the current crypto strategy.
    auto diskStrategy = dataProvider->getCryptoStrategy();

    // Vflash uses a multiple layers of crypto..
    auto strategy = new crypto::MultiLayerStrategy();
    strategy->addLayer(diskStrategy);
    strategy->addLayer(new crypto::AesXtsStrategy(encDecKeys.data(), encDecKeys.data() + 0x20));

    // TODO: Should probably pass this to disk->addPartition instead of inheriting from the disk.
    dataProvider->setCryptoStrategy(strategy);

    // Read in the VFLASH partition table
    std::vector<char> tmp(0x1000);
    dataProvider->seek(swap64(vflash->p_start) * kSectorSize);
    dataProvider->read(tmp.data(), 0x1000);

    auto base = swap64(vflash->p_start);

    d_partition* vflashTable = reinterpret_cast<d_partition*>(tmp.data()
        + sizeof(disklabel));

    addFatPartition(disk, "dev_flash1", base + swap64(vflashTable[1].p_start), swap64(vflashTable[1].p_size));
    addFatPartition(disk, "dev_flash2", base + swap64(vflashTable[2].p_start), swap64(vflashTable[2].p_size));
    addFatPartition(disk, "dev_flash3", base + swap64(vflashTable[3].p_start), swap64(vflashTable[3].p_size));

    // Set the crypto strategyback to the original.
    dataProvider->setCryptoStrategy(diskStrategy);
}

void CellDiskFormat::addFatPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length)
{
  auto partition = disk->addPartition(start * kSectorSize, length * kSectorSize);

  partition->setName(name);
  partition->getVfs()->setAdapter(new vfs::adapters::FatAdapter(partition->getDataProvider()));
}

void CellDiskFormat::addUfsPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length)
{
  auto partition = disk->addPartition(start * kSectorSize, length * kSectorSize);

  partition->setName(name);
  partition->getVfs()->setAdapter(new vfs::adapters::Ufs2Adapter(partition->getDataProvider(), true));
}

} /* namespace formats */
