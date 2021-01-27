#include "mainwindow.hpp"

#include <QtWidgets/QApplication>

#include <logging/logger.hpp>
#include <logging/stdout_log_handler.hpp>

int main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  auto logHandler = new logging::StdOutLogHandler();
  logHandler->setLogLevel(logging::LogLevel::Debug);
  rAddHandler("stdout", logHandler);

  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}