#ifndef DISK_HPP
#define DISK_HPP

#include <vector>

#include <io/data/data_provider.hpp>

class Partition;

/// Disk context
class Disk
{
public:
  Disk(DiskStream*,uint32_t);
  ~Disk();

  DataProvider* getDataProvider();
  const std::vector<Partition*>& getPartitions() const;
  Partition* addPartition(uint64_t start, uint64_t length);
  uint32_t getSectorSize() const;

private:
  DataProvider* dataProvider;
  std::vector<Partition*> partitions;
  uint32_t sectorSize;
};

#endif /* DISK_HPP */
