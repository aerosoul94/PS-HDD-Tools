#include "searchdialog.hpp"
#include "searchthread.hpp"

#include <QSizePolicy>
#include <QMessageBox>
#include <QDebug>
#include <QEventLoop>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

SearchDialog::SearchDialog(DataProvider* dataProvider, 
  QHexCursor* hexCursor, QWidget* parent) : QDialog(parent)
{
  m_dataProvider = dataProvider;
  m_hexCursor = hexCursor;
  m_progressDialog = nullptr;

  m_layout = new QGridLayout;
  m_lineEdit = new QLineEdit;
  //m_comboBox = new QComboBox;
  m_findButton = new QPushButton("&Find", this);
  m_cancelButton = new QPushButton("&Cancel", this);

  m_lineEdit->setPlaceholderText("Hex string");

  m_layout->addWidget(m_lineEdit, 0,0,1,3);
  //m_layout->addWidget(m_comboBox, 1,0,1,3);
  m_layout->addWidget(m_findButton, 2,1,1,1);
  m_layout->addWidget(m_cancelButton, 2,2,1,1);

  setLayout(m_layout);
  setFixedSize(this->sizeHint());
  setWindowTitle(tr("Find Bytes"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  setAttribute(Qt::WA_DeleteOnClose);

  m_lineEdit->setFocus();

  connect(m_findButton, SIGNAL(clicked()), this, SLOT(slotFind()));
  connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

void SearchDialog::slotCancel()
{
  this->reject();
}

void SearchDialog::setProgress(quint64 pos)
{
  auto value = ((double)pos / (double)m_dataProvider->getLength()) * INT_MAX;
  // QString label;
  // label.sprintf("%llu / %llu", pos, m_dataProvider->getLength());
  // m_progressDialog->setLabelText(label);
  m_progressDialog->setValue(value);
}

void SearchDialog::slotFind()
{
  auto text = m_lineEdit->text();
  QRegularExpression hexMatcher(
    "^([0-9A-F]{2}\\s*)+$", 
    QRegularExpression::CaseInsensitiveOption
  );
  QRegularExpressionMatch match = hexMatcher.match(text);
  if (!match.hasMatch()) {
    QMessageBox::warning(this, tr("Error"), 
      tr("The pattern has an invalid hex string."));
    return;
  }

  auto bytes = QByteArray::fromHex(text.toUtf8());

  if (!m_progressDialog)
    m_progressDialog = new QProgressDialog(this);
  m_progressDialog->setWindowTitle("Finding bytes");
  m_progressDialog->setWindowModality(Qt::WindowModal);
  auto currentValue = ((double)m_hexCursor->position().offset() / (double)m_dataProvider->getLength()) * INT_MAX;
  m_progressDialog->setValue(currentValue);
  m_progressDialog->setMinimum(0);
  m_progressDialog->setMaximum(INT_MAX);  // Maximum resolution
  m_progressDialog->setLabelText("Searching...");


  SearchThread* thread = new SearchThread(bytes, m_dataProvider, m_hexCursor->position().offset(), this);
  connect(thread, SIGNAL(progress(quint64)), this, SLOT(setProgress(quint64)));
  connect(thread, SIGNAL(onFound(quint64,quint32)), this, SLOT(onSearchCompleted(quint64, quint32)));
  connect(m_progressDialog, SIGNAL(canceled()), thread, SLOT(cancel()));
  m_progressDialog->show();
  thread->start();
}

void SearchDialog::onSearchCompleted(quint64 offset, quint32 size)
{
  m_hexCursor->moveTo(offset);
  m_hexCursor->select(size);
  m_progressDialog->close();
  parentWidget()->activateWindow();
}