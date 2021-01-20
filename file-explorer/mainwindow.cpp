#include "mainwindow.hpp"
#include "QFileDiskStream.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QApplication>

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

  m_partition = m_disk->getPartitions()[0];
  m_partition->mount();
  
  // Populate the tree view
  auto vfs = m_partition->getVfs();
  auto root = vfs->getRoot();

  m_treeViewModel = new QStandardItemModel;
  QStandardItem* item = new QStandardItem(m_partition->getName().c_str());
  item->setIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon));
  m_treeViewModel->appendRow(item);

  populateTreeView(item, root);

  m_treeView->setModel(m_treeViewModel);
}

void MainWindow::populateTreeView(QStandardItem* parent, vfs::VfsDirectory* root)
{
  for (auto node : root->getChildren()) {
    // Add the node
    QIcon icon = node->getType() == vfs::VfsNodeType::DIRECTORY ? 
      QApplication::style()->standardIcon(QStyle::SP_DirIcon) :
      QApplication::style()->standardIcon(QStyle::SP_FileIcon);
      
    auto item = new QStandardItem(icon, node->getName().c_str());
    parent->appendRow(item);

    if (node->getType() == vfs::VfsNodeType::DIRECTORY) {
      populateTreeView(item, static_cast<vfs::VfsDirectory*>(node));
    }
  }
}