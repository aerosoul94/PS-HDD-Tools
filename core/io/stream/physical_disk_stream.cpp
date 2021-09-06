#include "physical_disk_stream.hpp"

#if defined(__unix__) || defined(__APPLE__)
  #include <sys/ioctl.h>
  #include <fcntl.h>
  #include <unistd.h>
#endif

#if defined(__linux__)
  #include <linux/fs.h>
#elif defined(__APPLE__)
  #include <sys/disk.h>
#endif

namespace io {
namespace stream {

PhysicalDiskStream::PhysicalDiskStream(std::string& path) 
{
#if defined(_WIN32)
  handle = CreateFile(
    path.c_str(), // lpFileName
    GENERIC_READ, // dwDesiredAccess
    0,            // dwShareMode
    NULL,         // lpSecurityAttributes
    OPEN_EXISTING,// dwCreationDisposition
    0,            // dwFlagsAndAttributes
    NULL          // hTemplateFile
  );

  DWORD dw = GetLastError();

  GET_LENGTH_INFORMATION info = {0};
  DWORD bytesReturned;
  DeviceIoControl(handle, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &info,
                  sizeof(info), &bytesReturned, NULL);

  this->length = info.Length.QuadPart;
#elif defined(__unix__)
  fd = open(path.c_str(), O_RDONLY);

 #if defined(__linux__)
  uint64_t size = 0;
  ioctl(fd, BLKGETSIZE64, &size);
  size *= 512ULL;
#elif defined(__APPLE__)
  uint32_t bsize = 0;
  uint64_t count = 0;
  ioctl(fd, DKIOCGETBLOCKCOUNT, &count);
  ioctl(fd, DKIOCGETBLOCKSIZE, &bsize);
  uint64_t size = count * bsize;
#endif

  this->length = size;
#elif defined(__APPLE__)
#endif
}

#if defined(_WIN32)
PhysicalDiskStream::PhysicalDiskStream(HANDLE handle)
  : handle(handle)
{
  GET_LENGTH_INFORMATION info = {0};
  DWORD bytesReturned;
  DeviceIoControl(handle, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &info,
                  sizeof(info), &bytesReturned, NULL);

  this->length = info.Length.QuadPart;
}
#endif

uint64_t PhysicalDiskStream::read(char* data, uint32_t length)
{
#if defined(_WIN32)
  DWORD bytesRead;

  BOOL result = ReadFile(handle, (LPVOID)data, length, &bytesRead, NULL);

  if (result)
    return bytesRead;

  // TODO: Set a standard for what we return.
  return -1;
#elif defined(__unix__) || defined(__APPLE__)
  return ::read(fd, (void *)data, length);
#else
  throw "read() not implemented";
#endif
}

uint64_t PhysicalDiskStream::seek(int64_t offset, uint32_t whence)
{
#if defined(_WIN32)
  LARGE_INTEGER pos;

  SetFilePointerEx(handle, *(LARGE_INTEGER*)&offset, &pos, whence);

  return (uint64_t)pos.QuadPart;
#elif defined(__unix__) || defined(__APPLE__)
  // TODO: We can optimize by keeping track of position, 
  // then use pread() in our read() method.
  return lseek(fd, offset, whence);
#else
  throw "seek() not implemented";
#endif
}

uint64_t PhysicalDiskStream::tell()
{
#if defined(_WIN32)
  LARGE_INTEGER pos;
  LARGE_INTEGER offset = {0};

  SetFilePointerEx(handle, offset, &pos, FILE_CURRENT);

  return pos.QuadPart;
#elif defined(__unix__) || defined(__APPLE__)
  return lseek(fd, 0, SEEK_CUR);
#else
  throw "tell() not implemented";
#endif
}

uint64_t PhysicalDiskStream::getLength() const
{
  return length;
}

} /* namespace stream */
} /* namespace io */
