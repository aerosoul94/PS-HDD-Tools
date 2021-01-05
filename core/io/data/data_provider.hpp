#ifndef DATA_PROVIDER_HPP
#define DATA_PROVIDER_HPP

#include <vector>

#include <io/stream/disk_stream.hpp>
#include <crypto/crypto_strategy.hpp>

namespace io {
namespace data {

/// Data provider with optional post/pre processing. 
/// This also provides sector aligned reads.
class DataProvider
{
public:
  DataProvider(stream::DiskStream* stream, uint32_t sectorSize);

  void setCryptoStrategy(crypto::CryptoStrategy* strategy);
  void setSectorBias(uint32_t sector);
  virtual uint64_t read(char* data, uint32_t length);
  virtual uint64_t seek(int64_t offset, uint32_t whence = 0);
  virtual uint64_t tell();
  crypto::CryptoStrategy* getCryptoMethod() const;
  stream::DiskStream* getStream() const;
  uint64_t getLength() const;
  
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
