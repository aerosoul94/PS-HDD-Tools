#ifndef FILE_ALLOCATION_TABLE_HPP
#define FILE_ALLOCATION_TABLE_HPP

#include <io/data/data_provider.hpp>

#include "fat_boot_sector.hpp"
#include "fat/fat_.h"

namespace vfs {
namespace adapters {

class FatFileAllocationTable
{
public:
  FatFileAllocationTable(io::data::DataProvider* dataProvider, 
    FatBootSector* bootSector);

  void getClusterChain(uint32_t firstCluster, std::vector<uint32_t>& chain);

private:
  void readFileAllocationTable();

  uint32_t getFatEntry(uint32_t clusterIndex);
  uint32_t getFat12Entry(uint32_t clusterIndex);
  uint32_t getFat16Entry(uint32_t clusterIndex);
  uint32_t getFat32Entry(uint32_t clusterIndex);

  io::data::DataProvider* dataProvider;
  FatBootSector* bootSector;

  std::vector<uint8_t> fileAllocationTableBuffer;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* FILE_ALLOCATION_TABLE_HPP */