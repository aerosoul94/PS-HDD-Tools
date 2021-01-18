#ifndef DISK_FORMAT_HPP
#define DISK_FORMAT_HPP

#include <fstream>
#include <vector>

#include <disk/disk.hpp>
#include <disk/disk_config.hpp>
#include <io/data/data_provider.hpp>
#include <io/stream/disk_stream.hpp>

namespace formats {

/// Abstract interface for a disk format builder
class IDiskFormat
{
public:
  /**
   * @brief This method should return whether or not the disk matches a disk 
   * format.
   * 
   * @param disk The input device.
   * @param config Disk information provided by the user.
   * @return true If the disk format has been detected.
   * @return false If the disk format was not detected.
   */
  virtual bool match(disk::Disk* disk, disk::DiskConfig* config) = 0;

  /**
   * @brief This method builds the Disk object.
   * 
   * @param disk The input disk context.
   * @param config Disk information provided by the user.
   */
  virtual void build(disk::Disk* disk, disk::DiskConfig* config) = 0;
};

} /* namespace formats */

#endif /* DISK_FORMAT_HPP */
