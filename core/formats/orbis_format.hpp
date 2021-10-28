#ifndef ORBIS_FORMAT_HPP
#define ORBIS_FORMAT_HPP

#include "disk_format.hpp"
#include "partition/gpt.h"

namespace formats {

/**
 * @brief Detects and loads the PS4 disk format.
 */
class OrbisDiskFormat : public IDiskFormat
{
public:
  OrbisDiskFormat() {}

  bool match(disk::Disk*, disk::DiskConfig*) override;
  void build(disk::Disk*, disk::DiskConfig*) override;

private:
  // TODO: Move these to a GPT Disk Format class.
  /**
   * @brief Verify that this is a GPT header.
   * 
   * @return true 
   * @return false 
   */
  bool verifyGptHeader();

  /**
   * @brief Load the GPT Entry Table from the disk.
   * 
   * @param dataProvider 
   */
  void loadGptEntTable(io::data::DataProvider* dataProvider);

  /**
   * @brief Get the a GPT Entry by its type from the GPT Ent Table.
   * 
   * @param type 
   * @return const gpt_ent* 
   */
  const gpt_ent* getGptEntByType(const uuid* type);

  void addFatPartition(disk::Disk* disk, crypto::CryptoStrategy* strategy, std::string name, const gpt_ent* ent);
  void addUfsPartition(disk::Disk* disk, crypto::CryptoStrategy* strategy, std::string name, const gpt_ent* ent);
  
  gpt_hdr hdr;
  std::vector<gpt_ent> partitions;
  const uuid user_ent = GPT_ENT_TYPE_ORBIS_USER;
  const uuid eap_user_ent = GPT_ENT_TYPE_ORBIS_EAP_USER;
  const uuid eap_vsh_ent = GPT_ENT_TYPE_ORBIS_EAP_VSH;
  const uuid update_ent = GPT_ENT_TYPE_ORBIS_UPDATE;
  const uint32_t kSectorSize = 0x200;
};

} /* namespace formats */

#endif /* ORBIS_FORMAT_HPP */
