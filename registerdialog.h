#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

    bool m_user;
    QString m_name;
    QString m_password;
    QSqlDatabase m_db;
public:
    explicit RegisterDialog(QSqlDatabase db, QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_pushButton_clicked();

    void on_normalBtn_clicked();

    void on_adminBtn_clicked();

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTERDIALOG_H
