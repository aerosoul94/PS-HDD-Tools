#include "mainwindow.hpp"
#include "hexview.hpp"
#include "QDiskDevice.hpp"
#include "QFileDiskStream.hpp"
#include "QDiskBuffer.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  m_tabWidget = new QTabWidget;

  m_imageFile = nullptr;
  m_disk = nullptr;

  setCentralWidget(m_tabWidget);

  addToolBar("File");
  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction("Open..", this, SLOT(slotOpenFile()));
}

void MainWindow::slotOpenFile()
{
  auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"));
  if (fileName.isNull())
    return;

  auto keyFileName = QFileDialog::getOpenFileName(this, tr("Open Key File"));
  if (keyFileName.isNull())
    return;

  if (m_imageFile && m_imageFile->isOpen())
    m_imageFile->close();
  m_imageFile = new QFile(fileName);

  if (m_disk)
    delete m_disk;

  QFile* keyFile = new QFile(keyFileName);
  m_imageFile->open(QIODevice::ReadOnly);
  QFileDiskStream* stream = new QFileDiskStream(m_imageFile);

  keyFile->open(QIODevice::ReadOnly);
  auto keyLen = keyFile->size();
  
  disk::DiskConfig config;
  config.setStream(stream);
  config.setKeys(keyFile->readAll().data(), keyLen);
  keyFile->close();

  m_disk = formats::DiskFormatFactory::getInstance()->detectFormat(&config);
  if (!m_disk) {
    QMessageBox::warning(this, tr("Cannot load disk"),
      tr("Disk format not recognized."));
    return;
  }

  if (!m_disk->getPartitions().size()) {
    QMessageBox::warning(this, tr("Cannot load disk"), 
      tr("No partitions were found."));
    return;
  }

  m_tabWidget->clear();
  for (auto partition : m_disk->getPartitions()) {
    m_tabWidget->addTab(new HexView(partition), tr(partition->getName().c_str()));
  }
}
