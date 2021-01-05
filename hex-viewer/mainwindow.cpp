#include "mainwindow.hpp"
#include "searchdialog.hpp"
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
  m_imageFile = nullptr;

  m_disk = nullptr;
  m_partition = nullptr;
  m_searchDialog = nullptr;

  m_hexView = new QHexView(this);
  m_hexView->setReadOnly(true);

  setCentralWidget(m_hexView);

  addToolBar("File");
  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction("Open..", this, SLOT(slotOpenFile()));

  QAction* gotoAction = new QAction(this);
  gotoAction->setShortcut(Qt::CTRL | Qt::Key_G);
  connect(gotoAction, SIGNAL(triggered()), this, SLOT(slotGotoOffset()));
  this->addAction(gotoAction);

  // TODO: For some reason QHexView doesn't allow copying when in read-only mode
  QAction* copyAction = new QAction(this);
  copyAction->setShortcut(Qt::CTRL | Qt::Key_C);
  connect(copyAction, SIGNAL(triggered()), this, SLOT(slotCopyData()));
  this->addAction(copyAction);

  QAction* findAction = new QAction(this);
  findAction->setShortcut(Qt::CTRL | Qt::Key_F);
  connect(findAction, SIGNAL(triggered()), this, SLOT(slotFindData()));
  this->addAction(findAction);
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

  m_partition = m_disk->getPartitions()[0];
  // Enable this to only view a single partition
  QDiskDevice* device = new QDiskDevice(m_partition->getDataProvider());
  // Enable this to view the entire drive
  //QDiskDevice* device = new QDiskDevice(m_disk->getDataProvider());
  device->open(QIODevice::ReadOnly);
  QHexDocument* hexEditData = QHexDocument::fromDevice<QDiskBuffer>(device);
  //hexEditData->setBaseAddress(m_partition->getStart());
  m_hexView->setDocument(hexEditData);

  m_searchDialog = new SearchDialog(m_partition->getDataProvider(), 
    m_hexView->document()->cursor(), this);
  //connect(m_searchDialog, SIGNAL(finished()), SLOT());
}

void MainWindow::slotGotoOffset()
{
  bool ok;
  QString offsetText = QInputDialog::getText(this, tr("Go to offset"), tr("Enter Offset"),
    QLineEdit::Normal, "", &ok);
  if (ok && !offsetText.isEmpty()) {
    quint64 offset;
    if (offsetText.startsWith("0x"))
      offset = offsetText.toULongLong(&ok, 16);
    else
      offset = offsetText.toULongLong(&ok, 10);
    if (ok) {
      auto cursor = m_hexView->document()->cursor();
      cursor->moveTo(offset);
    }
  }
}

void MainWindow::slotCopyData()
{
  auto document = m_hexView->document();
  if (!document->cursor()->hasSelection())
    return;
  auto bytes = document->selectedBytes().toHex(' ').toUpper();
  qApp->clipboard()->setText(bytes);
}

void MainWindow::slotFindData()
{
  // Make sure a disk is loaded
  if (!m_partition)
    return;
  // Check if the search dialog is already open
  if (m_searchDialog && m_searchDialog->isVisible()) {
    m_searchDialog->activateWindow();
    return;
  }
  m_searchDialog->show();
}