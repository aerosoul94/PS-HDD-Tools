#include <QWidget>

#include <qhexview.h>

#include <disk/partition.hpp>

#include "searchdialog.hpp"

class HexView : public QWidget
{
  Q_OBJECT

public:
  explicit HexView(disk::Partition* partition, QWidget* parent = nullptr);

  QHexView* m_hexView;
  disk::Partition* m_partition;

  bool m_searchDialogOpen;
  SearchDialog* m_searchDialog;
  
private slots:
  void slotGotoOffset();
  void slotCopyData();
  void slotFindData();
};