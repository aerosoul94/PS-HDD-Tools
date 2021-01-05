#ifndef DISK_HPP
#define DISK_HPP

#include <vector>

#include <io/data/data_provider.hpp>

namespace disk {

class Partition;

/// Disk context
class Disk
{
public:
  Disk(io::stream::DiskStream* stream, uint32_t sectorSize);
  ~Disk();

  io::data::DataProvider* getDataProvider();
  const std::vector<Partition*>& getPartitions() const;
  Partition* addPartition(uint64_t start, uint64_t length);
  uint32_t getSectorSize() const;

private:
  io::data::DataProvider* dataProvider;
  std::vector<Partition*> partitions;
  uint32_t sectorSize;
};

} /* namespace disk */

#endif /* DISK_HPP */
