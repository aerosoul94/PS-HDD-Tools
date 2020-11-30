#include "disk.hpp"
#include "partition.hpp"

Disk::Disk(DiskStream* stream, uint32_t sectorSize)
{
  this->dataProvider = new DataProvider(stream, sectorSize);
  this->sectorSize = sectorSize;
}

Disk::~Disk()
{
  for (auto iter = partitions.begin(); iter != partitions.end(); ++iter)
    delete *iter;

  partitions.clear();
}

DataProvider* Disk::getDataProvider()
{
  return this->dataProvider;
}

const std::vector<Partition*>& Disk::getPartitions() const
{
  return this->partitions;
}

Partition* Disk::addPartition(uint64_t start, uint64_t length)
{
  this->partitions.push_back(new Partition(this, start, length));
  return this->partitions.back();
}

uint32_t Disk::getSectorSize() const
{
  return this->sectorSize;
}
