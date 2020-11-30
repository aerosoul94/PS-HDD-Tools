#ifndef QDISKDEVICE_HPP
#define QDISKDEVICE_HPP

#include <QIODevice>

#include <io/data/data_provider.hpp>

/// A QIODevice adapter for DataProvider
class QDiskDevice : public QIODevice
{
  DataProvider* dataProvider;
public:
  explicit QDiskDevice(DataProvider*);
  qint64 readData(char* data, qint64 maxSize) override;
  qint64 writeData(const char* data, qint64 maxSize) override;
  bool seek(qint64 pos) override;
  qint64 size() const override;
};

#endif