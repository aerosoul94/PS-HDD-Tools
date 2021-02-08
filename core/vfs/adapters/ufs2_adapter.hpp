#ifndef UFS2_ADAPTER_HPP
#define UFS2_ADAPTER_HPP

#include "adapter.hpp"

#include "ufs/types.h"
#include "ufs/ffs/fs.h"
#include "ufs/ufs/dir.h"
#include "ufs/ufs/dinode.h"

namespace vfs {
namespace adapters {

class Ufs2Adapter : public VfsAdapter
{
public:
  Ufs2Adapter(io::data::DataProvider* dataProvider, bool swap = false);

  VfsDirectory* mount();
  void unmount();

private:
  void getInode(uint32_t ino, ufs2_dinode* inode);
  void loadDirectory(VfsDirectory* root, ufs2_dinode* dinode);
  std::vector<uint32_t> getBlockListForInode(ufs2_dinode* inode);
  std::vector<char> readBlocks(std::vector<uint32_t> blockList);
  void swapInode(ufs2_dinode* inode);
  void swapDirect(direct* di);
  void swapSuperblock(fs* superblock);
  bool checkSuperblock(fs* superblock);

  fs* super;
  bool needsSwap;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* UFS2_ADAPTER_HPP */