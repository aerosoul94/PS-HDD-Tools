#include "cell_slim_format.hpp"

#include <crypto/aes_xts_swapped_strategy.hpp>
#include <utilities/endian.hpp>

namespace formats {

bool CellSlimDiskFormat::match(disk::Disk* disk, disk::DiskConfig* config)
{
    auto keyData = config->getKeys();
    auto dataProvider = disk->getDataProvider();

    if (keyData.size() < 0x30)
        return false;

    generateKeys(keyData);

    if (checkDiskLabel(dataProvider))
        return true;

    return false;
}

bool CellSlimDiskFormat::checkDiskLabel(io::data::DataProvider* dataProvider)
{
    std::vector<char> buf(kSectorSize);

    dataProvider->setCryptoStrategy(new crypto::AesXtsSwappedStrategy(ataKeys.data(), ataKeys.data() + 0x20));

    dataProvider->seek(0);
    dataProvider->read(buf.data(), kSectorSize);

    disklabel* hdr = reinterpret_cast<disklabel*>(buf.data());

    if (swap64(hdr->d_magic1) == MAGIC1 && swap64(hdr->d_magic2) == MAGIC2)
        return true;

    return false;
}

void CellSlimDiskFormat::build(disk::Disk* disk, disk::DiskConfig* config)
{
    auto dataProvider = disk->getDataProvider();

    std::vector<char> buf(0x2000);
    dataProvider->seek(0);
    dataProvider->read(buf.data(), 0x2000);

    auto table = reinterpret_cast<d_partition*>(buf.data() + sizeof(disklabel));
    d_partition* vflash = &table[0],
               * hdd0 = &table[1],
               * hdd1 = &table[2];

    addVflashPartitions(disk, dataProvider, vflash);

    addUfsPartition(disk, "dev_hdd0", swap64(hdd0->p_start), swap64(hdd0->p_size));
    addFatPartition(disk, "dev_hdd1", swap64(hdd1->p_start), swap64(hdd1->p_size));
}

} /* namespace formats */
