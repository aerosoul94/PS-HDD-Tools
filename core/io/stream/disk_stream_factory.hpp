#ifndef DISK_STREAM_FACTORY_HPP
#define DISK_STREAM_FACTORY_HPP

#include "disk_stream.hpp"

#include <string>

namespace io {
namespace stream {

class DiskStreamFactory 
{
public:
  /**
   * Detects the file type and returns the correct DiskStream type.
   * 
   * @param path The path to the file or device.
   * @returns io::stream::DiskStream*
   */
  static DiskStream *getStream(std::string &path);

private:
  static bool isFile(std::string &path);
  static bool isDevice(std::string &path);
};

} /* namespace stream */
} /* namespace io */

#endif /* DISK_STREAM_FACTORY_HPP */
