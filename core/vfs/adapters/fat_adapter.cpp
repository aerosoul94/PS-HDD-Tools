#include "fat_adapter.hpp"

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

  // When I start using cluster chains, loadDirectory() will handle seeking
  // by seeking to each cluster.
  auto fileAreaByteOffset = bootSector->getFileAreaByteOffset();
  dataProvider->seek(fileAreaByteOffset);
  VfsDirectory* root = new VfsDirectory();
  loadDirectory(root);

  return root;
}

void FatAdapter::unmount()
{

}

void FatAdapter::loadDirectory(VfsDirectory* root)
{
  // FatLocateDirent:
  // https://github.com/microsoft/Windows-driver-samples/blob/master/filesys/fastfat/dirsup.c#L1247

  std::u16string longFileName;
  auto lfnIndex = 0;
  auto useLfn = false;
  dirent dir;
  while (true) {
    dataProvider->read(reinterpret_cast<char*>(&dir), sizeof(dirent));

    if (dir.fileName[0] == FAT_DIRENT_NEVER_USED) {
      break;
    }

    if (dir.fileName[0] == FAT_DIRENT_DELETED) {
      continue;
    }

    // Check if it's a long file name entry
    if (dir.attributes == FAT_DIRENT_ATTR_LFN) {
      useLfn = true;

      dirent_lfn* lfn = reinterpret_cast<dirent_lfn*>(&dir);

      // copy the name to the beginning
      longFileName.insert(0, reinterpret_cast<char16_t*>(&lfn->name1), 5);
      longFileName.insert(5, reinterpret_cast<char16_t*>(&lfn->name2), 6);
      longFileName.insert(11, reinterpret_cast<char16_t*>(&lfn->name3), 2);

      // Do some sanity checks
      bool isLast = dir.fileName[0] & FAT_LAST_LONG_ENTRY;
      // if (isLast) {
      //   useLfn = false;
      // }
    }
    else {
      VfsNode* node;

      if (dir.attributes & FAT_DIRENT_ATTR_DIRECTORY) {
        VfsDirectory* dir = new VfsDirectory;
        
        // loadDirectory(dir);

        node = dir;
      }
      else {
        VfsFile* file = new VfsFile;
        file->setFileSize(dir.fileSize);

        node = file;
      }

      if (useLfn) {
        // TODO: Switch to unicode names
        std::string convert;
        convert.resize((longFileName.length() + 2) / 2);

        for (auto i = 0; i < convert.length(); i++) {
          if (longFileName[i] == 0xffff)
            break;

          convert[i] = longFileName[i];
        }

        node->setName(convert);

        longFileName.clear();
      }
      else {
        // TODO: Short file names
        node->setName("TODO SFN");
      }

      root->addChild(node);

      useLfn = false;
    }
  }
}

} /* namespace adapters */
} /* namespace vfs */
