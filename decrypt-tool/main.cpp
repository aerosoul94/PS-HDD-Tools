#include <disk/disk.hpp>
#include <disk/disk_config.hpp>
#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>
#include <io/stream/file_disk_stream.hpp>
#include <logging/logger.hpp>
#include <logging/stdout_log_handler.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

void writePartitionToFile(disk::Partition* partition, std::ofstream& file)
{
  const uint32_t kBufSize = 0x200 * 0x800;
  std::vector<char> buf(kBufSize);

  auto length = partition->getLength();
  auto dataProvider = partition->getDataProvider();

  // Make sure we're at the start of the partition
  dataProvider->seek(0);
  for (uint64_t sector = 0; sector < length; sector += kBufSize) {
    auto readLen = dataProvider->read(buf.data(), kBufSize);
    file.write(buf.data(), readLen);
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
  std::cout << "Usage: " << argv[0] << " [command] [arguments...]" << std::endl
            << std::endl
            << "Commands:" << std::endl
            << "  list    <input> <key file>                       List available partitions." << std::endl
            << "  decrypt <input> <key file> <partition> <output>  Decrypt a partition to a new file." << std::endl
            << std::endl;
}

int main(int argc, char** argv)
{
  if (argc < 4) {
    printUsage(argc, argv);
    return 1;
  }

  auto handler = new logging::StdOutLogHandler();
#ifndef NDEBUG
  handler->setLogLevel(logging::LogLevel::Debug);
#endif
  rAddHandler("stdout", handler);

  std::string command(argv[1]);
  
  std::ifstream imageFile;
  imageFile.open(argv[2], std::ios::binary);
  if (!imageFile.is_open()) {
    rError(std::string("Failed to open image file: ") + std::string(argv[2]));
    return 1;
  }

  std::ifstream keyFile;
  keyFile.open(argv[3], std::ios::binary);
  if (!keyFile.is_open()) {
    rError(std::string("Failed to open key file: ") + std::string(argv[3]));
    return 1;
  }

  disk::DiskConfig config;
  buildConfig(&config, imageFile, keyFile);

  disk::Disk* disk = formats::DiskFormatFactory::getInstance()->detectFormat(&config);
  if (!disk) {
    rError( "Could not detect disk format");
    return 1;
  }

  if (!disk->getPartitions().size()) {
    rError("Could not find any partitions in this disk");
    return 1;
  }

  if (command == "list") {
    listPartitions(disk);
  }
  else if (command == "decrypt") {
    std::string partitionName(argv[4]);
    auto partition = disk->getPartitionByName(partitionName);
    if (partition == nullptr) {
      rError("No partition named: " + partitionName);
      return 1;
    }

    std::ofstream outputFile;
    outputFile.open(argv[5], std::ios::binary);

    writePartitionToFile(partition, outputFile);
  }

  return 0;
}
