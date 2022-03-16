#include "QFileDiskStream.hpp"
#include <QDebug>

QFileDiskStream::QFileDiskStream(QFile* file)
  : file(file)
{}

uint64_t QFileDiskStream::read(char* data, uint32_t length)
{
  //qInfo() << "Reading at: " << this->file->pos() << " of length " << length;
  return this->file->read(data, length);
}

uint64_t QFileDiskStream::seek(int64_t offset, [[maybe_unused]] uint32_t whence)
{
  // TODO: handle whence
  this->file->seek(offset);
  return this->file->pos();
}

uint64_t QFileDiskStream::tell()
{
  return this->file->pos();
}

uint64_t QFileDiskStream::getLength() const
{
  return this->file->size();
}
