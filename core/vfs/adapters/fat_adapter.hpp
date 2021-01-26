#ifndef FAT_ADAPTER_HPP
#define FAT_ADAPTER_HPP

#include <io/data/data_provider.hpp>
#include <vfs/adapters/adapter.hpp>
#include "fat/fat_.h"
#include "fat_boot_sector.hpp"

namespace vfs {
namespace adapters {

class FatAdapter : public VfsAdapter
{
public:
  FatAdapter(io::data::DataProvider* dataProvider, bool swap = false);

  VfsDirectory* mount() override;
  void unmount() override;

private:
  void loadDirectory(VfsDirectory* root);

  FatBootSector* bootSector;
  bool needsSwap;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* FAT_ADAPTER_HPP */
