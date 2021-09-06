#include "disk_stream_factory.hpp"

#include "file_disk_stream.hpp"
#include "physical_disk_stream.hpp"

#if defined(_WIN32)
  #include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

namespace io {
namespace stream {

DiskStream *DiskStreamFactory::getStream(std::string &path) {

  if (isDevice(path)) {
    return new io::stream::PhysicalDiskStream(path);
  }

  if (isFile(path)) {
    return new io::stream::FileDiskStream(path);
  }

  return nullptr;
}

bool DiskStreamFactory::isDevice(std::string &path) {
  // TODO: C++17 has std::filesystem::is_block_file()
  //std::filesystem::path path(path);
  //return std::filesystem::is_block_file(path);

#if defined(_WIN32)

  DWORD attr = GetFileAttributes(path.c_str());

  return attr & FILE_ATTRIBUTE_DEVICE;

#elif defined(__unix__) || defined(__APPLE__)

  struct stat st;
  stat(path.c_str(), &st);
  return S_ISBLK(st.st_mode);

#endif
}

bool DiskStreamFactory::isFile(std::string &path) {
  // TODO: C++17 has std::filesystem::is_regular_file()
  //std::filesystem::path path(path);
  //return std::filesystem::is_regular_file(path);

#if defined(_WIN32)

  DWORD attr = GetFileAttributes(path.c_str());

  // TODO: Might need some work. This just checks that it is not a device.
  // There doesn't seem to be a way to check if it's a file?
  return !(attr & FILE_ATTRIBUTE_DEVICE);

#elif defined(__unix__) || defined(__APPLE__)

  struct stat st;
  stat(path.c_str(), &st);
  return S_ISREG(st.st_mode);

#endif
}

} /* namespace stream */
} /* namespace io */
