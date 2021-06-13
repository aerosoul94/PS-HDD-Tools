#include "data_provider.hpp"

#include <crypto/crypto_strategy.hpp>
#include <io/stream/disk_stream.hpp>

#include <cstring>

namespace io {
namespace data {

DataProvider::DataProvider(stream::DiskStream* stream, uint32_t sectorSize)
  : stream(stream), sectorSize(sectorSize)
{
  this->cryptoStrategy = nullptr;
  this->sectorBias = 0;
}

void DataProvider::setCryptoStrategy(crypto::CryptoStrategy* strategy)
{
  this->cryptoStrategy = strategy;
}

void DataProvider::setSectorBias(uint32_t sector)
{
  // TODO: Move this method to AesXtsStrategy or CryptoStrategy
  this->sectorBias = sector;
}

uint64_t DataProvider::read(char* data, uint32_t length)
{
  auto readLen = readInternal(this->tell(), data, length);
  return readLen;
}

uint64_t DataProvider::readInternal(uint64_t offset, char* data, uint32_t length)
{
  auto alignedOffset = offset;

  // Align our offset to the nearest sector
  if (alignedOffset % this->sectorSize != 0)
    alignedOffset -= offset % this->sectorSize;

  this->stream->seek(alignedOffset);

  auto alignedLength = ((offset - alignedOffset) + length + (this->sectorSize - 1))
    & ~(this->sectorSize - 1);

  std::vector<char> buffer(alignedLength);

  uint32_t sectorIndex = alignedOffset / this->sectorSize;

  auto readLen = this->stream->read(buffer.data(), alignedLength);

  if (this->cryptoStrategy)
    this->cryptoStrategy->decrypt(buffer.data(), sectorIndex - sectorBias, alignedLength);

  memcpy(data, buffer.data() + (offset - alignedOffset), length);

  return (readLen > length) ? length : readLen;
}

uint64_t DataProvider::seek(int64_t offset, uint32_t whence)
{
  return this->stream->seek(offset, whence);
}

uint64_t DataProvider::tell()
{
  return this->stream->tell();
}

stream::DiskStream* DataProvider::getStream() const
{
  return this->stream;
}

crypto::CryptoStrategy* DataProvider::getCryptoStrategy() const
{
  return this->cryptoStrategy;
}

uint64_t DataProvider::getLength() const
{
  return this->stream->getLength();
}

} /* namespace data */
} /* namespace io */
