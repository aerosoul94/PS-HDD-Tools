#include "cell_format.hpp"

#include <crypto/aes/aes.h>
#include <crypto/crypto_strategy.hpp>
#include <crypto/aes_cbc_strategy.hpp>
#include <crypto/aes_xts_strategy.hpp>
#include <crypto/aes_cbc_swapped_strategy.hpp>
#include <crypto/aes_xts_swapped_strategy.hpp>
#include <crypto/multi_layer_strategy.hpp>
#include <utilities/endian.hpp>
#include <disk/partition.hpp>
#include <vfs/adapters/fat_adapter.hpp>
#include <vfs/adapters/ufs2_adapter.hpp>

#include <iostream>
#include <iomanip>

namespace formats {

static uint8_t encdec_seed_00[] =
{
  0xE2, 0xD0, 0x5D, 0x40, 0x71, 0x94, 0x5B, 0x01, 
  0xC3, 0x6D, 0x51, 0x51, 0xE8, 0x8C, 0xB8, 0x33,
  0x4A, 0xAA, 0x29, 0x80, 0x81, 0xD8, 0xC4, 0x4F, 
  0x18, 0x5D, 0xC6, 0x60, 0xED, 0x57, 0x56, 0x86
};

static uint8_t encdec_seed_20[] =
{
  0x02, 0x08, 0x32, 0x92, 0xC3, 0x05, 0xD5, 0x38, 
  0xBC, 0x50, 0xE6, 0x99, 0x71, 0x0C, 0x0A, 0x3E,
  0x55, 0xF5, 0x1C, 0xBA, 0xA5, 0x35, 0xA3, 0x80, 
  0x30, 0xB6, 0x7F, 0x79, 0xC9, 0x05, 0xBD, 0xA3
};

static uint8_t sb_indiv_seed_00[] =
{
  0xD9, 0x2D, 0x65, 0xDB, 0x05, 0x7D, 0x49, 0xE1, 
  0xA6, 0x6F, 0x22, 0x74, 0xB8, 0xBA, 0xC5, 0x08,
  0x83, 0x84, 0x4E, 0xD7, 0x56, 0xCA, 0x79, 0x51, 
  0x63, 0x62, 0xEA, 0x8A, 0xDA, 0xC6, 0x03, 0x26
};

static uint8_t sb_indiv_seed_20[] =
{
  0xC3, 0xB3, 0xB5, 0xAA, 0xCC, 0x74, 0xCD, 0x6A, 
  0x48, 0xEF, 0xAB, 0xF4, 0x4D, 0xCD, 0xF1, 0x6E,
  0x37, 0x9F, 0x55, 0xF5, 0x77, 0x7D, 0x09, 0xFB, 
  0xEE, 0xDE, 0x07, 0x05, 0x8E, 0x94, 0xBE, 0x08
};

bool CellDiskFormat::match(disk::Disk* disk, disk::DiskConfig* config)
{
  auto keyData = config->getKeys();
  auto dataProvider = disk->getDataProvider();

  // Make sure we have the correct amount of key data
  if (keyData.size() < 0x30)
    return false;

  // Generate encdec keys
  generateKeys(keyData);

  std::vector<char> buf(kSectorSize);

  this->type = Ps3Type::PHAT;
  dataProvider->seek(0);
  dataProvider->setCryptoStrategy(
    new crypto::AesCbcSwappedStrategy(ataKeys)
  );
  dataProvider->read(buf.data(), kSectorSize);
  disklabel* hdr = reinterpret_cast<disklabel*>(buf.data());
  if (swap64(hdr->d_magic1) == MAGIC1 && swap64(hdr->d_magic2) == MAGIC2)
    return true;

  this->type = Ps3Type::SLIM;
  dataProvider->seek(0);
  dataProvider->setCryptoStrategy(
    new crypto::AesXtsSwappedStrategy(ataKeys.data(), ataKeys.data() + 0x20)
  );
  dataProvider->read(buf.data(), kSectorSize);
  if (swap64(hdr->d_magic1) == MAGIC1 && swap64(hdr->d_magic2) == MAGIC2)
    return true;

  return false;
}

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

  generateKey(
    eidRootKey.data(),
    eidRootKey.data() + 0x20,
    sb_indiv_seed_00,
    ataKeys.data()
  );

  generateKey(
    eidRootKey.data(),
    eidRootKey.data() + 0x20,
    sb_indiv_seed_20,
    ataKeys.data() + 0x20
  );

