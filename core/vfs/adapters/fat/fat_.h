#ifndef FAT_H
#define FAT_H

struct bpb {
  uint16_t bytesPerSector;
  uint8_t sectorsPerCluster;
  uint16_t reservedSectors;
  uint8_t fats;
  uint16_t rootEntries;
  uint16_t sectors; // Total number of sectors in the volume
  uint8_t media;
  uint16_t sectorsPerFat;
  uint16_t sectorsPerTrack;
  uint16_t heads;
  uint16_t hiddenSectors;
  uint16_t largeSectors;
};

#pragma pack(push,1)
struct bpb_ex {
  uint16_t bytesPerSector;      // 0x0
  uint8_t sectorsPerCluster;    // 0x2
  uint16_t reservedSectors;     // 0x3
  uint8_t fats;                 // 0x5
  uint16_t rootEntries;         // 0x6
  uint16_t sectors;             // 0x8
  uint8_t media;                // 0xA
  uint16_t sectorsPerFat;       // 0xB
  uint16_t sectorsPerTrack;     // 0xD
  uint16_t heads;               // 0xF
  uint32_t hiddenSectors;       // 0x11
  uint32_t largeSectors;        // 0x15
  uint32_t largeSectorsPerFat;  // 0x19
  uint16_t extendedFlags;       // 0x1D
  uint16_t fsVersion;           // 0x1F
  uint32_t rootDirFirstCluster; // 0x21
  uint16_t fsInfoSector;        // 0x25
  uint16_t backupBootSector;    // 0x27
  uint8_t reserved[12];         // 0x29
};  // 0x35
#pragma pack(pop)

struct bootsector {
  uint8_t jump;
  uint8_t oem[8];
  bpb packedBpb;
  uint8_t physicalDriveNumber;
  uint8_t currentHead;
  uint8_t signature;
  uint8_t id[4];
  uint8_t volumeLabel[11];
  uint8_t systemId[8];
};

struct bootsector_ex {
  uint8_t jump;
  uint8_t oem[8];
  bpb_ex packedBpb;
  uint8_t physicalDriveNumber;
  uint8_t currentHead;
  uint8_t signature;
  uint8_t id[4];
  uint8_t volumeLabel[11];
  uint8_t systemId[8];
};

struct fsinfo {
  uint32_t sectorBeginSignature;
  uint8_t extraBootCode[480];
  uint32_t fsInfoSignature;
  uint32_t freeClusterCount;
  uint32_t nextFreeCluster;
  uint8_t reserved[12];
  uint32_t sectorEndSignature;
};

struct fat_time {
  uint16_t doubleSeconds : 5;
  uint16_t minute : 6;
  uint16_t hour : 5;
};

struct fat_date {
  uint16_t day : 5;
  uint16_t month : 4;
  uint16_t year : 7;
};

struct fat_time_stamp {
  fat_time time;
  fat_date date;
};

struct dirent {
  uint8_t fileName[11];
  uint8_t attributes;
  uint8_t ntByte;
  uint8_t creationMsec;
  fat_time_stamp creationTime;
  fat_date lastAccessDate;
  union {
    uint16_t extendedAttributes;
    uint16_t firstClusterOfFileHi;
  };
  fat_time_stamp lastWriteTime;
  uint16_t firstClusterOfFile;
  uint32_t fileSize;
}; 

#pragma pack(push,1)
struct dirent_lfn {
  uint8_t ordinal;
  uint16_t name1[5];
  uint8_t attributes;
  uint8_t type;
  uint8_t checksum;
  uint16_t name2[6];
  uint16_t mustBeZero;
  uint16_t name3[2];
};
#pragma pack(pop)

#define FAT_DIRENT_NEVER_USED            0x00
#define FAT_DIRENT_REALLY_0E5            0x05
#define FAT_DIRENT_DIRECTORY_ALIAS       0x2e
#define FAT_DIRENT_DELETED               0xe5

#define FAT_DIRENT_ATTR_READ_ONLY        0x01
#define FAT_DIRENT_ATTR_HIDDEN           0x02
#define FAT_DIRENT_ATTR_SYSTEM           0x04
#define FAT_DIRENT_ATTR_VOLUME_ID        0x08
#define FAT_DIRENT_ATTR_DIRECTORY        0x10
#define FAT_DIRENT_ATTR_ARCHIVE          0x20
#define FAT_DIRENT_ATTR_LFN              (FAT_DIRENT_ATTR_READ_ONLY | \
                                          FAT_DIRENT_ATTR_HIDDEN |    \
                                          FAT_DIRENT_ATTR_SYSTEM |    \
                                          FAT_DIRENT_ATTR_VOLUME_ID)

#define FAT_LAST_LONG_ENTRY              0x40

#define IsBpbFat32(bpb) (*(uint16_t*)(&(bpb)->sectorsPerFat) == 0)

#define FatBytesPerCluster(B) ((uint32_t)(B->bytesPerSector * B->sectorsPerCluster))
#define FatBytesPerFat(B) (IsBpbFat32(B) ? \
  (uint32_t)(B->bytesPerSector * B->largeSectorsPerFat) : \
  (uint32_t)(B->bytesPerSector * B->sectorsPerFat)) 
#define FatReservedBytes(B) (B->bytesPerSector * B->reservedSectors)

#endif /* FAT_H */
