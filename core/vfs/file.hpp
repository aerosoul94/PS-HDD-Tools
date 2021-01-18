#ifndef FILE_HPP
#define FILE_HPP

#include "node.hpp"

namespace vfs {

class VfsFile : public VfsNode
{
public:
  VfsFile();

  //VfsFileStream* getStream();

  /**
   * @brief Set this file's size.
   * 
   * @param size The file size.
   */
  void setFileSize(uint64_t size);

  /**
   * @brief Get this file's size.
   * 
   * @return uint64_t The file size.
   */
  uint64_t getFileSize() const;

private:
  uint64_t fileSize;
  // File handle attributes
  // uint64_t offset;
  // uint32_t id;
  // uint32_t flags;
};

} /* namespace vfs */

#endif /* FILE_HPP */
