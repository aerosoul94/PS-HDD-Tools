#ifndef DISK_CONFIG_HPP
#define DISK_CONFIG_HPP

#include <io/stream/disk_stream.hpp>
#include <vector>

namespace disk {

/// Initial disk configuration information. This information should be
/// provided by the user.
class DiskConfig final {
  public:
  DiskConfig() { }

  /**
   * @brief Set the DiskStream.
   */
  void setStream(io::stream::DiskStream*);

  /**
   * @brief Use this to provide the disk crypto keys.
   *
   * @param data The crypto key data.
   * @param length The length of the crypto key data.
   */
  void setKeys(const char* data, uint32_t length);

  /**
   * @brief Get the crypto key data.
   *
   * @return std::vector<char>
   */
  std::vector<char> getKeys() const;

  /**
   * @brief Get the DiskStream.
   *
   * @return io::stream::DiskStream*
   */
  io::stream::DiskStream* getStream() const;

  private:
  std::vector<char> keys;
  io::stream::DiskStream* stream;
};

} /* namespace disk */

#endif /* DISK_CONFIG_HPP */
