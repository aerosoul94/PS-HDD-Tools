#ifndef DISK_HPP
#define DISK_HPP

#include <vector>
#include <string>

#include <io/data/data_provider.hpp>

namespace disk {

class Partition;

/**
 * @brief Context class for a disk.
 */
class Disk
{
public:
  Disk(io::stream::DiskStream* stream, uint32_t sectorSize);
  ~Disk();

  /**
   * @brief Get the Data Provider for this disk.
   * 
   * @return io::data::DataProvider* 
   */
  io::data::DataProvider* getDataProvider();

  /**
   * @brief Get a list of Partition's that exist in this disk.
   * 
   * @return const std::vector<Partition*>& 
   */
  const std::vector<Partition*>& getPartitions() const;

  /**
   * @brief Get a partition by its name.
   * 
   * @param name The name of the partition.
   * @return Partition* 
   */
  Partition* getPartitionByName(std::string& name) const;

  /**
   * @brief Add a Partition into this disk.
   * 
   * @param start The start location of the partition
   * @param length The length of the partition
   * @return Partition* The partition if it was added or nullptr.
   */
  Partition* addPartition(uint64_t start, uint64_t length);

  /**
   * @brief Get the size of a single sector in this disk.
   * 
   * @return uint32_t Sector size
   */
  uint32_t getSectorSize() const;

private:
  io::data::DataProvider* dataProvider;
  std::vector<Partition*> partitions;
  uint32_t sectorSize;
};

} /* namespace disk */

#endif /* DISK_HPP */
