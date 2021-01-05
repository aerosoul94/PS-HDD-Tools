#include "disk_config.hpp"

namespace disk {

void DiskConfig::setKeys(const char* data, uint32_t length)
{
  this->keys.resize(length);
  memcpy(this->keys.data(), data, length);
}

std::vector<char> DiskConfig::getKeys() const
{
  return this->keys;
}

void DiskConfig::setStream(io::stream::DiskStream* stream)
{
  this->stream = stream;
}

io::stream::DiskStream* DiskConfig::getStream() const
{
  return this->stream;
}

} /* namespace disk */
