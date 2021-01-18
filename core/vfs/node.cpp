#include "node.hpp"

namespace vfs {

VfsNode::VfsNode(VfsNodeType type)
{
  this->type = type;
}

VfsNodeType VfsNode::getType() const
{
  return this->type;
}

void VfsNode::setName(std::string name)
{
  this->name = name;
}

std::string VfsNode::getName() const
{
  return this->name;
}

void VfsNode::setParent(VfsNode* parent)
{
  this->parent = parent;
}

} /* namespace vfs */