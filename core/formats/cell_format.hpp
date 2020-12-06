#ifndef CELL_FORMAT_HPP
#define CELL_FORMAT_HPP

#include <vector>
#include <stdint.h>

#include "disk_format.hpp"
#include "partition/ps3.h"

#include <io/data/data_provider.hpp>

/// Disk builder for PS3
class CellDiskFormat : public IDiskFormat
{
public:
  CellDiskFormat() {}
  
  bool match(Disk* disk, DiskConfig* config) override;
  void build(Disk* disk, DiskConfig* config) override;

private:
  void generateKeys(std::vector<char>& eidRootKey);

  std::vector<char> ataKeys;
  std::vector<char> encDecKeys;
  const uint32_t kSectorSize = 0x200;
  enum class Ps3Type {UNK,PHAT,SLIM/*,ARC*/} type = Ps3Type::UNK;
};

#endif /* CELL_FORMAT_HPP */
