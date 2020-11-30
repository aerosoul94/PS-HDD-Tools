#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <io/data/data_provider.hpp>

class Disk;

/// Partition context
class Partition
{
  DataProvider* dataProvider; // Should be a BoundedDataProvider
  uint64_t start, end, length;

public:
  Partition(Disk* disk, uint64_t start, uint64_t length);
  ~Partition();

  DataProvider* getDataProvider();
  uint64_t getLength() const;
  uint64_t getStart() const;
  uint64_t getEnd() const;
};

#endif /* PARTITION_HPP */
