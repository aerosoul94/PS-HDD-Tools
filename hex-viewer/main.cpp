#include "mainwindow.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}