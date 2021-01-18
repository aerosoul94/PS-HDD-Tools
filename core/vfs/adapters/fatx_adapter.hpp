#ifndef FATX_ADAPTER_HPP
#define FATX_ADAPTER_HPP

#include "adapter.hpp"

namespace vfs {
namespace adapters {

class FatXAdapter : public VfsAdapter
{
  VfsDirectory* mount(); // initialize file system
  void unmount(); // probably not needed
   // We will need:
   //  A bitmap of occupied blocks
   //  The sector size and block size
   //  Max file size
  // void getFsInfo();
  // void read(VfsFile* file, IoRequestParams* params);
  // void write(VfsFile* file, IoRequestParams* params);
  // std::vector<VfsNode*> list(VfsDirectory* dir);
  // void getFileInfo(VfsFile* file, VfsFileInfo* info);
};

} /* namespace adapters */
} /* namespace vfs */

#endif /* FATX_ADAPTER_HPP */