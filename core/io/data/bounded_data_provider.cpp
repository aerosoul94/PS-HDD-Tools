#include "bounded_data_provider.hpp"

#include <stdexcept>

namespace io {
namespace data {

BoundedDataProvider::BoundedDataProvider(
  DataProvider* base,   // Parent data provider
  uint64_t start,       // Offset in bytes to start of boundary
  uint64_t length,      // Length in bytes of boundary
  uint32_t sectorSize)
  : DataProvider(base->getStream(), sectorSize)
{
  this->position = 0;
  this->cryptoStrategy = base->getCryptoStrategy();
  this->start = start;
  this->end = start + length;
  this->length = length;
}

uint64_t BoundedDataProvider::read(char* data, uint32_t length)
{
  auto readLen = readInternal(this->start + this->tell(), data, length);
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

} /* namespace data */
} /* namespace io */
