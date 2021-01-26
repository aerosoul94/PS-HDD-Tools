#include "fat_boot_sector.hpp"

#include "fat/fat_.h"

namespace vfs {
namespace adapters {

FatBootSector::FatBootSector(io::data::DataProvider* dataProvider)
{
  this->dataProvider = dataProvider;

  this->readBootSector(0);
}

void FatBootSector::readBootSector(uint64_t offset)
{
  dataProvider->seek(offset + 0xb);

  bytesPerSector = readUInt16();
  sectorsPerCluster = readUInt8();
  reservedSectors = readUInt16();
  numFats = readUInt8();
  rootEntryCount = readUInt16();
  totalSectors = readUInt16();
  media = readUInt8();
  sectorsPerFat = readUInt16();
  sectorsPerTrack = readUInt16();
  numHeads = readUInt16();
  hiddenSectors = readUInt32();
  totalSectors32 = readUInt32();

  if (sectorsPerFat == 0) {
    type = FatType::FAT32;

    largeSectorsPerFat = readUInt32();
    extendedFlags = readUInt16();
    fsVersion = readUInt16();
    rootDirFirstCluster = readUInt32();
    fsInfoSector = readUInt16();
    backupBootSector = readUInt16();
  }
  else {
    auto clusterCount = 0;  // Clusters available in the file area
    auto fileAreaLbo = reservedSectors + (numFats * sectorsPerFat)
      + ((rootEntryCount * sizeof(dirent)) / bytesPerSector);
    
    if (totalSectors)
      clusterCount = (totalSectors - fileAreaLbo) / sectorsPerCluster;
    else
      clusterCount = (totalSectors32 - fileAreaLbo) / sectorsPerCluster;

    if (clusterCount < 4087)
      type = FatType::FAT12;
    else
      type = FatType::FAT16;
  }
}

uint16_t FatBootSector::getReservedSectors()
{
  return this->reservedSectors;
}

uint8_t FatBootSector::getNumFats()
{
  return this->numFats;
}

uint32_t FatBootSector::getSectorsPerFat()
{
  return (type == FatType::FAT32) ? largeSectorsPerFat : sectorsPerFat;
}

uint64_t FatBootSector::getFatByteOffset()
{
  return this->reservedSectors * this->bytesPerSector;
}

uint64_t FatBootSector::getBytesPerFat()
{
  return this->getSectorsPerFat() * this->bytesPerSector;
}

uint64_t FatBootSector::getFileAreaByteOffset()
{
  auto fatByteOffset = getFatByteOffset();
  auto bytesPerFat = getBytesPerFat();
  auto numFats = getNumFats();
  return fatByteOffset + (bytesPerFat * numFats);
}

uint32_t FatBootSector::getRootDirFirstCluster()
{
  return (type == FatType::FAT32) ? rootDirFirstCluster : 2;
}

uint8_t FatBootSector::readUInt8()
{
  uint8_t value;
  dataProvider->read(reinterpret_cast<char*>(&value), sizeof(value));
  return value;
}

uint16_t FatBootSector::readUInt16()
{
  uint16_t value;
  dataProvider->read(reinterpret_cast<char*>(&value), sizeof(value));
  return value;
}

uint32_t FatBootSector::readUInt32()
{
  uint32_t value;
  dataProvider->read(reinterpret_cast<char*>(&value), sizeof(value));
  return value;
}

}
}