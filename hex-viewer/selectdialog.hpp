#ifndef SELECTDIALOG_HPP
#define SELECTDIALOG_HPP

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>

#include <document/qhexcursor.h>

class SelectDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SelectDialog(QHexCursor* hexCursor, QWidget* parent = nullptr);

private:
  QFormLayout* m_formLayout;
  QLineEdit* m_startText;
  QLineEdit* m_endText;
  QLineEdit* m_lengthText;

  QPushButton* m_okButton;
  QPushButton* m_cancelButton;

  QHexCursor* m_hexCursor;

private slots:
  void slotCancel();
  void selectBytes();
};

#endif /* SELECTDIALOG_HPP */
