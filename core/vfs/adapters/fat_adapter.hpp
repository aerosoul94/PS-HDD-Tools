#ifndef FAT_ADAPTER_HPP
#define FAT_ADAPTER_HPP

#include <io/data/data_provider.hpp>
#include <vfs/adapters/adapter.hpp>
#include "fat/fat_.h"

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

  bpb_ex* b;
  bool needsSwap;
  enum class FatType {
    FAT12,
    FAT16,
    FAT32
  } type;

  uint32_t rootDirFirstCluster;
  uint64_t fatByteOffset;
  uint32_t bytesPerFat;
  uint32_t numberOfFats;
  uint64_t fileAreaByteOffset;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* FAT_ADAPTER_HPP */
