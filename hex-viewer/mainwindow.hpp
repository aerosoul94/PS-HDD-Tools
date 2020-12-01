#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>

#include <qhexview.h>

#include <disk/disk.hpp>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = Q_NULLPTR);

private:
  QHexView* m_hexView;
  QFile* m_imageFile;

  Disk* m_disk;

private slots:
  void slotOpenFile();
  void slotGotoOffset();
  void slotCopyData();
};

#endif /* MAINWINDOW_HPP */