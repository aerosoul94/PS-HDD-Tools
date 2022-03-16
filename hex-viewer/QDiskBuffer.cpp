#include "QDiskBuffer.hpp"

#include <QByteArray>

QDiskBuffer::QDiskBuffer(QObject* parent) : QHexBuffer(parent) { }

QDiskBuffer::~QDiskBuffer()
{}

uchar QDiskBuffer::at(qint64 idx)
{
  dataProvider->seek(idx);
  char c = ' ';
  // bool res = dataProvider->read(&c, 1);
  return static_cast<uchar>(c);
}

qint64 QDiskBuffer::length() const
{
  return dataProvider->size();
}

// TODO: Implement these
void QDiskBuffer::insert([[maybe_unused]] qint64 offset, [[maybe_unused]] const QByteArray& data)
{
  // Not implemented
  assert(false && "Unimplemented.\n");
}

void QDiskBuffer::remove([[maybe_unused]] qint64 offset, [[maybe_unused]] int length)
{
  // Not implemented
  assert(false && "Unimplemented.\n");
}

QByteArray QDiskBuffer::read(qint64 offset, int length)
{
  QByteArray* byteArray = new QByteArray;
  byteArray->resize(length);
  dataProvider->seek(offset);
  dataProvider->readData(byteArray->data(), length);
  return *byteArray;
}

bool QDiskBuffer::read(QIODevice* device)
{
  this->dataProvider = static_cast<QDiskDevice*>(device);
  // Not implemented
  return true;
}

// TODO: Implement these
void QDiskBuffer::write([[maybe_unused]] QIODevice* device)
{
  // Not implemented
  assert(false && "Unimplemented.\n");
}

qint64 QDiskBuffer::indexOf([[maybe_unused]] const QByteArray& ba, [[maybe_unused]] qint64 from)
{
  assert(false && "Unimplemented.\n");
  return 0;
}

qint64 QDiskBuffer::lastIndexOf([[maybe_unused]] const QByteArray& ba,[[maybe_unused]] qint64 from)
{
  assert(false && "Unimplemented.\n");
  return 0;
}

