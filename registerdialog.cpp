#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
#include <QSqlQuery>

RegisterDialog::RegisterDialog(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    m_user = true;
    ui->normalBtn->setChecked(true);
    m_db = db;
    m_name = "";
    m_password = "";
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_clicked()
{
    m_name = ui->nameEdit->text();
    m_password = ui->passwordEdit->text();

    if (m_name.isEmpty())
    {
        QMessageBox::critical(this, "错误", "用户名不能为空");
        return;
    }
    if (m_password.isEmpty())
    {
        QMessageBox::critical(this, "错误", "密码不能为空");
        return;
    }

    if (!m_db.open())
    {
        QMessageBox::critical(this, "错误", "数据库连接失败");
        return;
    }

    QSqlQuery query(m_db);
    QString command;
    if (m_user)
        command = QString("select * from normallist where name = " + m_name + ";");
    else
        command = QString("select * from adminlist where name = " + m_name + ";");
    query.exec(command);

    if (query.size())
    {

        m_db.close();
        QMessageBox::critical(this, "错误", "用户已存在");
        return;
    }

    if (m_user)
        command = QString("insert into normallist ");
    else
        command = QString("select into adminlist ");
    command.append("values(\"" + m_name + "\", \"" + m_password + "\");");

    if (!query.exec(command))
    {
        m_db.close();
        QMessageBox::critical(this, "错误", "添加失败");
        return;
    }

    m_db.close();
    accept();
    close();
}


void RegisterDialog::on_normalBtn_clicked()
{
    m_user = true;
}


void RegisterDialog::on_adminBtn_clicked()
{
    m_user = false;
}

