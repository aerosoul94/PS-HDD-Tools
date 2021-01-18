#include "file.hpp"

namespace vfs {

VfsFile::VfsFile() : VfsNode(VfsNodeType::FILE) {}

void VfsFile::setFileSize(uint64_t size)
{
  this->fileSize = size;
}

uint64_t VfsFile::getFileSize() const
{
  return this->fileSize;
}

} /* namespace vfs */