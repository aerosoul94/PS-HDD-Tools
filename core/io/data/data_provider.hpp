#ifndef DATA_PROVIDER_HPP
#define DATA_PROVIDER_HPP

#include <vector>

#include <io/stream/disk_stream.hpp>
#include <crypto/crypto_strategy.hpp>

namespace io {
namespace data {

/**
 * @brief Provides data from the disk.
 * 
 * This data provider takes an input `DiskStream` which is what is used to 
 * access raw data from a disk and uses an optional `CryptoStrategy` to provide 
 * post/pre processing. It also ensures that data is read using sector aligned 
 * reads.
 */
class DataProvider
{
public:
  DataProvider(stream::DiskStream* stream, uint32_t sectorSize);
  virtual ~DataProvider() = default;
  /**
   * @brief Set a crypto strategy for this data provider.
   * 
   * This crypto strategy will be used to either decrypt data after it is read,
   * or to encrypt data before it is written to the disk.
   * 
   * @param strategy 
   */
  void setCryptoStrategy(crypto::CryptoStrategy* strategy);
  void setSectorBias(uint32_t sector);

  /**
   * @brief Read data from the disk.
   * 
   * @param data The location to store the data being read.
   * @param length The amount of bytes to read.
   * @return uint64_t The amount of bytes read.
   */
  virtual uint64_t read(char* data, uint32_t length);

  /**
   * @brief Seek to a location into the disk.
   * 
   * @param offset The offset into the disk to seek to.
   * @param whence Where to seek from.
   *  Use 0 to seek to an absolute offset.
   *  Use 1 to seek from the current position.
   *  Use 2 to seek from the end of the stream.
   * @return uint64_t The location seeked to.
   */
  virtual uint64_t seek(int64_t offset, uint32_t whence = 0);

  /**
   * @brief Returns the current stream position.
   * 
   * @return uint64_t Current position into the stream.
   */
  virtual uint64_t tell();

  /**
   * @brief Get the crypto strategy being used for this data provider.
   * 
   * @return crypto::CryptoStrategy* 
   */
  crypto::CryptoStrategy* getCryptoStrategy() const;

  /**
   * @brief Get the raw input disk stream.
   * 
   * @return stream::DiskStream* 
   */
  stream::DiskStream* getStream() const;

  /**
   * @brief Get the length of the input disk stream.
   * 
   * @return uint64_t 
   */
  virtual uint64_t getLength() const;
  
protected:
  virtual uint64_t readInternal(uint64_t offset, char* data, uint32_t length);

  stream::DiskStream* stream;
  crypto::CryptoStrategy* cryptoStrategy;
  uint32_t sectorSize;
  uint32_t sectorBias;
};

} /* namespace data */
} /* namespace io */

#endif /* DATA_PROVIDER_HPP */
