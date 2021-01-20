#include "ufs2_adapter.hpp"
#include <utilities/endian.hpp>
#include <logging/logger.hpp>

#include <assert.h>

namespace vfs {
namespace adapters {

Ufs2Adapter::Ufs2Adapter(io::data::DataProvider* dataProvider, bool swap)
  : VfsAdapter(dataProvider)
{
  this->needsSwap = swap;
}

VfsDirectory* Ufs2Adapter::mount()
{
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

void Ufs2Adapter::loadDirectory(VfsDirectory* root, ufs2_dinode* inode)
{
  std::vector<uint32_t> blockList = getBlockListForInode(inode);
  auto data = readBlocks(blockList);
  auto pDir = data.data();
  while (pDir <= data.data() + inode->di_size) {
    direct* dir = reinterpret_cast<direct*>(pDir);
    if (needsSwap) {
      swapDirect(dir);
    }

    pDir += dir->d_reclen;
    //VfsNode* node = nullptr;
    if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
      continue;
    if (!dir->d_ino)
      break;

    rDebug(dir->d_name);
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

    node->setName(dir->d_name);
    // node->setAttributes(inode.di_mode);
    // node->setLastAccessTime(inode.di_atime);
    // node->setLastModifiedTime(inode.d_mtime);
    // node->setCreationTime(inode.di_birthnsec);
    node->setParent(root);

    root->addChild(node);
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

#define _swap16(x) x = swap16(x)
#define _swap32(x) x = swap32(x)
#define _swap64(x) x = swap64(x)

void Ufs2Adapter::swapInode(ufs2_dinode* inode)
{
  _swap16(inode->di_mode);
  _swap16(inode->di_nlink);
  _swap32(inode->di_uid);
  _swap32(inode->di_gid);
  _swap32(inode->di_blksize);
  _swap64(inode->di_size);
  _swap64(inode->di_blocks);
  _swap32(inode->di_atime);
  _swap32(inode->di_mtime);
  _swap32(inode->di_ctime);
  _swap32(inode->di_birthtime);
  _swap32(inode->di_mtimensec);
  _swap32(inode->di_atimensec);
  _swap32(inode->di_birthnsec);
  _swap32(inode->di_gen);
  _swap32(inode->di_kernflags);
  _swap32(inode->di_flags);
  _swap32(inode->di_extsize);
  for (auto i = 0; i < NXADDR; i++)
    _swap64(inode->di_extb[i]);
  for (auto i = 0; i < NDADDR; i++)
    _swap64(inode->di_db[i]);
  for (auto i = 0; i < NIADDR; i++)
    _swap64(inode->di_ib[i]);
  _swap64(inode->di_modrev);
  _swap32(inode->di_freelink);
}

void Ufs2Adapter::swapDirect(direct* d)
{
  _swap32(d->d_ino);
  _swap16(d->d_reclen);
}

void Ufs2Adapter::swapSuperblock(fs* superblock)
{
  _swap32(superblock->fs_firstfield);
  _swap32(superblock->fs_unused_1);
  _swap32(superblock->fs_sblkno);
  _swap32(superblock->fs_cblkno);
  _swap32(superblock->fs_iblkno);
  _swap32(superblock->fs_dblkno);
  _swap32(superblock->fs_old_cgoffset);
  _swap32(superblock->fs_old_cgmask);
  _swap32(superblock->fs_old_time);
  _swap32(superblock->fs_old_size);
  _swap32(superblock->fs_old_dsize);
  _swap32(superblock->fs_ncg);
  _swap32(superblock->fs_bsize);
  _swap32(superblock->fs_fsize);
  _swap32(superblock->fs_frag);
  _swap32(superblock->fs_minfree);
  _swap32(superblock->fs_old_rotdelay);
  _swap32(superblock->fs_old_rps);
  _swap32(superblock->fs_bmask);
  _swap32(superblock->fs_fmask);
  _swap32(superblock->fs_bshift);
  _swap32(superblock->fs_fshift);
  _swap32(superblock->fs_maxcontig);
  _swap32(superblock->fs_maxbpg);
  _swap32(superblock->fs_fragshift);
  _swap32(superblock->fs_fsbtodb);
  _swap32(superblock->fs_sbsize);
  _swap32(superblock->fs_spare1[0]);
  _swap32(superblock->fs_spare1[1]);
  _swap32(superblock->fs_nindir);
  _swap32(superblock->fs_inopb);
  _swap32(superblock->fs_old_nspf);
  _swap32(superblock->fs_optim);
  _swap32(superblock->fs_old_npsect);
  _swap32(superblock->fs_old_interleave);
  _swap32(superblock->fs_old_trackskew);
  _swap32(superblock->fs_id[0]);
  _swap32(superblock->fs_id[1]);
  _swap32(superblock->fs_old_csaddr);
  _swap32(superblock->fs_cssize);
  _swap32(superblock->fs_cgsize);
  _swap32(superblock->fs_spare2);
  _swap32(superblock->fs_old_nsect);
  _swap32(superblock->fs_old_spc);
  _swap32(superblock->fs_old_ncyl);
  _swap32(superblock->fs_old_cpg);
  _swap32(superblock->fs_ipg);
  _swap32(superblock->fs_fpg);
  _swap64(superblock->fs_swuid);
  //_swap32(superblock->fs_pad);
  _swap32(superblock->fs_cgrotor);
  _swap32(superblock->fs_old_cpc);
  _swap32(superblock->fs_maxbsize);
  _swap64(superblock->fs_unrefs);
  _swap64(superblock->fs_providersize);
  _swap64(superblock->fs_metaspace);
  _swap64(superblock->fs_sparecon64[0]);
  _swap64(superblock->fs_sparecon64[1]);
  _swap64(superblock->fs_sparecon64[2]);
  _swap64(superblock->fs_sparecon64[3]);
  _swap64(superblock->fs_sparecon64[4]);
  _swap64(superblock->fs_sparecon64[5]);
  _swap64(superblock->fs_sparecon64[6]);
  _swap64(superblock->fs_sparecon64[7]);
  _swap64(superblock->fs_sparecon64[8]);
  _swap64(superblock->fs_sparecon64[9]);
  _swap64(superblock->fs_sparecon64[10]);
  _swap64(superblock->fs_sparecon64[11]);
  _swap64(superblock->fs_sparecon64[12]);
  _swap64(superblock->fs_sparecon64[13]);
  _swap64(superblock->fs_sblockloc);
  _swap32(superblock->fs_time);
  _swap64(superblock->fs_size);
  _swap64(superblock->fs_dsize);
  _swap32(superblock->fs_csaddr);
  _swap64(superblock->fs_pendingblocks);
  _swap32(superblock->fs_pendinginodes);
  _swap32(superblock->fs_snapinum[0]);
  _swap32(superblock->fs_snapinum[1]);
  _swap32(superblock->fs_snapinum[2]);
  _swap32(superblock->fs_snapinum[3]);
  _swap32(superblock->fs_snapinum[4]);
  _swap32(superblock->fs_snapinum[5]);
  _swap32(superblock->fs_snapinum[6]);
  _swap32(superblock->fs_snapinum[7]);
  _swap32(superblock->fs_snapinum[8]);
  _swap32(superblock->fs_snapinum[9]);
  _swap32(superblock->fs_snapinum[10]);
  _swap32(superblock->fs_snapinum[11]);
  _swap32(superblock->fs_snapinum[12]);
  _swap32(superblock->fs_snapinum[13]);
  _swap32(superblock->fs_snapinum[14]);
  _swap32(superblock->fs_snapinum[15]);
  _swap32(superblock->fs_snapinum[16]);
  _swap32(superblock->fs_snapinum[17]);
  _swap32(superblock->fs_snapinum[18]);
  _swap32(superblock->fs_snapinum[19]);
  _swap32(superblock->fs_avgfilesize);
  _swap32(superblock->fs_avgfpdir);
  _swap32(superblock->fs_save_cgsize);
  _swap32(superblock->fs_mtime);
  _swap32(superblock->fs_sujfree);
  _swap32(superblock->fs_sparecon32[0]);
  _swap32(superblock->fs_sparecon32[1]);
  _swap32(superblock->fs_sparecon32[2]);
  _swap32(superblock->fs_sparecon32[3]);
  _swap32(superblock->fs_sparecon32[4]);
  _swap32(superblock->fs_sparecon32[5]);
  _swap32(superblock->fs_sparecon32[6]);
  _swap32(superblock->fs_sparecon32[7]);
  _swap32(superblock->fs_sparecon32[8]);
  _swap32(superblock->fs_sparecon32[9]);
  _swap32(superblock->fs_sparecon32[10]);
  _swap32(superblock->fs_sparecon32[11]);
  _swap32(superblock->fs_sparecon32[12]);
  _swap32(superblock->fs_sparecon32[13]);
  _swap32(superblock->fs_sparecon32[14]);
  _swap32(superblock->fs_sparecon32[15]);
  _swap32(superblock->fs_sparecon32[16]);
  _swap32(superblock->fs_sparecon32[17]);
  _swap32(superblock->fs_sparecon32[18]);
  _swap32(superblock->fs_sparecon32[19]);
  _swap32(superblock->fs_sparecon32[20]);
  _swap32(superblock->fs_sparecon32[21]);
  _swap32(superblock->fs_sparecon32[22]);
  _swap32(superblock->fs_flags);
  _swap32(superblock->fs_contigsumsize);
  _swap32(superblock->fs_maxsymlinklen);
  _swap32(superblock->fs_old_inodefmt);
  _swap64(superblock->fs_maxfilesize);
  _swap64(superblock->fs_qbmask);
  _swap64(superblock->fs_qfmask);
  _swap32(superblock->fs_state);
  _swap32(superblock->fs_old_postblformat);
  _swap32(superblock->fs_old_nrpos);
  _swap32(superblock->fs_spare5[0]);
  _swap32(superblock->fs_spare5[1]);
  _swap32(superblock->fs_magic);
}

} /* namespace adapters */
} /* namespace vfs */
