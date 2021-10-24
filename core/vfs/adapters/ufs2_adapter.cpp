#include "ufs2_adapter.hpp"
#include <logging/logger.hpp>

#include <assert.h>
#include <cstring>

namespace vfs {
namespace adapters {

Ufs2Adapter::Ufs2Adapter(io::data::DataProvider* dataProvider, bool swap)
  : VfsAdapter(dataProvider)
{
  this->needsSwap = swap;
}

VfsDirectory* Ufs2Adapter::mount()
{
  // TODO: Move this to a Ufs2SuperBlock class
  super = new fs;
  dataProvider->seek(SBLOCK_UFS2);
  dataProvider->read(reinterpret_cast<char*>(super), sizeof(*super));

  // Swap endian if needed
  if (needsSwap)
    swapSuperblock(super);

  // Make sure we are actually dealing with UFS2
  if (!checkSuperblock(super))
    return nullptr;

  // Create an index of the file system
  VfsDirectory* root = new VfsDirectory();
  ufs2_dinode inode;
  getInode(ROOTINO, &inode);

  auto fsbo = ino_to_fsbo(super, ROOTINO);  // Returns the inode index within a inode block
  auto offset = (fsbtodb(super, ino_to_fsba(super, ROOTINO)) * 0x200) + fsbo * sizeof(inode);

  root->addOffset("inode", offset);

  loadDirectory(root, &inode);

  return root;
}

void Ufs2Adapter::unmount() {}

void Ufs2Adapter::getInode(uint32_t ino, ufs2_dinode* inode)
{
  // TODO: Move this to a new method
  auto fsbo = ino_to_fsbo(super, ino);  // Returns the inode index within a inode block
  auto offset = (fsbtodb(super, ino_to_fsba(super, ino)) * 0x200) + fsbo * sizeof(*inode);
  dataProvider->seek(offset);
  dataProvider->read(reinterpret_cast<char*>(inode), sizeof(*inode));
  if (needsSwap)
    swapInode(inode);
}

void Ufs2Adapter::loadDirectory(VfsDirectory* root, ufs2_dinode* dinode)
{
  // TODO: Move this to a Ufs2Indexer class.
  std::vector<uint32_t> blockList = getBlockListForInode(dinode);
  auto data = readBlocks(blockList);
  auto pDir = data.data();
  while (pDir < data.data() + dinode->di_size) {
    direct* dir = reinterpret_cast<direct*>(pDir);
    if (needsSwap) {
      swapDirect(dir);
    }

    auto off = pDir - data.data();
    pDir += dir->d_reclen;
    //VfsNode* node = nullptr;
    if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
      continue;
    if (!dir->d_ino)
      break;

    ufs2_dinode inode;
    getInode(dir->d_ino, &inode);

    VfsNode* node;
    if (dir->d_type == DT_DIR) {
      node = new VfsDirectory();

      loadDirectory(static_cast<VfsDirectory*>(node), &inode);
    } else {
      auto file = new VfsFile();
      file->setFileSize(inode.di_size);

      node = file;
    }

    auto blockIndex = off/super->fs_bsize;
    auto addr = ((uint64_t)blockList[blockIndex] * super->fs_fsize) + 
      (off - (blockIndex * super->fs_fsize));

    node->addOffset("dirent", addr);

    auto fsbo = ino_to_fsbo(super, dir->d_ino);  // Returns the inode index within a inode block
    auto offset = (fsbtodb(super, ino_to_fsba(super, dir->d_ino)) * 0x200) + fsbo * sizeof(inode);

    node->addOffset("inode", offset);
    loadDataOffsets(node, &inode);

    node->setName(dir->d_name);
    // node->setAttributes(inode.di_mode);

    node->getCreationTime()
      ->setFromTime(reinterpret_cast<time_t*>(&inode.di_ctime));
    node->getLastAccessTime()
      ->setFromTime(reinterpret_cast<time_t*>(&inode.di_atime));
    node->getLastModifiedTime()
      ->setFromTime(reinterpret_cast<time_t*>(&inode.di_mtime));

    node->setParent(root);

    root->addChild(node);
  }
}

void Ufs2Adapter::loadDataOffsets(VfsNode* node, ufs2_dinode* inode)
{
  for (auto i = 0; i < NDADDR; i++) {
    if (inode->di_db[i] == 0)
      return;
    auto offset = inode->di_db[i] * super->fs_fsize;
    node->addOffset("data", offset);
  }

  if (inode->di_ib[0]) {
    node->addOffset("blocktable", inode->di_ib[0] * super->fs_fsize);
    loadIndirectBlockTable(node, inode->di_ib[0]);
  }

  if (inode->di_ib[1]) {
    // TODO: Maybe add a group for each level
    node->addOffset("blocktable", inode->di_ib[1] * super->fs_fsize);
    std::vector<uint64_t> buffer(super->fs_bsize / sizeof(uint64_t));
    dataProvider->seek(inode->di_ib[1] * super->fs_fsize);
    dataProvider->read(reinterpret_cast<char*>(buffer.data()), super->fs_bsize);
    for (auto x = 0; x < super->fs_nindir; x++) {
      if (buffer[x] == 0)
        return;
      swap(buffer[x]);
      node->addOffset("blocktable", buffer[x] * super->fs_fsize);
      loadIndirectBlockTable(node, buffer[x]);
    }
  }

  if (inode->di_ib[2]) {
    node->addOffset("blocktable", inode->di_ib[2] * super->fs_fsize);
    std::vector<uint64_t> buffer(super->fs_bsize / sizeof(uint64_t));
    dataProvider->seek(inode->di_ib[2] * super->fs_fsize);
    dataProvider->read(reinterpret_cast<char*>(buffer.data()), super->fs_bsize);
    for (auto x = 0; x < super->fs_nindir; x++) {
      if (buffer[x] == 0)
        return;
      swap(buffer[x]);
      node->addOffset("blocktable", buffer[x] * super->fs_fsize);
      std::vector<uint64_t> bufferY(super->fs_bsize / sizeof(uint64_t));
      dataProvider->seek(buffer[x] * super->fs_fsize);
      dataProvider->read(reinterpret_cast<char*>(bufferY.data()), super->fs_bsize);
      for (auto y = 0; y < super->fs_nindir; y++) {
        if (bufferY[y] == 0)
          return;
        swap(bufferY[y]);
        node->addOffset("blocktable", bufferY[y] * super->fs_fsize);
        loadIndirectBlockTable(node, bufferY[y]);
      }
    }
  }
}

void Ufs2Adapter::loadIndirectBlockTable(VfsNode* node, ufs2_daddr_t addr)
{
  std::vector<uint64_t> buffer(super->fs_bsize / sizeof(uint64_t));
  dataProvider->seek(addr * super->fs_fsize);
  dataProvider->read(reinterpret_cast<char*>(buffer.data()), super->fs_bsize);
  for (auto i = 0; i < super->fs_nindir; i++) {
    if (buffer[i] == 0)
      return;
    swap(buffer[i]);
    auto offset = buffer[i] * super->fs_fsize;
    node->addOffset("data", offset);
  }
}

std::vector<uint32_t> Ufs2Adapter::getBlockListForInode(ufs2_dinode* inode)
{
  std::vector<uint32_t> blockList;
  for (auto i = 0; i < NDADDR; i++) {
    if (inode->di_db[i] == 0)
      break;
    blockList.push_back(inode->di_db[i]);
  }
  // TODO: Indirect blocks
  return blockList;
}

std::vector<char> Ufs2Adapter::readBlocks(std::vector<uint32_t> blockList)
{
  std::vector<char> buffer(blockList.size() * super->fs_bsize);
  auto readOffset = 0;
  for (auto blockIndex : blockList) {
    uint64_t offset = (uint64_t)blockIndex * super->fs_fsize;
    dataProvider->seek(offset);
    dataProvider->read(buffer.data() + readOffset, super->fs_bsize);
    readOffset += super->fs_bsize;
  }
  return buffer;
}

bool Ufs2Adapter::checkSuperblock(fs* superblock)
{
  return superblock->fs_magic == FS_UFS2_MAGIC;
}

void Ufs2Adapter::swapInode(ufs2_dinode* inode)
{
  swap(inode->di_mode);
  swap(inode->di_nlink);
  swap(inode->di_uid);
  swap(inode->di_gid);
  swap(inode->di_blksize);
  swap(inode->di_size);
  swap(inode->di_blocks);
  swap(inode->di_atime);
  swap(inode->di_mtime);
  swap(inode->di_ctime);
  swap(inode->di_birthtime);
  swap(inode->di_mtimensec);
  swap(inode->di_atimensec);
  swap(inode->di_birthnsec);
  swap(inode->di_gen);
  swap(inode->di_kernflags);
  swap(inode->di_flags);
  swap(inode->di_extsize);
  for (auto i = 0; i < NXADDR; i++)
    swap(inode->di_extb[i]);
  for (auto i = 0; i < NDADDR; i++)
    swap(inode->di_db[i]);
  for (auto i = 0; i < NIADDR; i++)
    swap(inode->di_ib[i]);
  swap(inode->di_modrev);
  swap(inode->di_freelink);
}

void Ufs2Adapter::swapDirect(direct* d)
{
  swap(d->d_ino);
  swap(d->d_reclen);
}

void Ufs2Adapter::swapSuperblock(fs* superblock)
{
  swap(superblock->fs_firstfield);
  swap(superblock->fs_unused_1);
  swap(superblock->fs_sblkno);
  swap(superblock->fs_cblkno);
  swap(superblock->fs_iblkno);
  swap(superblock->fs_dblkno);
  swap(superblock->fs_old_cgoffset);
  swap(superblock->fs_old_cgmask);
  swap(superblock->fs_old_time);
  swap(superblock->fs_old_size);
  swap(superblock->fs_old_dsize);
  swap(superblock->fs_ncg);
  swap(superblock->fs_bsize);
  swap(superblock->fs_fsize);
  swap(superblock->fs_frag);
  swap(superblock->fs_minfree);
  swap(superblock->fs_old_rotdelay);
  swap(superblock->fs_old_rps);
  swap(superblock->fs_bmask);
  swap(superblock->fs_fmask);
  swap(superblock->fs_bshift);
  swap(superblock->fs_fshift);
  swap(superblock->fs_maxcontig);
  swap(superblock->fs_maxbpg);
  swap(superblock->fs_fragshift);
  swap(superblock->fs_fsbtodb);
  swap(superblock->fs_sbsize);
  swap(superblock->fs_spare1[0]);
  swap(superblock->fs_spare1[1]);
  swap(superblock->fs_nindir);
  swap(superblock->fs_inopb);
  swap(superblock->fs_old_nspf);
  swap(superblock->fs_optim);
  swap(superblock->fs_old_npsect);
  swap(superblock->fs_old_interleave);
  swap(superblock->fs_old_trackskew);
  swap(superblock->fs_id[0]);
  swap(superblock->fs_id[1]);
  swap(superblock->fs_old_csaddr);
  swap(superblock->fs_cssize);
  swap(superblock->fs_cgsize);
  swap(superblock->fs_spare2);
  swap(superblock->fs_old_nsect);
  swap(superblock->fs_old_spc);
  swap(superblock->fs_old_ncyl);
  swap(superblock->fs_old_cpg);
  swap(superblock->fs_ipg);
  swap(superblock->fs_fpg);
  swap(superblock->fs_swuid);
  //swap(superblock->fs_pad);
  swap(superblock->fs_cgrotor);
  swap(superblock->fs_old_cpc);
  swap(superblock->fs_maxbsize);
  swap(superblock->fs_unrefs);
  swap(superblock->fs_providersize);
  swap(superblock->fs_metaspace);
  swap(superblock->fs_sparecon64[0]);
  swap(superblock->fs_sparecon64[1]);
  swap(superblock->fs_sparecon64[2]);
  swap(superblock->fs_sparecon64[3]);
  swap(superblock->fs_sparecon64[4]);
  swap(superblock->fs_sparecon64[5]);
  swap(superblock->fs_sparecon64[6]);
  swap(superblock->fs_sparecon64[7]);
  swap(superblock->fs_sparecon64[8]);
  swap(superblock->fs_sparecon64[9]);
  swap(superblock->fs_sparecon64[10]);
  swap(superblock->fs_sparecon64[11]);
  swap(superblock->fs_sparecon64[12]);
  swap(superblock->fs_sparecon64[13]);
  swap(superblock->fs_sblockloc);
  swap(superblock->fs_time);
  swap(superblock->fs_size);
  swap(superblock->fs_dsize);
  swap(superblock->fs_csaddr);
  swap(superblock->fs_pendingblocks);
  swap(superblock->fs_pendinginodes);
  swap(superblock->fs_snapinum[0]);
  swap(superblock->fs_snapinum[1]);
  swap(superblock->fs_snapinum[2]);
  swap(superblock->fs_snapinum[3]);
  swap(superblock->fs_snapinum[4]);
  swap(superblock->fs_snapinum[5]);
  swap(superblock->fs_snapinum[6]);
  swap(superblock->fs_snapinum[7]);
  swap(superblock->fs_snapinum[8]);
  swap(superblock->fs_snapinum[9]);
  swap(superblock->fs_snapinum[10]);
  swap(superblock->fs_snapinum[11]);
  swap(superblock->fs_snapinum[12]);
  swap(superblock->fs_snapinum[13]);
  swap(superblock->fs_snapinum[14]);
  swap(superblock->fs_snapinum[15]);
  swap(superblock->fs_snapinum[16]);
  swap(superblock->fs_snapinum[17]);
  swap(superblock->fs_snapinum[18]);
  swap(superblock->fs_snapinum[19]);
  swap(superblock->fs_avgfilesize);
  swap(superblock->fs_avgfpdir);
  swap(superblock->fs_save_cgsize);
  swap(superblock->fs_mtime);
  swap(superblock->fs_sujfree);
  swap(superblock->fs_sparecon32[0]);
  swap(superblock->fs_sparecon32[1]);
  swap(superblock->fs_sparecon32[2]);
  swap(superblock->fs_sparecon32[3]);
  swap(superblock->fs_sparecon32[4]);
  swap(superblock->fs_sparecon32[5]);
  swap(superblock->fs_sparecon32[6]);
  swap(superblock->fs_sparecon32[7]);
  swap(superblock->fs_sparecon32[8]);
  swap(superblock->fs_sparecon32[9]);
  swap(superblock->fs_sparecon32[10]);
  swap(superblock->fs_sparecon32[11]);
  swap(superblock->fs_sparecon32[12]);
  swap(superblock->fs_sparecon32[13]);
  swap(superblock->fs_sparecon32[14]);
  swap(superblock->fs_sparecon32[15]);
  swap(superblock->fs_sparecon32[16]);
  swap(superblock->fs_sparecon32[17]);
  swap(superblock->fs_sparecon32[18]);
  swap(superblock->fs_sparecon32[19]);
  swap(superblock->fs_sparecon32[20]);
  swap(superblock->fs_sparecon32[21]);
  swap(superblock->fs_sparecon32[22]);
  swap(superblock->fs_flags);
  swap(superblock->fs_contigsumsize);
  swap(superblock->fs_maxsymlinklen);
  swap(superblock->fs_old_inodefmt);
  swap(superblock->fs_maxfilesize);
  swap(superblock->fs_qbmask);
  swap(superblock->fs_qfmask);
  swap(superblock->fs_state);
  swap(superblock->fs_old_postblformat);
  swap(superblock->fs_old_nrpos);
  swap(superblock->fs_spare5[0]);
  swap(superblock->fs_spare5[1]);
  swap(superblock->fs_magic);
}

} /* namespace adapters */
} /* namespace vfs */
