#ifndef PHYSICAL_DISK_STREAM_HPP
#define PHYSICAL_DISK_STREAM_HPP

#include "disk_stream.hpp"

#include <string>

#if defined(_WIN32)
  #include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

namespace io {
namespace stream {

/**
 * A DiskStream for physical devices (e.g. PhysicalDriveX or /dev/sda).
 * 
 * This was added because standard file streams don't seem to support these 
 * file types.
 */
class PhysicalDiskStream : public DiskStream 
{
public:
  PhysicalDiskStream(std::string &);
#if defined(_WIN32)
  PhysicalDiskStream(HANDLE handle);
#endif

  uint64_t read(char *data, uint32_t length) override;
  uint64_t seek(int64_t offset, uint32_t whence = 0) override;
  uint64_t tell() override;
  uint64_t getLength() const override;

private:
#if defined(_WIN32)
  HANDLE handle;
#elif defined(__unix__) || defined(__APPLE__)
  int fd;
#endif
  uint64_t length;
};

} /* namespace stream */
} /* namepsace io */

#endif /* PHYSICAL_DISK_STREAM_HPP */
