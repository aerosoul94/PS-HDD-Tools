#ifndef UFS2_ADAPTER_HPP
#define UFS2_ADAPTER_HPP

#include "adapter.hpp"
#include "utilities/endian.hpp"

#include "ufs/types.h"
#include "ufs/ffs/fs.h"
#include "ufs/ufs/dir.h"
#include "ufs/ufs/dinode.h"

#include <type_traits>

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
  void loadDataOffsets(VfsNode* node, ufs2_dinode* inode);
  void loadIndirectBlockTable(VfsNode* node, ufs2_daddr_t addr);
  
  /* In-place endian swap functions.. */

  template <typename T, typename std::enable_if<sizeof(T) == 8, bool>::type = true>
  T swap(T& v) {
    v = needsSwap ? swap64(v) : v;
    return v;
  }
  
  template <typename T, typename std::enable_if<sizeof(T) == 4, bool>::type = true>
  T swap(T& v) {
    v = needsSwap ? swap32(v) : v;
    return v;
  }
  
  template <typename T, typename std::enable_if<sizeof(T) == 2, bool>::type = true>
  T swap(T& v) {
    v = needsSwap ? swap16(v) : v;
    return v;
  }

  fs* super;
  bool needsSwap;
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* UFS2_ADAPTER_HPP */
