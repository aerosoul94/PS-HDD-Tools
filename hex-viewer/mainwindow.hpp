#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>

#include <qhexview.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = Q_NULLPTR);

private:
  QHexView* m_hexView;
  QFile* m_imageFile;

private slots:
  void slotOpenFile();
  void slotGotoOffset();
};

#endif /* MAINWINDOW_HPP */