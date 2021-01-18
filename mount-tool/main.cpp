#include <disk/disk.hpp>
#include <disk/disk_config.hpp>
#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>
#include <io/stream/file_disk_stream.hpp>
#include <logging/logger.hpp>
#include <logging/stdout_log_handler.hpp>
#include <vfs/directory.hpp>
#include <vfs/file.hpp>
#include <vfs/node.hpp>

#include <iostream>
#include <fstream>
#include <vector>

void printDirectory(vfs::VfsDirectory* dir, int level)
{
  for (auto child : dir->getChildren()) {
    for (auto i = 0; i < level; i++) 
      std::cout << "  ";
    std::cout << child->getName().c_str() << std::endl;
    if (child->getType() == vfs::VfsNodeType::DIRECTORY) {
      auto childDir = static_cast<vfs::VfsDirectory*>(child);
      printDirectory(childDir, level+1);
    }
  }
}

void buildConfig(disk::DiskConfig* config, std::ifstream& imageFile, std::ifstream& keyFile)
{
  keyFile.seekg(0, std::ios::end);
  auto keyLen = keyFile.tellg();
  keyFile.seekg(0, std::ios::beg);

  std::vector<char> keyData(keyLen);
  keyFile.read(keyData.data(), keyLen);

  config->setKeys(keyData.data(), keyLen);
  config->setStream(new io::stream::FileDiskStream(imageFile));
}

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cout << "Usage: "
              << argv[0]
              << " <input image> <key file>"
              << std::endl;
    return 0;
  }

  auto logger = new logging::StdOutLogHandler;
#ifndef NDEBUG
  logger->setLogLevel(logging::LogLevel::Debug);
#endif
  rAddHandler("stdout", logger);

  std::ifstream imageFile;
  imageFile.open(argv[1], std::ios::binary);
  if (!imageFile.is_open()) {
    std::cout << "Failed to open image file: " << argv[1] << std::endl;
    return 0;
  }

  std::ifstream keyFile;
  keyFile.open(argv[2], std::ios::binary);
  if (!keyFile.is_open()) {
    std::cout << "Failed to open key file: " << argv[2] << std::endl;
    return 0;
  }

  disk::DiskConfig config;
  buildConfig(&config, imageFile, keyFile);

  disk::Disk* disk = formats::DiskFormatFactory::getInstance()->detectFormat(&config);
  if (!disk) {
    std::cout << "Could not detect disk format" << std::endl;
    return 0;
  }

  if (!disk->getPartitions().size()) {
    std::cout << "Could not find any partitions in this disk" << std::endl;
    return 0;
  }

  auto partition = disk->getPartitions().front();
  partition->mount();
  auto vfs = partition->getVfs();

  if (!vfs->isMounted()) {
    rError("Could not mount the file system.");
    return 0;
  }

  auto root = vfs->getRoot();
  printDirectory(root, 0);

  rInfo("Done!");
}