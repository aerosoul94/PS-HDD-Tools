#ifndef CELL_FORMAT_HPP
#define CELL_FORMAT_HPP

#include <vector>
#include <stdint.h>

#include "disk_format.hpp"
#include "partition/ps3.h"

namespace formats {

/**
 * @brief Detects and loads the PS3 disk format.
 */
class CellDiskFormat : public IDiskFormat
{
protected:
  /**
   * @brief Generate crypto keys from an eid root key.
   * 
   * @param eidRootKey 
   */
  void generateKeys(std::vector<char>& eidRootKey);

  void addVflashPartitions(disk::Disk* disk, io::data::DataProvider* dataProvider, d_partition* vflash);
  void addFatPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length);
  void addUfsPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length);

  std::vector<char> ataKeys;
  std::vector<char> encDecKeys;
  // TODO: We should be getting this from DiskConfig
  const uint32_t kSectorSize = 0x200;
};

} /* namespace formats */

#endif /* CELL_FORMAT_HPP */
