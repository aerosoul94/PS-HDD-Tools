#include "fat_adapter.hpp"

#include <logging/logger.hpp>

#include <vector>

namespace vfs {
namespace adapters {

FatAdapter::FatAdapter(io::data::DataProvider* dataProvider, bool swap)
  : VfsAdapter(dataProvider)
{
  this->needsSwap = swap;
}

VfsDirectory* FatAdapter::mount()
{
  bootSector = new FatBootSector(dataProvider);
  fat = new FatFileAllocationTable(dataProvider, bootSector);

  fileAreaByteOffset = bootSector->getFileAreaByteOffset();
  bytesPerCluster = bootSector->getBytesPerCluster();
  maxDirents = bytesPerCluster / sizeof(dirent);

  return readRootDirectory();
}

void FatAdapter::unmount()
{
}

VfsDirectory* FatAdapter::readRootDirectory()
{
  VfsDirectory* root = new VfsDirectory();

  if (bootSector->getFatType() == FatType::FAT32) {
    std::vector<uint32_t> chain;
    fat->getClusterChain(bootSector->getRootDirFirstCluster(), chain);

    // For FAT32, we read the root directory like a regular file
    std::vector<char> buffer(bytesPerCluster);
    for (auto cluster : chain) {
      auto clusterOffset = clusterToFileAreaByteOffset(cluster);
      dataProvider->seek(clusterOffset);
      dataProvider->read(buffer.data(), bytesPerCluster);

      loadDirectory(buffer, root);
    }
  } 
  else {
    auto rootDirOffset = bootSector->getFatByteOffset() + 
      (bootSector->getBytesPerFat() * bootSector->getNumFats()); 
    auto rootDirSize = bootSector->getRootDirEntryCount() * sizeof(dirent);

    std::vector<char> buffer(rootDirSize);
    dataProvider->seek(rootDirOffset);
    dataProvider->read(buffer.data(), rootDirSize);

    loadDirectory(buffer, root);
  }

  return root;
}

void FatAdapter::loadDirectory(std::vector<char>& buffer, VfsDirectory* root)
{
  // TODO: Move all this to a FatIndexer class.
  std::u16string longFileName;
  auto lfnIndex = 0;
  auto useLfn = false;
  auto curr = buffer.data();
  auto end = buffer.data() + buffer.size();

  while (curr < end) {
    auto* dir = reinterpret_cast<dirent*>(curr);

    // Check if we are at the end of the list
    if (dir->fileName[0] == FAT_DIRENT_NEVER_USED)
      break;

    // Skip deleted entries
    if (dir->fileName[0] == FAT_DIRENT_DELETED) {
      curr += sizeof(dirent);
      continue;
    }

    // Check if it's a long file name entry
    if (dir->attributes == FAT_DIRENT_ATTR_LFN) {
      useLfn = true;

      auto lfn = reinterpret_cast<dirent_lfn*>(dir);

      longFileName.insert(0, reinterpret_cast<char16_t*>(&lfn->name1), 5);
      longFileName.insert(5, reinterpret_cast<char16_t*>(&lfn->name2), 6);
      longFileName.insert(11, reinterpret_cast<char16_t*>(&lfn->name3), 2);
    }
    else {
      VfsNode* node;

      std::string name;
      if (useLfn) {
        // Convert unicode name to ascii
        for (auto i = 0; i < longFileName.length(); i++) {
          auto u = longFileName[i];
          if (u == 0xffff || u == 0x0000)
            break;

          name.push_back(u);
        }

        rDebug(name);

        longFileName.clear();
      }
      else {
        name.resize(0xB);
        name.assign(dir->fileName, dir->fileName + 0xb);
      }

      if (name.compare(".          ") && name.compare("..         ")) {
        if (dir->attributes & FAT_DIRENT_ATTR_DIRECTORY) {
          node = new VfsDirectory();

          std::vector<uint32_t> chain;
          fat->getClusterChain(dir->firstClusterOfFile, chain);

          // TODO: Copy and paste from readRootDirectory(), refactor this.
          std::vector<char> ents(bytesPerCluster);
          for (auto cluster : chain) {
            auto clusterOffset = clusterToFileAreaByteOffset(cluster);
            dataProvider->seek(clusterOffset);
            dataProvider->read(ents.data(), bytesPerCluster);

            loadDirectory(ents, static_cast<VfsDirectory*>(node));
          }
        }
        else {
          VfsFile* file = new VfsFile();

          file->setFileSize(dir->fileSize);

          node = file;
        }

        node->setName(name);
        node->setCreationTime(
          dir->creationTime.date.day,
          dir->creationTime.date.month,
          dir->creationTime.date.year + 1980,
          dir->creationTime.time.doubleSeconds * 2,
          dir->creationTime.time.minute,
          dir->creationTime.time.hour
        );

        node->setLastModifiedTime(
          dir->lastWriteTime.date.day,
          dir->lastWriteTime.date.month,
          dir->lastWriteTime.date.year + 1980,
          dir->lastWriteTime.time.doubleSeconds * 2,
          dir->lastWriteTime.time.minute,
          dir->lastWriteTime.time.hour
        );

        node->setLastAccessTime(
          dir->lastAccessDate.day,
          dir->lastAccessDate.month,
          dir->lastAccessDate.year + 1980
        );

        root->addChild(node);
      }

      useLfn = false;
    }

    curr += sizeof(dirent);
  }
}

uint64_t FatAdapter::clusterToFileAreaByteOffset(uint32_t clusterIndex)
{
  return fileAreaByteOffset + ((clusterIndex - 2) * bytesPerCluster);
}

} /* namespace adapters */
} /* namespace vfs */
