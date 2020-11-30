#include "partition.hpp"
#include "disk.hpp"

#include <io/data/bounded_data_provider.hpp>

Partition::Partition(Disk* disk, uint64_t start, uint64_t length)
{
  this->dataProvider = new BoundedDataProvider(
    disk->getDataProvider(), start, length, disk->getSectorSize());
  this->start = start;
  this->end = start + length;
  this->length = length;
}

Partition::~Partition()
{
  delete this->dataProvider;
}

DataProvider* Partition::getDataProvider()
{
  return this->dataProvider;
}

uint64_t Partition::getLength() const
{
  return this->length;
}

uint64_t Partition::getStart() const
{
  return this->start;
}

uint64_t Partition::getEnd() const
{
  return this->end;
}
