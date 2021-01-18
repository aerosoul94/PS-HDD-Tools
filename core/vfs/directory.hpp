#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "node.hpp"

#include <vector>

namespace vfs {

class VfsDirectory : public VfsNode
{
public:
  VfsDirectory();

  void addChild(VfsNode* child);
  std::vector<VfsNode*> getChildren(); 
  uint32_t getChildCount() const;

private:
  std::vector<VfsNode*> children;
};

} /* namespace vfs */

#endif /* DIRECTORY_HPP */