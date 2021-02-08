#include "mainwindow.hpp"
#include "QFileDiskStream.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QApplication>
#include <QHeaderView>

#include <formats/disk_format_factory.hpp>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  m_treeView = new QTreeView;

  //m_treeView->setModel(m_treeViewModel);

  setCentralWidget(m_treeView);

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

  QFile* imageFile = new QFile(fileName);
  imageFile->open(QIODevice::ReadOnly);

  QFile* keyFile = new QFile(keyFileName);
  keyFile->open(QIODevice::ReadOnly);

  populateExplorer(imageFile, keyFile);
}

void MainWindow::populateExplorer(QFile* imageFile, QFile* keyFile)
{
  QFileDiskStream* stream = new QFileDiskStream(imageFile);

  disk::DiskConfig config;
  config.setStream(stream);
  config.setKeys(keyFile->readAll().data(), keyFile->size());

  m_disk = formats::DiskFormatFactory::getInstance()->detectFormat(&config);
  if (!m_disk) {
    QMessageBox::warning(this, tr("Cannot load disk image"),
      tr("Disk format not recognized."));
    return;
  }

  m_treeViewModel = new QStandardItemModel;
  m_treeViewModel->setHorizontalHeaderLabels(QStringList() << "File Name" 
    << "File Size" << "Date Created" << "Date Modified" << "Date Accessed");

  for (auto partition : m_disk->getPartitions()) {
    partition->mount();

    auto vfs = partition->getVfs();
    auto root = vfs->getRoot();

    QStandardItem* item = new QStandardItem(partition->getName().c_str());
    item->setIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon));
    m_treeViewModel->appendRow(item);

    populateTreeView(item, root);
  }

  m_treeView->setModel(m_treeViewModel);
}

void MainWindow::populateTreeView(QStandardItem* parent, vfs::VfsDirectory* root)
{
  for (auto node : root->getChildren()) {
    // Add the node
    QIcon icon = node->getType() == vfs::VfsNodeType::DIRECTORY ? 
      QApplication::style()->standardIcon(QStyle::SP_DirIcon) :
      QApplication::style()->standardIcon(QStyle::SP_FileIcon);
      
    QList<QStandardItem*> row;
    auto item = new QStandardItem(icon, node->getName().c_str());
    row.append(item);

    auto fileSizeCell = new QStandardItem("");
    if (node->getType() == vfs::VfsNodeType::FILE) {
      fileSizeCell->setText(
        QString::number((ulong)static_cast<vfs::VfsFile*>(node)->getFileSize())
      );
    }

    row.append(fileSizeCell);
    row.append(new QStandardItem(
      QString("%1/%2/%3 %4:%5:%6")
        .arg(node->getCreationTime()->getMonth())
        .arg(node->getCreationTime()->getDay())
        .arg(node->getCreationTime()->getYear())
        .arg(node->getCreationTime()->getHour(), 2, 10, QLatin1Char('0'))
        .arg(node->getCreationTime()->getMinute(), 2, 10, QLatin1Char('0'))
        .arg(node->getCreationTime()->getSecond(), 2, 10, QLatin1Char('0'))
    ));

    row.append(new QStandardItem(
      QString("%1/%2/%3 %4:%5:%6")
        .arg(node->getLastModifiedTime()->getMonth())
        .arg(node->getLastModifiedTime()->getDay())
        .arg(node->getLastModifiedTime()->getYear())
        .arg(node->getLastModifiedTime()->getHour(), 2, 10, QLatin1Char('0'))
        .arg(node->getLastModifiedTime()->getMinute(), 2, 10, QLatin1Char('0'))
        .arg(node->getLastModifiedTime()->getSecond(), 2, 10, QLatin1Char('0'))
    ));

    row.append(new QStandardItem(
      QString("%1/%2/%3 %4:%5:%6")
        .arg(node->getLastAccessTime()->getMonth())
        .arg(node->getLastAccessTime()->getDay())
        .arg(node->getLastAccessTime()->getYear())
        .arg(node->getLastAccessTime()->getHour(), 2, 10, QLatin1Char('0'))
        .arg(node->getLastAccessTime()->getMinute(), 2, 10, QLatin1Char('0'))
        .arg(node->getLastAccessTime()->getSecond(), 2, 10, QLatin1Char('0'))
    ));

    parent->appendRow(row);

    if (node->getType() == vfs::VfsNodeType::DIRECTORY) {
      populateTreeView(item, static_cast<vfs::VfsDirectory*>(node));
    }
  }
}