#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QSqlQuery>
#include <QMessageBox>
#include "widget.h"
#include "registerdialog.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    initWidget();
    initDatabase();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_normalBtn_clicked()
{
    m_user = 1;
}


void LoginWidget::on_adminBtn_clicked()
{
    m_user = 0;
}


void LoginWidget::on_loginBtn_clicked()
{
    m_name = ui->nameEdit->text();
    m_password = ui->passwordEdit->text();


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

    if (!query.next())
    {
        m_db.close();
        QMessageBox::critical(this, "错误", "用户不存在");
        return;
    }
    if (query.value(1).toString() != m_password)
    {
        m_db.close();
        QMessageBox::critical(this, "错误", "密码错误");
        return;
    }
    m_db.close();

    Widget *mainWidget = new Widget(m_user);
    mainWidget->show();
    close();
}


void LoginWidget::on_registerBtn_clicked()
{
    RegisterDialog dialog(m_db, this);
    dialog.exec();
}

void LoginWidget::initWidget()
{
    ui->normalBtn->setChecked(true);
    m_user = true;
}

void LoginWidget::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL", "booklist");
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("password");

    if (!m_db.open())
    {
        QMessageBox::critical(this, "错误", "数据库连接失败");
        return;
    }
    m_db.exec("create database if not exists book;");
//    if (!m_db.exec("create database if not exists book;").next())
//    {
//        m_db.close();
//        QMessageBox::critical(this, "错误", "数据库创建失败");
//        return;
//    }
    m_db.close();

    m_db.setDatabaseName("book");
    if (!m_db.open())
    {
        QMessageBox::critical(this, "错误", "数据库打开失败");
        return;
    }
    QSqlQuery query(m_db);
    if (!query.exec("create table if not exists normallist("
                    "name varchar(20) primary key,"
                    "password varchar(20));"))
    {
        m_db.close();
        QMessageBox::critical(this, "错误", "数据表创建失败");
        return;
    }

    if (!query.exec("create table if not exists adminlist("
                    "name varchar(20) primary key,"
                    "password varchar(20));"))
    {
        m_db.close();
        QMessageBox::critical(this, "错误", "数据表创建失败");
        return;
    }

    m_db.close();
}

