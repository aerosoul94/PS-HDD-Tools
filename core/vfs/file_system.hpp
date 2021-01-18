#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include "directory.hpp"
#include "adapters/adapter.hpp"

namespace vfs {

class Vfs
{
public:
  Vfs();

  /**
   * @brief Mount the file system.
   */
  void mount();

  /**
   * @brief Get the root directory for this file system.
   * 
   * @return VfsDirectory* 
   */
  VfsDirectory* getRoot() const;

  /**
   * @brief Set the adapter to use to mount the file system.
   * 
   * @param adapter 
   */
  void setAdapter(VfsAdapter* adapter);

  /**
   * @brief Check if this file system is currently mounted.
   * 
   * @return true 
   * @return false 
   */
  bool isMounted() const;

private:
  VfsDirectory* root;
  VfsAdapter* adapter;
  bool mounted;
};

} /* namespace vfs */

#endif /* FILE_SYSTEM_HPP */