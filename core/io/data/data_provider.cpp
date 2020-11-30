#include "data_provider.hpp"

#include <crypto/crypto_strategy.hpp>
#include <io/stream/disk_stream.hpp>

DataProvider::DataProvider(DiskStream* stream, uint32_t sectorSize)
  : stream(stream), sectorSize(sectorSize)
{
  this->cryptoMethod = nullptr;
  this->sectorBias = 0;
}

void DataProvider::setCryptoStrategy(CryptoStrategy* strategy)
{
  this->cryptoMethod = strategy;
}

void DataProvider::setSectorBias(uint32_t sector)
{
  this->sectorBias = sector;
}

uint64_t DataProvider::read(char* data, uint32_t length)
{
  // Seek to a sector aligned offset
  auto pos = this->stream->tell();
  auto offset = pos;
  if (pos % this->sectorSize != 0)
    offset -= pos % this->sectorSize;
  this->stream->seek(offset);

  // We need an aligned length
  auto alignedLength = (length + (this->sectorSize - 1)) 
    & ~(this->sectorSize - 1);

  std::vector<char> tempBuf(alignedLength);

  // Proper disk encryption should use a sector index
  uint32_t sector = this->stream->tell() / this->sectorSize;

  // Need to make sure we don't read past end of stream
  auto readLen = this->stream->read(tempBuf.data(), alignedLength);

  // Decrypt if needed
  if (this->cryptoMethod)
    this->cryptoMethod->decrypt(tempBuf.data(), sector - sectorBias, alignedLength);

  // Only read what we need
  memcpy(data, tempBuf.data() + (pos - offset), length);
  
  return readLen;
}

uint64_t DataProvider::seek(int64_t offset, uint32_t whence)
{
  return this->stream->seek(offset, whence);
}

uint64_t DataProvider::tell()
{
  return this->stream->tell();
}

DiskStream* DataProvider::getStream() const
{
  return this->stream;
}

CryptoStrategy* DataProvider::getCryptoMethod() const
{
  return this->cryptoMethod;
}

uint64_t DataProvider::getLength() const
{
  return this->stream->getLength();
}