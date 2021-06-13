#include "file_disk_stream.hpp"

namespace io {
namespace stream {

FileDiskStream::FileDiskStream(std::string& path)
{
  stream = new std::ifstream(path, std::ios::binary);
  const auto pos = stream->tellg();
  stream->seekg(0, std::ios_base::end);
  this->length = stream->tellg();
  stream->seekg(pos);
}

FileDiskStream::FileDiskStream(std::ifstream* file)
  : stream(file)
{
  const auto pos = file->tellg();
  file->seekg(0, std::ios_base::end);
  this->length = file->tellg();
  file->seekg(pos);
}

uint64_t FileDiskStream::read(char* data, uint32_t length)
{
  return stream->read(data, length).gcount();
}

uint64_t FileDiskStream::seek(int64_t offset, uint32_t whence)
{
  return stream->seekg(offset, std::ios_base::seekdir(whence)).tellg();
}

uint64_t FileDiskStream::tell()
{
  return stream->tellg();
}

uint64_t FileDiskStream::getLength() const
{
  return this->length;
}

} /* namespace stream */
} /* namespace io */
