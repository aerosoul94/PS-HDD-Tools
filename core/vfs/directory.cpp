#include "directory.hpp"

namespace vfs {

VfsDirectory::VfsDirectory() : VfsNode(VfsNodeType::DIRECTORY) {}

void VfsDirectory::addChild(VfsNode* child)
{
  children.push_back(child);
}

std::vector<VfsNode*> VfsDirectory::getChildren()
{
  return children;
}

uint32_t VfsDirectory::getChildCount() const
{
  return children.size();
}

} /* namespace vfs */