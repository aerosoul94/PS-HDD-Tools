#include "mainwindow.hpp"
#include "hexview.hpp"
#include "QFileDiskStream.hpp"
#include "document/qhexcursor.h"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>

#include <disk/partition.hpp>
#include <formats/disk_format_factory.hpp>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  m_tabWidget = new QTabWidget;

  m_imageFile = nullptr;
  m_disk = nullptr;

  m_offsetLabel = new QLabel(this);
  m_selectedRangeLabel = new QLabel(this);
  m_selectedLengthLabel = new QLabel(this);

  statusBar()->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
  statusBar()->addPermanentWidget(m_selectedLengthLabel, 1);
  statusBar()->addPermanentWidget(m_selectedRangeLabel, 1);
  statusBar()->addPermanentWidget(m_offsetLabel, 1);

  setCentralWidget(m_tabWidget);

  addToolBar("File");
  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction("Open..", this, SLOT(slotOpenFile()));

  connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateStatusBar()));
}

void MainWindow::updateStatusBar()
{
  auto index = m_tabWidget->currentIndex();
  HexView* widget = qobject_cast<HexView*>(m_tabWidget->widget(index));
  QHexView* hexView = widget->getHexView();

  auto* cursor = hexView->document()->cursor();
  auto offset = cursor->position().offset();
  auto start = cursor->selectionStart().offset();
  auto end = cursor->selectionEnd().offset();
  auto length = cursor->selectionLength();

  m_offsetLabel->setText(QString("Current Offset: %1").arg(QString("%1").arg(offset, 0, 16).toUpper()));
  if (length) {
    m_selectedRangeLabel->setText(QString("Range: %1-%2").arg(
      QString("%1").arg(start, 0, 16).toUpper(),
      QString("%1").arg(end, 0, 16).toUpper()
    ));
    m_selectedLengthLabel->setText(QString("Length: %1").arg(
      QString("%1").arg(length, 0, 16).toUpper()));
  }
  else {
    m_selectedRangeLabel->setText("");
    m_selectedLengthLabel->setText("");
  }
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
    auto hexView = new HexView(partition);
    m_tabWidget->addTab(hexView, tr(partition->getName().c_str()));
    connect(hexView, SIGNAL(positionUpdate()), this, SLOT(updateStatusBar()));
  }
}
