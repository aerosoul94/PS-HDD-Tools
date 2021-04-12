#ifndef BOUNDED_DATA_PROVIDER_HPP
#define BOUNDED_DATA_PROVIDER_HPP

#include "data_provider.hpp"

namespace io {
namespace data {

/**
 * @brief DataProvider with boundaries.
 * 
 * This creates a sub-view of a parent DataProvider.
 */
class BoundedDataProvider : public DataProvider
{
public:
  BoundedDataProvider(DataProvider* base, 
    uint64_t start, uint64_t length, uint32_t sectorSize);

  uint64_t read(char* data, uint32_t length) override;
  uint64_t seek(int64_t offset, uint32_t whence = 0) override;
  uint64_t tell() override;
  uint64_t getLength() const override;

private:
  uint64_t position;
  uint64_t start;
  uint64_t end;
  uint64_t length;
};

} /* namespace data */
} /* namespace io */

#endif /* BOUNDED_DATA_PROVIDER_HPP */
