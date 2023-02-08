#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT
    QSqlDatabase m_db;

public:
    explicit addDialog(QSqlDatabase db, QWidget *parent = nullptr);
    ~addDialog();

private slots:
    void on_addButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H
