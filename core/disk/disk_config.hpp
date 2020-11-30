#ifndef DISK_CONFIG_HPP
#define DISK_CONFIG_HPP

#include <vector>
#include <io/stream/disk_stream.hpp>

/// Initial disk configuration information. This information should be
/// provided by the user.
class DiskConfig final
{
public:
  DiskConfig() {}

  void setStream(DiskStream*);
  void setKeys(const char* data, uint32_t length);
  std::vector<char> getKeys() const;
  DiskStream* getStream() const;

private:
  std::vector<char> keys;
  DiskStream* stream;
};

#endif /* DISK_CONFIG_HPP */
