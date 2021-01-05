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
  virtual bool match(disk::Disk* disk, disk::DiskConfig* config) = 0;
  virtual void build(disk::Disk* disk, disk::DiskConfig* config) = 0;
};

} /* namespace formats */

#endif /* DISK_FORMAT_HPP */
