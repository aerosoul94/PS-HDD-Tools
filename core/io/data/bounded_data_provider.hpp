#ifndef BOUNDED_DATA_PROVIDER_HPP
#define BOUNDED_DATA_PROVIDER_HPP

#include "data_provider.hpp"

/// A DataProvider with boundaries (a sub stream)
class BoundedDataProvider : public DataProvider
{
public:
  BoundedDataProvider(DataProvider* base, 
    uint64_t start, uint64_t length, uint32_t sectorSize);

  uint64_t read(char* data, uint32_t length);
  uint64_t seek(int64_t offset, uint32_t whence = 0);
  uint64_t tell();
  uint64_t getLength();

private:
  uint64_t position;
  uint64_t start;
  uint64_t end;
  uint64_t length;
};

#endif /* BOUNDED_DATA_PROVIDER_HPP */
