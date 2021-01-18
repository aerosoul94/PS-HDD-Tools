#include "file_system.hpp"

namespace vfs {

Vfs::Vfs()
{
  this->root = nullptr;
  this->adapter = nullptr;
  this->mounted = false;
}

void Vfs::setAdapter(VfsAdapter* adapter)
{
  this->adapter = adapter;
}

void Vfs::mount()
{
  this->root = this->adapter->mount();
  if (this->root)
    this->mounted = true;
}

VfsDirectory* Vfs::getRoot() const
{
  return this->root;
}

bool Vfs::isMounted() const
{
  return this->mounted;
}

} /* namespace vfs */
