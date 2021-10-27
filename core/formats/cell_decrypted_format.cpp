#include "cell_decrypted_format.hpp"

#include "partition/ps3.h"

#include <disk/partition.hpp>
#include <io/data/data_provider.hpp>
#include <utilities/endian.hpp>
#include <vfs/adapters/fat_adapter.hpp>
#include <vfs/adapters/ufs2_adapter.hpp>

#include <vector>

namespace formats {

bool CellDecryptedDiskFormat::match(disk::Disk* disk, disk::DiskConfig* config)
{
    auto dataProvider = disk->getDataProvider();

    if (checkDiskLabel(dataProvider))
        return true;

    return false;
}

bool CellDecryptedDiskFormat::checkDiskLabel(io::data::DataProvider* dataProvider)
{
    std::vector<char> buf(kSectorSize);

    dataProvider->seek(0);
    dataProvider->read(buf.data(), kSectorSize);

    disklabel* hdr = reinterpret_cast<disklabel*>(buf.data());

    if (swap64(hdr->d_magic1) == MAGIC1 && swap64(hdr->d_magic2) == MAGIC2)
        return true;

    return false;
}

void CellDecryptedDiskFormat::build(disk::Disk* disk, disk::DiskConfig* config)
{
    auto dataProvider = disk->getDataProvider();

    std::vector<char> buf(0x2000);
    dataProvider->seek(0);
    dataProvider->read(buf.data(), 0x2000);

    auto table = reinterpret_cast<d_partition*>(buf.data() + sizeof(disklabel));

    d_partition* vflash = nullptr,
               * hdd0 = nullptr,
               * hdd1 = nullptr;

    auto vflashHeader = reinterpret_cast<disklabel*>(buf.data() + 0x1000);

    if (swap64(vflashHeader->d_magic1) == MAGIC1 && swap64(vflashHeader->d_magic2) == MAGIC2) {
        vflash = &table[0];
        hdd0 = &table[1];
        hdd1 = &table[2];
    } else {
        hdd0 = &table[0];
        hdd1 = &table[1];
    }

    if (vflash) {
        auto base = swap64(vflash->p_start);

        d_partition* vflashTable = reinterpret_cast<d_partition*>(buf.data() + 0x1000 + sizeof(disklabel));

        addFatPartition(disk, "dev_flash1", base + swap64(vflashTable[1].p_start), swap64(vflashTable[1].p_size));
        addFatPartition(disk, "dev_flash2", base + swap64(vflashTable[2].p_start), swap64(vflashTable[2].p_size));
        addFatPartition(disk, "dev_flash3", base + swap64(vflashTable[3].p_start), swap64(vflashTable[3].p_size));
    }

    addUfsPartition(disk, "dev_hdd0", swap64(hdd0->p_start), swap64(hdd0->p_size));
    addFatPartition(disk, "dev_hdd1", swap64(hdd1->p_start), swap64(hdd1->p_size));
}

void CellDecryptedDiskFormat::addFatPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length)
{
    auto partition = disk->addPartition(start * kSectorSize, length * kSectorSize);

    partition->setName(name);
    partition->getVfs()->setAdapter(new vfs::adapters::FatAdapter(partition->getDataProvider()));
}

void CellDecryptedDiskFormat::addUfsPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length)
{
    auto partition = disk->addPartition(start * kSectorSize, length * kSectorSize);

    partition->setName(name);
    partition->getVfs()->setAdapter(new vfs::adapters::Ufs2Adapter(partition->getDataProvider(), true));
}

} /* namespace formats */
