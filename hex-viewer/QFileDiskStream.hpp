#ifndef QDISKSTREAM_HPP
#define QDISKSTREAM_HPP

#include <io/stream/disk_stream.hpp>

#include <QFile>

/// A DiskStream adapter for QFile
class QFileDiskStream : public DiskStream
{
  QFile* file;
public:
  explicit QFileDiskStream(QFile*);
  uint64_t read(char*, uint32_t) override;
  uint64_t seek(int64_t, uint32_t) override;
  uint64_t tell() override;
  uint64_t getLength() const override;
};

#endif