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

VfsDateTime* VfsNode::getLastModifiedTime()
{
  return &this->lastModifiedTime;
}

VfsDateTime* VfsNode::getLastAccessTime()
{
  return &this->lastAccessTime;
}

VfsDateTime* VfsNode::getCreationTime()
{
  return &this->creationTime;
}

void VfsNode::setLastModifiedTime(int day, int month, int year,
  int second, int minute, int hour)
{
  lastModifiedTime.setDateTime(day, month, year, second, minute, hour);
}

void VfsNode::setLastAccessTime(int day, int month, int year,
  int second, int minute, int hour)
{
  lastAccessTime.setDateTime(day, month, year, second, minute, hour);
}

void VfsNode::setCreationTime(int day, int month, int year,
  int second, int minute, int hour)
{
  creationTime.setDateTime(day, month, year, second, minute, hour);
}

void VfsNode::setParent(VfsNode* parent)
{
  this->parent = parent;
}

void VfsNode::addOffset(std::string key, uint64_t value)
{
  offsets.insert({{key, value}});
}

std::vector<uint64_t> VfsNode::getOffsets(std::string& key)
{
  std::vector<uint64_t> result;
  auto range = offsets.equal_range(key);
  for (auto it = range.first; it != range.second; it++)
    result.push_back(it->second);
  return result;
}

} /* namespace vfs */