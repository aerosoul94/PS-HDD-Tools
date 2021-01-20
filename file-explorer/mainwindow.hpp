#include <QtWidgets/QMainWindow>
#include <QTreeView>
#include <QFile>
#include <QStandardItem>
#include <QStandardItemModel>

#include <disk/disk.hpp>
#include <disk/partition.hpp>
#include <vfs/directory.hpp>
#include <vfs/file_system.hpp>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = Q_NULLPTR);

private:
  void populateExplorer(QFile* imageFile, QFile* keyFile);
  void populateTreeView(QStandardItem* parent, vfs::VfsDirectory* root);

  QTreeView* m_treeView;
  QStandardItemModel* m_treeViewModel;

  disk::Disk* m_disk;
  disk::Partition* m_partition;

private slots:
  void slotOpenFile();
};