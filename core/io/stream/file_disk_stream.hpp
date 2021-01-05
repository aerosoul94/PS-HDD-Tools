#ifndef FILE_DISK_STREAM_HPP
#define FILE_DISK_STREAM_HPP

#include "disk_stream.hpp"

#include <fstream>

namespace io {
namespace stream {

/// Standard file stream handler
class FileDiskStream : public DiskStream
{
public:
  FileDiskStream(std::ifstream&);

  uint64_t read(char* data, uint32_t length) override;
  uint64_t seek(int64_t offset, uint32_t whence = 0) override;
  uint64_t tell() override;
  uint64_t getLength() const override;

private:
  std::ifstream& stream;
  uint64_t length;
};

} /* namespace stream */
} /* namespace io */

#endif /* FILE_DISK_STREAM_HPP */
