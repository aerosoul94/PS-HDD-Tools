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
#include <iomanip>
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

void listPartitions(disk::Disk* disk)
{
  std::cout << std::setw(26) << std::left << "Partition Name"
            << std::setw(16) << std::left << "Start"
            << std::setw(16) << std::left << "End"
            << std::setw(16) << std::left << "Length"
            << std::endl;

  for (auto partition : disk->getPartitions()) {
    std::cout << std::setw(26) << std::left << partition->getName()
              << std::setw(16) << std::left << std::hex << partition->getStart()
              << std::setw(16) << std::left << std::hex << partition->getEnd()
              << std::setw(16) << std::left << std::hex << partition->getLength()
              << std::endl;
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

void printUsage(int argc, char** argv)
{
  std::cout << "Usage: " << argv[0] << "[command] [arguments...]" << std::endl
            << std::endl
            << "Commands:" << std::endl
            << "  list    <input> <key file>              List available partitions." << std::endl
            << "  display <input> <key file> <partition>  Display file system contents." << std::endl
            << std::endl;
}

int main(int argc, char** argv)
{
  if (argc < 4) {
    printUsage(argc, argv);
    return 1;
  }

  auto logger = new logging::StdOutLogHandler;
#ifndef NDEBUG
  logger->setLogLevel(logging::LogLevel::Debug);
#endif
  rAddHandler("stdout", logger);

  std::string command(argv[1]);

  std::ifstream imageFile;
  imageFile.open(argv[2], std::ios::binary);
  if (!imageFile.is_open()) {
    rError("Failed to open image file: " + std::string(argv[2]));
    return 1;
  }

  std::ifstream keyFile;
  keyFile.open(argv[3], std::ios::binary);
  if (!keyFile.is_open()) {
    rError("Failed to open key file: " + std::string(argv[3]));
    return 1;
  }

  disk::DiskConfig config;
  buildConfig(&config, imageFile, keyFile);

  disk::Disk* disk = formats::DiskFormatFactory::getInstance()->detectFormat(&config);
  if (!disk) {
    rError("Could not detect disk format");
    return 1;
  }

  if (!disk->getPartitions().size()) {
    rError("Could not find any partitions in this disk");
    return 1;
  }

  if (command == "list") {
    listPartitions(disk);
  }
  else if (command == "display") {
    std::string partitionName(argv[4]);

    auto partition = disk->getPartitionByName(partitionName);
    if (partition == nullptr) {
      rError("No partition named: " + partitionName);
      return 1;
    }

    partition->mount();
    auto vfs = partition->getVfs();

    if (!vfs->isMounted()) {
      rError("Could not mount the file system.");
      return 1;
    }

    auto root = vfs->getRoot();
    printDirectory(root, 0);
  }

  return 0;
}