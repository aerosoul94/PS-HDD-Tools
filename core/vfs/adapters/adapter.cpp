#include "adapter.hpp"

namespace vfs {

VfsAdapter::VfsAdapter(io::data::DataProvider* dataProvider)
{
  this->dataProvider = dataProvider;
}

} /* namespace vfs */
