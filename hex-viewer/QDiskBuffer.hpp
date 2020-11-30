#ifndef QDISKBUFFER_HPP
#define QDISKBUFFER_HPP

#include <io/data/data_provider.hpp>

#include "QDiskDevice.hpp"

#include <document/buffer/qhexbuffer.h>

class QDiskBuffer : public QHexBuffer
{
  Q_OBJECT

public:
  explicit QDiskBuffer(QObject* parent = nullptr);
  ~QDiskBuffer();

  uchar at(qint64 idx) override;
  qint64 length() const override;
  void insert(qint64 offset, const QByteArray& data) override;
  void remove(qint64 offset, int length) override;
  QByteArray read(qint64 offset, int length) override;
  bool read(QIODevice* device) override;
  void write(QIODevice* device) override;

  qint64 indexOf(const QByteArray& ba, qint64 from) override;
  qint64 lastIndexOf(const QByteArray& ba, qint64 from) override;

private:
  QDiskDevice* dataProvider;
};

#endif