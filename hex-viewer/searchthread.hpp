#ifndef SEARCHTHREAD_HPP
#define SEARCHTHREAD_HPP

#include <QObject>
#include <QThread>
#include <QByteArray>

#include <io/data/data_provider.hpp>

class SearchThread : public QThread
{
  Q_OBJECT

public:
  SearchThread(QByteArray needle, 
    io::data::DataProvider* hayStack, quint64 from, QObject* parent = nullptr);

private:
  void run() override;

public slots:
  void cancel();

signals:
  void progress(quint64 value);
  void onFound(quint64 offset, quint32 size);

private:
  bool m_canceled = false;
  quint64 m_from;
  QByteArray m_needle;
  io::data::DataProvider* m_hayStack;
};

#endif