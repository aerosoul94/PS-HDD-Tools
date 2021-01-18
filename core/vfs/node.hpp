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

  /// Get whether this is a file or a directory.
  VfsNodeType getType() const;
  /// Get the name of this node. 
  /// This would represent the file or directory's name.
  std::string getName() const;
  /// Get a list of block indexes that contain this node's data.
  /// For FATX, this would represent the nodes's cluster chain.
  /// For UFS2, this would come from di_db and di_ib block lists.
  std::vector<uint64_t> getBlockIndexes() const;

  void setFlags();

  void setName(std::string name);

  void setLastAccessTime();
  void setLastModifiedTime();
  void setCreationTime();

  void setParent(VfsNode* parent);

protected:
  VfsNodeType type;
  std::string name;
  std::vector<uint64_t> blockIndexes;
  VfsNode* parent;
};

} /* namespace vfs */

#endif /* NODE_HPP */