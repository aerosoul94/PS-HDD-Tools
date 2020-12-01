#include "mainwindow.hpp"
#include "QDiskDevice.hpp"
#include "QFileDiskStream.hpp"
#include "QDiskBuffer.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QInputDialog>

#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  m_imageFile = nullptr;

  m_hexView = new QHexView(this);
  m_hexView->setReadOnly(true);

  setCentralWidget(m_hexView);

  QToolBar* fileToolBar = addToolBar("File");
  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction("Open..", this, SLOT(slotOpenFile()));

  QAction* gotoAction = new QAction(this);
  gotoAction->setShortcut(Qt::CTRL | Qt::Key_G);
  connect(gotoAction, SIGNAL(triggered()), this, SLOT(slotGotoOffset()));
  this->addAction(gotoAction);
}

void MainWindow::slotOpenFile()
{
  auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"));
  auto keyFileName = QFileDialog::getOpenFileName(this, tr("Open Key File"));
  if (m_imageFile && m_imageFile->isOpen())
    m_imageFile->close();
  m_imageFile = new QFile(fileName);
  QFile* keyFile = new QFile(keyFileName);
  m_imageFile->open(QIODevice::ReadOnly);
  keyFile->open(QIODevice::ReadOnly);
  auto keyLen = keyFile->size();
  QFileDiskStream* stream = new QFileDiskStream(m_imageFile);
  DiskConfig config;
  config.setStream(stream);
  config.setKeys(keyFile->readAll().data(), keyLen);
  keyFile->close();
  Disk* disk = DiskFormatFactory::getInstance()->detectFormat(&config);
  // Enable this to only view a single partition
  QDiskDevice* device = new QDiskDevice(disk->getPartitions()[0]->getDataProvider());
  // Enable this to view the entire drive
  //QDiskDevice* device = new QDiskDevice(disk->getDataProvider());
  device->open(QIODevice::ReadOnly);
  QHexDocument* hexEditData = QHexDocument::fromDevice<QDiskBuffer>(device);
  m_hexView->setDocument(hexEditData);
}

void MainWindow::slotGotoOffset()
{
  bool ok;
  QString offsetText = QInputDialog::getText(this, tr("Go to offset"), tr("Enter Offset"),
    QLineEdit::Normal, "", &ok);
  if (ok && !offsetText.isEmpty()) {
    quint64 offset = offsetText.toULongLong();
    auto cursor = m_hexView->document()->cursor();
    cursor->moveTo(offset);
  }
}