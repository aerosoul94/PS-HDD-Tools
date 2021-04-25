#include "selectdialog.hpp"

#include <QDialogButtonBox>
#include <QMessageBox>

SelectDialog::SelectDialog(QHexCursor* hexCursor, QWidget* parent)
  : QDialog(parent)
{
  m_hexCursor = hexCursor;

  auto start = m_hexCursor->selectionStart().offset();
  auto end = m_hexCursor->selectionEnd().offset();
  auto length = m_hexCursor->selectionLength();

  QVBoxLayout* main = new QVBoxLayout;
  QDialogButtonBox* buttonBox = new QDialogButtonBox;
  m_formLayout = new QFormLayout;
  
  m_startText = new QLineEdit;
  m_endText = new QLineEdit;
  m_lengthText = new QLineEdit;

  m_startText->setText(QString("0x%1").arg(start, 0, 16));
  m_endText->setText(QString("0x%1").arg(end, 0, 16));
  m_lengthText->setText(QString("0x%1").arg(length, 0, 16));

  m_formLayout->addRow(tr("&Start:"), m_startText);
  m_formLayout->addRow(tr("&End:"), m_endText);
  m_formLayout->addRow(tr("&Length:"), m_lengthText);

  buttonBox->addButton(tr("Ok"), QDialogButtonBox::AcceptRole);
  buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);

  main->addLayout(m_formLayout);
  main->addWidget(buttonBox);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(selectBytes()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  setLayout(main);
  setFixedSize(this->sizeHint());
  setWindowTitle(tr("Select Bytes"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  setAttribute(Qt::WA_DeleteOnClose);

  connect(m_startText, &QLineEdit::textChanged, [this]() {
    // length = end - start
    if (m_endText->text().size()) {
      bool ok;
      quint64 start, end;
      if (m_startText->text().startsWith("0x"))
        start = m_startText->text().mid(2).toULongLong(&ok, 16);
      else
        start = m_startText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (m_endText->text().startsWith("0x"))
        end = m_endText->text().mid(2).toULongLong(&ok,16);
      else
        end = m_endText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (start > end)
        return;
      auto length = (end - start) + 1;
      m_lengthText->setText(QString("0x%1").arg(length, 0, 16));
    }
  });

  connect(m_endText, &QLineEdit::textEdited, [this]() {
    // length = end - start
    if (m_startText->text().size()) {
      bool ok;
      quint64 start, end;
      if (m_startText->text().startsWith("0x"))
        start = m_startText->text().mid(2).toULongLong(&ok, 16);
      else
        start = m_startText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (m_endText->text().startsWith("0x"))
        end = m_endText->text().mid(2).toULongLong(&ok,16);
      else
        end = m_endText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (start > end)
        return;
      auto length = (end - start) + 1;
      m_lengthText->setText(QString("0x%1").arg(length, 0, 16));
    }
  });

  connect(m_lengthText, &QLineEdit::textEdited, [this]() {
    // end = start + length
    if (!m_lengthText->text().isEmpty() && !m_startText->text().isEmpty()) {
      bool ok;
      quint64 start, length;
      if (m_startText->text().startsWith("0x"))
        start = m_startText->text().mid(2).toULongLong(&ok, 16);
      else
        start = m_startText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (m_lengthText->text().startsWith("0x"))
        length = m_lengthText->text().mid(2).toULongLong(&ok,16);
      else
        length = m_lengthText->text().toULongLong(&ok);
      if (!ok)
        return;
      if (length == 0)
        return;
      auto end = (start + length) - 1;
      m_endText->setText(QString("0x%1").arg(end, 0, 16));
    }
  });
}

void SelectDialog::slotCancel()
{
  // Unused
}

void SelectDialog::selectBytes()
{
  bool ok;
  quint64 start, end;
  if (m_startText->text().startsWith("0x"))
    start = m_startText->text().mid(2).toULongLong(&ok, 16);
  else
    start = m_startText->text().toULongLong(&ok);
  if (!ok) {
    QMessageBox::warning(this, 
      "Invalid input", "Invalid start offset!");
    return;
  }
  if (m_endText->text().startsWith("0x"))
    end = m_endText->text().mid(2).toULongLong(&ok, 16);
  else
    end = m_endText->text().toULongLong(&ok);
  if (!ok) {
    QMessageBox::warning(this,
      "Invalid input", "Invalid end offset!");
    return;
  }
  // TODO: There appears to be a bug in selectOffset where it only selects a single line
  // That, or I'm using it incorrectly.
  // m_hexCursor->selectOffset(start, (end - start) + 1);
  m_hexCursor->moveTo(start / 0x10, start % 0x10);
  m_hexCursor->select(end / 0x10, end % 0x10);
  accept();
}
