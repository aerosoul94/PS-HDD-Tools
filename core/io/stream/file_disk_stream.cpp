#include "file_disk_stream.hpp"

FileDiskStream::FileDiskStream(std::ifstream& file)
  : stream(file)
{
  const auto pos = file.tellg();
  file.seekg(0, 2);
  this->length = file.tellg();
  file.seekg(pos);
}

uint64_t FileDiskStream::read(char* data, uint32_t length)
{
  return stream.read(data, length).gcount();
}

uint64_t FileDiskStream::seek(int64_t offset, uint32_t whence)
{
  return stream.seekg(offset, whence).tellg();
}

uint64_t FileDiskStream::tell()
{
  return stream.tellg();
}

uint64_t FileDiskStream::getLength() const
{
  return this->length;
}
