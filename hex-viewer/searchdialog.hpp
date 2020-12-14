#ifndef SEARCHDIALOG_HPP
#define SEARCHDIALOG_HPP

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressDialog>

#include <io/data/data_provider.hpp>

#include <document/qhexcursor.h>

class SearchDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SearchDialog(DataProvider* dataProvider, 
    QHexCursor* hexCursor, QWidget* parent = nullptr);
  
private:
  QGridLayout* m_layout;
  QLineEdit* m_lineEdit;
  QComboBox* m_comboBox;
  QPushButton* m_findButton;
  QPushButton* m_cancelButton;

  DataProvider* m_dataProvider;
  QHexCursor* m_hexCursor;

  QProgressDialog* m_progressDialog;

private slots:
  void slotCancel();
  void slotFind();
  void setProgress(quint64);
  void onSearchCompleted(quint64, quint32);
};

#endif