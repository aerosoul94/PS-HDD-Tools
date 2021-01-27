#ifndef FAT_BOOT_SECTOR_HPP
#define FAT_BOOT_SECTOR_HPP

#include <io/data/data_provider.hpp>

namespace vfs {
namespace adapters {

enum class FatType {
  FAT12,
  FAT16,
  FAT32
};

class FatBootSector
{
public:
  FatBootSector(io::data::DataProvider* dataProvider);

  uint16_t getReservedSectors();
  uint8_t getNumFats();
  uint32_t getSectorsPerFat();
  uint64_t getFatByteOffset();
  uint64_t getBytesPerFat();
  uint64_t getFileAreaByteOffset();
  uint32_t getRootDirFirstCluster();
  uint32_t getRootDirEntryCount();
  uint32_t getSectorsPerCluster();
  uint32_t getBytesPerCluster();
  FatType getFatType();

private:
  void readBootSector(uint64_t offset);

  // TODO: Write a helper class for this.
  uint8_t readUInt8();
  uint16_t readUInt16();
  uint32_t readUInt32();

  io::data::DataProvider* dataProvider;
  FatType type;

  // These fields are read from the boot sector.
  uint16_t bytesPerSector;
  uint8_t  sectorsPerCluster;
  uint16_t reservedSectors;
  uint8_t  numFats;
  uint16_t rootEntryCount;
  uint16_t totalSectors;
  uint8_t  media;
  uint16_t sectorsPerFat;
  uint16_t sectorsPerTrack;
  uint16_t numHeads;
  uint32_t hiddenSectors;
  uint32_t totalSectors32;

  uint32_t largeSectorsPerFat;
  uint16_t extendedFlags;
  uint16_t fsVersion;
  uint32_t rootDirFirstCluster;
  uint16_t fsInfoSector;
  uint16_t backupBootSector;
};

}
}

#endif /* FAT_BOOT_SECTOR_HPP */