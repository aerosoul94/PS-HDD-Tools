#ifndef DISK_STREAM_HPP
#define DISK_STREAM_HPP

#include <vector>

/// Abstract interface for a disk stream
class DiskStream
{
public:
  virtual uint64_t read(char* data, uint32_t length) = 0;
  virtual uint64_t seek(int64_t offset, uint32_t whence = 0) = 0;
  virtual uint64_t tell() = 0;
  virtual uint64_t getLength() const = 0;
};

#endif /* DISK_STREAM_HPP */
