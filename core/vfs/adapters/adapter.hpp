#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include <vfs/file.hpp>
#include <vfs/directory.hpp>

#include <io/data/data_provider.hpp>

#include <cstdint>

namespace vfs {

/**
 * *** File System Operations ***
 * Mount:
 *   Returns the VfsNode that represents the root.
 * Unmount:
 *   Unmounts the file system
 * GetFsInfo:
 *   Get information about the file system (block size, ...)
 * Create/Open:
 *   Create a new file and return the file handle
 * Close:
 *   Close a file handle
 * Read:
 *   Read from a file
 * Write:
 *   Write to a file
 * List:
 *   Get a list of VfsNodes from within a directory
 * GetFileInfo:
 *   Get information about a file
 * SetFileInfo:
 *   Set file information
 **/

// struct IoRequestParams
// {
//   uint64_t offset;
// };

class VfsAdapter
{
public:
  VfsAdapter(io::data::DataProvider* dataProvider);

  virtual VfsDirectory* mount() = 0;
  virtual void unmount() = 0;

  // Eventually we will need more dynamic accessor methods, but for now,
  // we will keep it simple and just create an index of the file system.
  //virtual void getFsInfo() = 0;
  //virtual VfsFile* create(std::string& path) = 0;
  //virtual void remove(VfsFile* entry) = 0;
  //virtual void read(VfsFile* file, IoRequestParams* params) = 0;
  //virtual void write(VfsFile* file, IoRequestParams* params) = 0;
  //virtual std::vector<VfsNode*> list(VfsDirectory* dir) = 0;
  //virtual void getFileInfo(VfsFile* file, VfsFileInfo* info) = 0;
  //virtual void setFileInfo(VfsFile* file) = 0;

protected:
  io::data::DataProvider* dataProvider;
};

} /* namespace vfs */

#endif /* ADAPTER_HPP */
