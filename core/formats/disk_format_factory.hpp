#ifndef DISK_FORMAT_FACTORY_HPP
#define DISK_FORMAT_FACTORY_HPP

#include <vector>
#include <string>

#include "disk_format.hpp"

/// Disk format detection service
class DiskFormatFactory
{
  std::vector<IDiskFormat*> formats;
public:
  DiskFormatFactory();

  static DiskFormatFactory* getInstance();
  Disk* detectFormat(DiskConfig* config);
  void registerFormat(IDiskFormat* format);
};

#endif /* DISK_FORMAT_FACTORY_HPP */
