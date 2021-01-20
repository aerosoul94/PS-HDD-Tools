#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <io/data/data_provider.hpp>
#include <vfs/file_system.hpp>

namespace disk {

class Disk;

/// Partition context
class Partition
{
public:
  Partition(Disk* disk, uint64_t start, uint64_t length);
  ~Partition();

  /**
   * @brief Mount the file system that exists in this partition.
   */
  void mount();

  /**
   * @brief Get this partition's name.
   * 
   * @return std::string The partition's name.
   */
  const std::string getName();

  /**
   * @brief Set this partition's name.
   * 
   * @param name The partition's name.
   */
  void setName(std::string name);

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

private:
  std::string name;
  uint64_t start, end, length;
  io::data::DataProvider* dataProvider; // Should be a BoundedDataProvider
  // NOTE: I suppose it's possible to have more than one file system
  vfs::Vfs* vfs; 
};

} /* namespace disk */

#endif /* PARTITION_HPP */
