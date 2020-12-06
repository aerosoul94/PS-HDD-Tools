#include "bounded_data_provider.hpp"

#include <stdexcept>

BoundedDataProvider::BoundedDataProvider(
  DataProvider* base,   // Parent data provider
  uint64_t start,       // Offset in bytes to start of boundary
  uint64_t length,      // Length in bytes of boundary
  uint32_t sectorSize)
  : DataProvider(base->getStream(), sectorSize)
{
  this->position = 0;
  this->cryptoStrategy = base->getCryptoMethod();
  this->start = start;
  this->end = start + length;
  this->length = length;
}

uint64_t BoundedDataProvider::read(char* data, uint32_t length)
{
  // TODO: Create a readInternal method so that we don't have to embed
  //   this code that I copied from DataProvider::read

  // Seek to a sector aligned offset
  auto pos = this->position;
  auto offset = pos;
  if (pos % this->sectorSize != 0)
    offset -= pos % this->sectorSize;
  this->stream->seek(this->start + offset);

  // We need an aligned length
  auto alignedLength = (length + (this->sectorSize - 1)) 
    & ~(this->sectorSize - 1);

  std::vector<char> tempBuf(alignedLength);

  // Proper disk encryption should use a sector index
  uint32_t sector = (this->start + offset) / this->sectorSize;

  // Need to make sure we don't read past end of stream
  auto readLen = this->stream->read(tempBuf.data(), alignedLength);

  // Decrypt if needed
  if (this->cryptoStrategy)
    this->cryptoStrategy->decrypt(tempBuf.data(), sector - sectorBias, alignedLength);

  // Only read what we need
  memcpy(data, tempBuf.data() + (pos - offset), length);
  this->position += readLen;
  return readLen;
}

uint64_t BoundedDataProvider::seek(int64_t offset, uint32_t whence)
{
  // Contain the position within our boundaries
  switch (whence) {
    case 0:
      // Seek from beginning
      if (offset < 0)
        throw std::invalid_argument("Cannot seek before start of stream");
      if (offset > this->length)
        offset = this->length;
      this->position = offset;
      break;
    case 1:
      // Seek from current position
      if (this->position + offset < 0)
        throw std::invalid_argument("Cannot seek before start of stream");
      if (this->position + offset > this->length)
        offset = this->length - this->position;
      this->position += offset;
      break;
    case 2:
      // Seek from end, usually negative. Positive offsets have no effect.
      if (this->length + offset < 0)
        throw std::invalid_argument("Cannot seek before start of stream");
      if (this->length + offset > this->length)
        offset = 0;
      this->position = this->length + offset;
      break;
    default:
      throw std::invalid_argument("Invalid whence. Must be 0, 1, or 2.");
  }
  return this->position;
}

uint64_t BoundedDataProvider::tell()
{
  return this->position;
}

uint64_t BoundedDataProvider::getLength()
{
  return this->length;
}
