#ifndef DISK_FORMAT_HPP
#define DISK_FORMAT_HPP

#include <fstream>
#include <vector>

#include <disk/disk.hpp>
#include <disk/disk_config.hpp>
#include <io/data/data_provider.hpp>
#include <io/stream/disk_stream.hpp>

/// Abstract interface for a disk format builder
class IDiskFormat
{
public:
  virtual bool match(Disk* disk, DiskConfig* config) = 0;
  virtual void build(Disk* disk, DiskConfig* config) = 0;
};

#endif /* DISK_FORMAT_HPP */
