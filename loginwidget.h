#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

    bool m_user;
    QSqlDatabase m_db;
    QString m_name;
    QString m_password;
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void on_normalBtn_clicked();
    void on_adminBtn_clicked();
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();

private:
    Ui::LoginWidget *ui;

    void initWidget();
    void initDatabase();
};

#endif // LOGINWIDGET_H
