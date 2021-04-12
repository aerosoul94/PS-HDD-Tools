#include "hexview.hpp"
#include "QDiskDevice.hpp"
#include "QDiskBuffer.hpp"

#include <QAction>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>

HexView::HexView(disk::Partition* partition, QWidget* parent)
  : m_partition(partition), QWidget(parent)
{
  m_searchDialog = nullptr;

  m_hexView = new QHexView();
  m_hexView->setReadOnly(true);

  QDiskDevice* device = new QDiskDevice(m_partition->getDataProvider());
  device->open(QIODevice::ReadOnly);
  QHexDocument* hexEditData = QHexDocument::fromDevice<QDiskBuffer>(device);
  m_hexView->setDocument(hexEditData);

  m_hexCursor = m_hexView->document()->cursor();

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_hexView);
  setLayout(mainLayout);

  // Add go-to action
  QAction* gotoAction = new QAction(this);
  gotoAction->setShortcut(Qt::CTRL | Qt::Key_G);
  connect(gotoAction, SIGNAL(triggered()), this, SLOT(slotGotoOffset()));
  this->addAction(gotoAction);
  
  // Add copy action
  // TODO: For some reason QHexView doesn't allow copying when in read-only mode
  QAction* copyAction = new QAction(this);
  copyAction->setShortcut(Qt::CTRL | Qt::Key_C);
  connect(copyAction, SIGNAL(triggered()), this, SLOT(slotCopyData()));
  this->addAction(copyAction);

  // Add find action
  QAction* findAction = new QAction(this);
  findAction->setShortcut(Qt::CTRL | Qt::Key_F);
  connect(findAction, SIGNAL(triggered()), this, SLOT(slotFindData()));
  this->addAction(findAction);

  // Selection changed update
  connect(m_hexCursor, SIGNAL(positionChanged()), this, SLOT(positionChanged()));
}

void HexView::positionChanged()
{
  emit positionUpdate();
}

void HexView::slotGotoOffset()
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

void HexView::slotCopyData()
{
  auto document = m_hexView->document();
  if (!document->cursor()->hasSelection())
    return;
  auto bytes = document->selectedBytes().toHex(' ').toUpper();
  qApp->clipboard()->setText(bytes);
}

void HexView::slotFindData()
{
  // Make sure a partition is loaded
  if (!m_partition)
    return;
  // Check if the search dialog is already open
  if (m_searchDialog && m_searchDialog->isVisible()) {
    m_searchDialog->activateWindow();
    return;
  }
  m_searchDialog = new SearchDialog(m_partition->getDataProvider(),
    m_hexView->document()->cursor());
  m_searchDialog->exec();
  m_searchDialog = nullptr;
}