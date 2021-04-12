#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QLabel>

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
  QLabel* m_offsetLabel;
  QLabel* m_selectedRangeLabel;
  QLabel* m_selectedLengthLabel;

private slots:
  void updateStatusBar();
  void slotOpenFile();
};

#endif /* MAINWINDOW_HPP */