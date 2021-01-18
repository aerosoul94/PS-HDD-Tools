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

  /**
   * @brief Mount the file system that exists in this partition.
   */
  void mount();

  /**
   * @brief Get the virtual file system for this partition.
   * 
   * @return vfs::Vfs* This partition's virtual file system.
   */
  vfs::Vfs* getVfs();

  /**
   * @brief Get the Data Provider for this partition.
   * 
   * @return io::data::DataProvider* 
   */
  io::data::DataProvider* getDataProvider();

  /**
   * @brief Get the length of this partition.
   * 
   * @return uint64_t 
   */
  uint64_t getLength() const;

  /**
   * @brief Get the start location of this partition in the disk.
   * 
   * @return uint64_t 
   */
  uint64_t getStart() const;

  /**
   * @brief Get the end location of this partition in the disk.
   * 
   * @return uint64_t 
   */
  uint64_t getEnd() const;
};

} /* namespace disk */

#endif /* PARTITION_HPP */
