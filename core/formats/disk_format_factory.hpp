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

  /**
   * @brief Try and detect the disk format.
   * 
   * @param config 
   * @return disk::Disk* 
   */
  disk::Disk* detectFormat(disk::DiskConfig* config);

  /**
   * @brief Register a disk format.
   * 
   * @param format 
   */
  void registerFormat(IDiskFormat* format);
};

} /* namespace formats */

#endif /* DISK_FORMAT_FACTORY_HPP */
