#ifndef CELL_SLIM_FORMAT_HPP
#define CELL_SLIM_FORMAT_HPP

#include <vector>
#include <stdint.h>

#include "cell_format.hpp"

namespace formats {

/**
 * @brief Detects and loads a slim PS3 disk format.
 */
class CellSlimDiskFormat : public CellDiskFormat
{
public:
  CellSlimDiskFormat() {}

  bool match(disk::Disk* disk, disk::DiskConfig* config) override;
  void build(disk::Disk* disk, disk::DiskConfig* config) override;

private:
  bool checkDiskLabel(io::data::DataProvider* dataProvider);
};

} /* namespace formats */

#endif /* CELL_SLIM_FORMAT_HPP */