#ifndef DISK_CONFIG_HPP
#define DISK_CONFIG_HPP

#include <vector>
#include <io/stream/disk_stream.hpp>

namespace disk {

/// Initial disk configuration information. This information should be
/// provided by the user.
class DiskConfig final
{
public:
  DiskConfig() {}

  void setStream(io::stream::DiskStream*);
  void setKeys(const char* data, uint32_t length);
  std::vector<char> getKeys() const;
  io::stream::DiskStream* getStream() const;

private:
  std::vector<char> keys;
  io::stream::DiskStream* stream;
};

} /* namespace disk */

#endif /* DISK_CONFIG_HPP */
