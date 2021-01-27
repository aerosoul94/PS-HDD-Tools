#ifndef FAT_ADAPTER_HPP
#define FAT_ADAPTER_HPP

#include <io/data/data_provider.hpp>
#include <vfs/adapters/adapter.hpp>
#include "fat/fat_.h"
#include "fat_boot_sector.hpp"
#include "fat_file_allocation_table.hpp"

namespace vfs {
namespace adapters {

class FatAdapter : public VfsAdapter
{
public:
  FatAdapter(io::data::DataProvider* dataProvider, bool swap = false);

  VfsDirectory* mount() override;
  void unmount() override;

private:
  VfsDirectory* readRootDirectory();
  void loadDirectory(std::vector<char>& buffer, VfsDirectory* root);
  uint64_t clusterToFileAreaByteOffset(uint32_t clusterIndex);

  FatBootSector* bootSector;
  FatFileAllocationTable* fat;
  uint64_t fileAreaByteOffset;
  uint32_t bytesPerCluster;
  uint32_t maxDirents;
  bool needsSwap;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* FAT_ADAPTER_HPP */
