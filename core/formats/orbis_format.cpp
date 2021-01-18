#include "orbis_format.hpp"

#include <iostream>
#include <iomanip>

#include <crypto/aes_xts_strategy.hpp>
#include <io/data/data_provider.hpp>
#include <disk/disk.hpp>
#include <disk/partition.hpp>
#include <vfs/adapters/ufs2_adapter.hpp>

namespace formats {

bool OrbisDiskFormat::match(disk::Disk* disk, disk::DiskConfig* config)
{
  auto keyData = config->getKeys();
  auto dataProvider = disk->getDataProvider();

  if (keyData.size() < 0x20)
    return false;

  dataProvider->seek(0x200);
  dataProvider->read(reinterpret_cast<char*>(&hdr), sizeof(hdr));

  if (!verifyGptHeader())
    return false;

  return !memcmp(hdr.hdr_sig, GPT_HDR_SIG, sizeof(hdr.hdr_sig));
}

bool OrbisDiskFormat::verifyGptHeader()
{
  if (hdr.entsz > sizeof(gpt_ent))
    return false;

  if (hdr.entries > 0x80)
    return false;

  if (hdr.lba_table != 0x2)
    return false;

  return true;
}

void OrbisDiskFormat::build(disk::Disk* disk, disk::DiskConfig* config)
{
  auto dataProvider = disk->getDataProvider();
  this->loadGptEntTable(dataProvider);

  auto keys = config->getKeys();

  auto ent = getGptEntByType(&user_ent);
  auto partition = disk->addPartition(
    ent->lba_start * kSectorSize, 
    (ent->lba_end - ent->lba_start) * kSectorSize
  );

  auto partitionDataProvider = partition->getDataProvider();
  partitionDataProvider->setSectorBias(ent->lba_start);
  partitionDataProvider->setCryptoStrategy(
    new crypto::AesXtsStrategy(keys.data(), keys.data() + 0x10)
  );

  partition->getVfs()->setAdapter(
    new vfs::adapters::Ufs2Adapter(partition->getDataProvider())
  );

  // Some partitions use different keys
  // for (auto ent : partitionTable) {
  //   auto partition = new Partition(disk);
  //   auto partitionDataProvider = partition->getDataProvider();
  //   partitionDataProvider->setCryptoStrategy(
  //     new AesXtsStrategy(keys.data(), keys.data() + 0x10)
  //   );
  //   partitions.push_back(partition);
  // }
}

void OrbisDiskFormat::loadGptEntTable(io::data::DataProvider* dataProvider)
{
  const uint64_t offset = hdr.lba_table * kSectorSize;
  dataProvider->seek(offset);
  partitions.resize(hdr.entries);
  dataProvider->read(reinterpret_cast<char*>(partitions.data()), 
    partitions.size() * hdr.entsz);
}

const gpt_ent* OrbisDiskFormat::getGptEntByType(const uuid* type)
{
  for (const auto& ent : partitions)
    if (!memcmp(&ent.type, type, sizeof(uuid)))
      return &ent;
  return nullptr;
}

} /* namespace formats */
