#ifndef ORBIS_FORMAT_HPP
#define ORBIS_FORMAT_HPP

#include "disk_format.hpp"
#include "partition/gpt.h"

/// Disk builder for PS4
class OrbisDiskFormat : public IDiskFormat
{
public:
  OrbisDiskFormat() {}

  bool match(Disk*, DiskConfig*) override;
  void build(Disk*, DiskConfig*) override;

private:
  bool verifyGptHeader();
  void loadGptEntTable(DataProvider* dataProvider);
  const gpt_ent* getGptEntByType(const uuid* type);
  
  gpt_hdr hdr;
  std::vector<gpt_ent> partitions;
  const uuid user_ent = GPT_ENT_TYPE_ORBIS_USER;
  const uint32_t kSectorSize = 0x200;
};

#endif /* ORBIS_FORMAT_HPP */
