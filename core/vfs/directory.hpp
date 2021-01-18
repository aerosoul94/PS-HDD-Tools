#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "node.hpp"

#include <vector>

namespace vfs {

class VfsDirectory : public VfsNode
{
public:
  VfsDirectory();

  /**
   * @brief Add a child node to this directory.
   * 
   * @param child 
   */
  void addChild(VfsNode* child);

  /**
   * @brief Get a list of child node's.
   * 
   * @return std::vector<VfsNode*> 
   */
  std::vector<VfsNode*> getChildren(); 

  /**
   * @brief Get the number of nodes in this directory.
   * 
   * @return uint32_t 
   */
  uint32_t getChildCount() const;

private:
  std::vector<VfsNode*> children;
};

} /* namespace vfs */

#endif /* DIRECTORY_HPP */