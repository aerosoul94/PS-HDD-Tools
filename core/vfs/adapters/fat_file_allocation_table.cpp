#include "fat_file_allocation_table.hpp"

namespace vfs {
namespace adapters {

FatFileAllocationTable::FatFileAllocationTable(
    io::data::DataProvider* dataProvider, FatBootSector* bootSector)
{
  this->dataProvider = dataProvider;
  this->bootSector = bootSector;
  this->readFileAllocationTable();
}

void FatFileAllocationTable::readFileAllocationTable()
{
  auto fatByteOffset = bootSector->getFatByteOffset();
  auto bytesPerFat = bootSector->getBytesPerFat();
  dataProvider->seek(fatByteOffset);

  fileAllocationTableBuffer.resize(bytesPerFat);
  dataProvider->read(reinterpret_cast<char*>(fileAllocationTableBuffer.data()), 
    bytesPerFat);
  
  // backupFileAllocationTableBuffer.resize(bytesPerFat);
  // dataProvider->read(reinterpret_cast<char*>(backupFileAllocationTableBuffer.data()),
  //   bytesPerFat);
}

void FatFileAllocationTable::getClusterChain(uint32_t firstCluster, 
    std::vector<uint32_t>& chain)
{
  chain.push_back(firstCluster);

  auto nextCluster = getFatEntry(firstCluster);
  while (nextCluster <= FAT_CLUSTER_RESERVED) {
    chain.push_back(nextCluster);
    nextCluster = getFatEntry(nextCluster);
  }
}

uint32_t FatFileAllocationTable::getFatEntry(uint32_t clusterIndex)
{
  switch (bootSector->getFatType()) {
    case FatType::FAT12:
      return this->getFat12Entry(clusterIndex);
    case FatType::FAT16:
      return this->getFat16Entry(clusterIndex);
    case FatType::FAT32:
      return this->getFat32Entry(clusterIndex);
  }
}

uint32_t FatFileAllocationTable::getFat12Entry(uint32_t clusterIndex)
{
  return FAT_CLUSTER_LAST;
}

uint32_t FatFileAllocationTable::getFat16Entry(uint32_t clusterIndex)
{
  uint32_t entry = reinterpret_cast<uint16_t*>(fileAllocationTableBuffer.data())[clusterIndex];
  if (entry >= 0x0FFF0) {
    entry |= 0x0FFF0000;
  }
  return entry;
}

uint32_t FatFileAllocationTable::getFat32Entry(uint32_t clusterIndex)
{
  return reinterpret_cast<uint32_t*>(fileAllocationTableBuffer.data())[clusterIndex];
}

} /* namespace adapters */
} /* namespace vfs */
