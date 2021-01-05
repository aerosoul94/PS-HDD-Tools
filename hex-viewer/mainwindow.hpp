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
  QHexView* m_hexView;
  QFile* m_imageFile;

  disk::Disk* m_disk;
  disk::Partition* m_partition;
  bool m_searchDialogOpen;
  SearchDialog* m_searchDialog;

private slots:
  void slotOpenFile();
  void slotGotoOffset();
  void slotCopyData();
  void slotFindData();
};

#endif /* MAINWINDOW_HPP */