  encDecKeys.resize(0x40);

  generateKey(
    eidRootKey.data(), 
    eidRootKey.data() + 0x20, 
    encdec_seed_00, 
    encDecKeys.data()
  );

  generateKey(
    eidRootKey.data(), 
    eidRootKey.data() + 0x20, 
    encdec_seed_20, 
    encDecKeys.data() + 0x20
  );
}

void CellDiskFormat::build(disk::Disk* disk, disk::DiskConfig* config)
{
  auto dataProvider = disk->getDataProvider();
  dataProvider->seek(0);
  std::vector<char> buf(0x1000);
  dataProvider->read(buf.data(), 0x1000);

  auto table = reinterpret_cast<d_partition*>(buf.data() + sizeof(disklabel));
  d_partition* vflash = nullptr,
    * hdd0 = nullptr,
    * hdd1 = nullptr;
  switch (type) {
    case Ps3Type::SLIM:
      vflash = &table[0];
      hdd0 = &table[1];
      hdd1 = &table[2];
      break;
    case Ps3Type::PHAT:
      // TODO: I'm not sure if this is entirely reliable.
      //   This is how we're detecting the VFLASH partition.
      //   Alternatively, we could probably look into the p_acl fields.
      if (swap64(table[0].p_start) == 8) {
        vflash = &table[0];
        hdd0 = &table[1];
        hdd1 = &table[2];
      } else {
        hdd0 = &table[0];
        hdd1 = &table[1];
      }
      break;
    case Ps3Type::UNK: // Should not be possible.
    default:
      return;
  }
  
  if (vflash)
  {
    // vflash has multiple partitions of its own..
    // [0] = ?
    // [1] = dev_flash  FAT16
    // [2] = dev_flash2 FAT16
    // [3] = dev_flash3 FAT12
    // [4] = dev_flash4 

    // auto vflashView = disk->addPartition(
    //   swap64(vflash->p_start) * kSectorSize,
    //   swap64(vflash->p_size) * kSectorSize
    // );

    // vflashView->setName("dev_flash");

    // auto strategy = new crypto::MultiLayerStrategy();
    // strategy->addLayer(dataProvider->getCryptoStrategy());

    // TODO: They supposedly have different usage of algorithms depending on
    //  the type of PS3 it is.
    // crypto::CryptoStrategy* secondLayer;
    // if (this->type == Ps3Type::PHAT)
    //   secondLayer = new crypto::AesXtsStrategy(encDecKeys.data(), 
    //     encDecKeys.data() + 0x20);
    // else
    //   secondLayer = new crypto::AesXtsStrategy(encDecKeys.data(), 
    //     encDecKeys.data() + 0x20);

    // TODO: Refactor crypto strategy's to always use MultiLayerStrategy.
    //  Separate "swapping" code from crypto code, to a new CryptoStrategy.
    //  Name it SwapShortsStrategy or something.
    // strategy->addLayer(
    //   new crypto::AesXtsStrategy(encDecKeys.data(), encDecKeys.data() + 0x20)
    // );
    // vflashView->getDataProvider()->setCryptoStrategy(strategy);

    // Could just copy the CryptoStrategy from vflash
    // auto partition = disk->addPartition(
    //   swap64(vflashTable[0].p_start) * kSectorSize,
    //   swap64(vflashTable[0].p_size) * kSectorSize
    // );
    // partition->setName("dev_flash");
    // partition->getDataProvider()->setCryptoStrategy(
    //   vflashView->getDataProvider()->getCryptoStrategy()
    // );
  }

  // Add dev_hdd0
  {
    // No need to set crypto method as it inherits from the base
    auto partition = disk->addPartition(
      swap64(hdd0->p_start) * kSectorSize, 
      swap64(hdd0->p_size) * kSectorSize
    );
    
    partition->setName("dev_hdd0");
    partition->getVfs()->setAdapter(
      new vfs::adapters::Ufs2Adapter(partition->getDataProvider(), true)
    );
  }

  // Add dev_hdd1
  {
    auto partition = disk->addPartition(
      swap64(hdd1->p_start) * kSectorSize,
      swap64(hdd1->p_size) * kSectorSize
    );

    partition->setName("dev_hdd1");
    partition->getVfs()->setAdapter(
      new vfs::adapters::FatAdapter(partition->getDataProvider())
    );
  }
}

} /* namespace formats */
