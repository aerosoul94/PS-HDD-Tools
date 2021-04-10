#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>

#include <qhexview.h>

#include <disk/disk.hpp>

#include "searchdialog.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = Q_NULLPTR);

private:
  QTabWidget* m_tabWidget;
  QFile* m_imageFile;
  disk::Disk* m_disk;

private slots:
  void slotOpenFile();
};

#endif /* MAINWINDOW_HPP */