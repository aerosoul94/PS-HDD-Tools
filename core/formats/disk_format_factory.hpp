#ifndef DISK_FORMAT_FACTORY_HPP
#define DISK_FORMAT_FACTORY_HPP

#include <vector>
#include <string>

#include "disk_format.hpp"

namespace formats {

/// Disk format detection service
class DiskFormatFactory
{
  std::vector<IDiskFormat*> formats;
public:
  DiskFormatFactory();

  static DiskFormatFactory* getInstance();
  disk::Disk* detectFormat(disk::DiskConfig* config);
  void registerFormat(IDiskFormat* format);
};

} /* namespace formats */

#endif /* DISK_FORMAT_FACTORY_HPP */
