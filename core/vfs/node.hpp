#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

namespace vfs {

enum class VfsNodeType
{
  DIRECTORY,
  FILE
};

class VfsNode
{
public:
  VfsNode(VfsNodeType type);

  /**
   * @brief Get the type of node this is. See VfsNodeType.
   * 
   * @return VfsNodeType 
   */
  VfsNodeType getType() const;
  
  /**
   * @brief Get the name of this node.
   * 
   * This should represent the file or directory name.
   * 
   * @return std::string 
   */
  std::string getName() const;
  
  /**
   * @brief Get a list of block indexes that form this node's data.
   * 
   * @return std::vector<uint64_t> 
   */
  std::vector<uint64_t> getBlockIndexes() const;

  /**
   * @brief Set this node's name.
   * 
   * This should be set to the file or directory name.
   * 
   * @param name 
   */
  void setName(std::string name);

  /**
   * @brief Set the last access time for this node.
   */
  void setLastAccessTime();

  /**
   * @brief Set the last modified time for this node.
   */
  void setLastModifiedTime();

  /**
   * @brief Set the creation time for this node.
   */
  void setCreationTime();

  /**
   * @brief Set this node's parent node.
   * 
   * @param parent The parent node.
   */
  void setParent(VfsNode* parent);

protected:
  VfsNodeType type;
  std::string name;
  std::vector<uint64_t> blockIndexes;
  VfsNode* parent;
};

} /* namespace vfs */

#endif /* NODE_HPP */