#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include "directory.hpp"
#include "adapters/adapter.hpp"

namespace vfs {

class Vfs
{
public:
  Vfs();

  void mount();
  VfsDirectory* getRoot() const;
  void setAdapter(VfsAdapter* adapter);
  bool isMounted() const;

private:
  VfsDirectory* root;
  VfsAdapter* adapter;
  bool mounted;
};

} /* namespace vfs */

#endif /* FILE_SYSTEM_HPP */