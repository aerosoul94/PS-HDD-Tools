#include <formats/disk_format_factory.hpp>
#include <disk/disk_config.hpp>
#include <disk/disk.hpp>
#include <disk/partition.hpp>
#include <io/stream/file_disk_stream.hpp>

#include <iostream>
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
  if (argc != 4) {
    std::cout << "Usage: " 
              << argv[0] 
              << " <input image> <key file> <output file>"
              << std::endl;
    return 0;
  }

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

  std::ofstream outputFile;
  outputFile.open(argv[3], std::ios::binary);
  writePartitionToFile(disk->getPartitions()[0], outputFile);

  return 0;
}