#include "QDiskBuffer.hpp"

#include <QByteArray>

QDiskBuffer::QDiskBuffer(QObject* parent) : QHexBuffer(parent) { }

QDiskBuffer::~QDiskBuffer()
{}

uchar QDiskBuffer::at(qint64 idx)
{
  dataProvider->seek(idx);
  char c;
  bool res = dataProvider->read(&c, 1);
  return static_cast<uchar>(c);
}

qint64 QDiskBuffer::length() const
{
  return dataProvider->size();
}

void QDiskBuffer::insert(qint64 offset, const QByteArray& data)
{
  // Not implemented
}

void QDiskBuffer::remove(qint64 offset, int length)
{
  // Not implemented
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

void QDiskBuffer::write(QIODevice* device)
{
  // Not implemented
}

qint64 QDiskBuffer::indexOf(const QByteArray& ba, qint64 from)
{
  return 0;
}

qint64 QDiskBuffer::lastIndexOf(const QByteArray& ba, qint64 from)
{
  return 0;
}