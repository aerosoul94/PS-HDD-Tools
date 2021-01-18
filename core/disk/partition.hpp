#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <io/data/data_provider.hpp>
#include <vfs/file_system.hpp>

namespace disk {

class Disk;

/// Partition context
class Partition
{
  io::data::DataProvider* dataProvider; // Should be a BoundedDataProvider
  // NOTE: I suppose it's possible to have more than one file system
  vfs::Vfs* vfs; 
  uint64_t start, end, length;

public:
  Partition(Disk* disk, uint64_t start, uint64_t length);
  ~Partition();

  void mount();
  vfs::Vfs* getVfs();
  io::data::DataProvider* getDataProvider();
  uint64_t getLength() const;
  uint64_t getStart() const;
  uint64_t getEnd() const;
};

} /* namespace disk */

#endif /* PARTITION_HPP */
