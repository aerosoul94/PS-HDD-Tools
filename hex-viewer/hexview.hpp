#include <QWidget>

#include <qhexview.h>

#include <disk/partition.hpp>

#include "searchdialog.hpp"

class HexView : public QWidget
{
  Q_OBJECT

public:
  explicit HexView(disk::Partition* partition, QWidget* parent = nullptr);

  QHexView* getHexView() { return m_hexView; }

  QHexView* m_hexView;
  QHexCursor* m_hexCursor;
  disk::Partition* m_partition;

  bool m_searchDialogOpen;
  SearchDialog* m_searchDialog;

signals:
  void positionUpdate();
  
private slots:
  void positionChanged();
  void slotGotoOffset();
  void slotCopyData();
  void slotFindData();
};