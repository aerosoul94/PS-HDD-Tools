#ifndef CELL_DECRYPTED_FORMAT_HPP
#define CELL_DECRYPTED_FORMAT_HPP

#include "disk_format.hpp"

namespace formats {
  
/**
 * @brief Detects and loads a decrypted PS3 disk format.
 * 
 * Because this is an unconventional format, it will be kept separate from CellDiskFormat.
 */
class CellDecryptedDiskFormat  : public IDiskFormat
{
public:
  CellDecryptedDiskFormat() {}

  bool match(disk::Disk* disk, disk::DiskConfig* config) override;
  void build(disk::Disk* disk, disk::DiskConfig* config) override;

private:
  bool checkDiskLabel(io::data::DataProvider* dataProvider);
  void addFatPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length);
  void addUfsPartition(disk::Disk* disk, std::string name, uint64_t start, uint64_t length);

  // TODO: This should be passed in through DiskConfig
  const uint32_t kSectorSize = 0x200;
};

} /* namespace formats */

#endif /* CELL_DECRYPTED_FORMAT_HPP */
