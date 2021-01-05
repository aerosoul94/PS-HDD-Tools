#include "searchthread.hpp"

#include <QDebug>

SearchThread::SearchThread(QByteArray needle, io::data::DataProvider* hayStack, 
  quint64 from, QObject* parent) : QThread(parent)
{
  this->m_needle = needle;
  this->m_hayStack = hayStack;
  this->m_from = from;
}

void SearchThread::run()
{
  const quint32 kBufSize = 0x4000000;
  quint64 pos = static_cast<quint64>(m_from) + 1;
  QByteArray buffer(kBufSize, 0);

  qDebug() << m_needle.toHex();
  for (; pos < m_hayStack->getLength(); 
      pos += (kBufSize - m_needle.size())) {
    qDebug() << hex << pos;
    if (m_canceled)
      return;
    emit progress(pos);
    m_hayStack->seek(pos);
    m_hayStack->read(buffer.data(), kBufSize);
    auto idx = buffer.indexOf(m_needle);
    if (idx != -1) {
      emit onFound(pos + idx, m_needle.size());
      return;
    }
  }
}

void SearchThread::cancel()
{
  this->m_canceled = true;
}