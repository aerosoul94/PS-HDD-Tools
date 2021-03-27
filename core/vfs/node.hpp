#ifndef NODE_HPP
#define NODE_HPP

#include "date_time.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <ctime>

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
  virtual ~VfsNode() = default;

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

  VfsDateTime* getLastAccessTime();

  VfsDateTime* getLastModifiedTime();

  VfsDateTime* getCreationTime();

  /**
   * @brief Set this node's name.
   * 
   * This should be set to the file or directory name.
   * 
   * @param name 
   */
  void setName(std::string name);

  /**
   * @brief Set this node's parent node.
   * 
   * @param parent The parent node.
   */
  void setParent(VfsNode* parent);

  /**
   * @brief Set the last access time for this node.
   */
  void setLastAccessTime(int day = 0, int month = 0, int year = 0, 
                         int second = 0, int minute = 0, int hour = 0);

  /**
   * @brief Set the last modified time for this node.
   */
  void setLastModifiedTime(int day = 0, int month = 0, int year = 0,
                           int second = 0, int minute = 0, int hour = 0);

  /**
   * @brief Set the creation time for this node.
   */
  void setCreationTime(int day = 0, int month = 0, int year = 0,
                       int second = 0, int minute = 0, int hour = 0);

  /**
   * @brief Add an offset associated with this file.
   * 
   * @param key The name or group this offset belongs to.
   * @param value The offset.
   */
  void addOffset(std::string key, uint64_t value);

  /**
   * @brief Get offsets by group name.
   * 
   * @param key Group name.
   * @return std::vector<uint64_t> Offsets associated with this file.
   */
  std::vector<uint64_t> getOffsets(std::string& key);

protected:
  VfsNodeType type;
  std::string name;
  std::vector<uint64_t> blockIndexes;
  VfsNode* parent;
  // TODO: Eventually these will be more dynamic. We can move these to 
  //   some sort of std::map<string, VfsDateTime>.
  VfsDateTime creationTime, 
              lastModifiedTime, 
              lastAccessTime;
  std::unordered_multimap<std::string, uint64_t> offsets;
};

} /* namespace vfs */

#endif /* NODE_HPP */