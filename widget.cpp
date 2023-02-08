#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout>
#include "adddialog.h"

Widget::Widget(bool user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    if (user)
    {
        ui->addButton->hide();
        ui->delButton->hide();
    }

    initWidget();
    initDatabase();
    resetSearchState();
    showAllData();
}

Widget::~Widget()
{
    ui->listTable->clearContents();
    delete ui;
}


void Widget::on_addButton_clicked()
{
    addDialog dialog(m_db, this);
    //添加成功则刷新列表
    if (dialog.exec())
    {
        showAllData();
    }
}


void Widget::on_delButton_clicked()
{
    int delRow = ui->listTable->currentRow();
    int delId = ui->listTable->item(delRow, 0)->text().toInt();

    auto selectBtn = QMessageBox::question(this, "删除", "是否删除所选书籍");
    if (selectBtn == QMessageBox::Yes)
    {
        m_db.open();
        QSqlQuery query(m_db);
        QString command = QString("delete from booklist where id=%1;").arg(delId);
        if (!query.exec(command))
             QMessageBox::critical(this, "错误", "删除失败");
        else
             QMessageBox::about(this, "成功", "删除成功");
        m_db.close();
        showAllData();
    }
}

void Widget::on_flashButton_clicked()
{
    showAllData();
}

void Widget::on_searchButton_clicked()
{
    if (m_searchingState)
    {
        resetSearchState();
        showAllData();
    }
    else
    {
        m_searchingState = true;
        ui->searchWidget->setFixedHeight(50);
    }
}

void Widget::initWidget()
{
    m_columnCount = 4;
    m_rowCount = 0;
    ui->listTable->setColumnCount(m_columnCount);
    ui->listTable->setRowCount(m_rowCount);
    QStringList headLabels;
    headLabels << "编号" << "书名" << "作者" << "价格";
    ui->listTable->setHorizontalHeaderLabels(headLabels);
}

void Widget::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL", "booklist");
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("password");

    if (!m_db.open())
    {
        QMessageBox::critical(this, "错误", "数据库连接失败");
    }
    else
    {
        if (!m_db.exec("show databases like \"book\";").next())
            if (!m_db.exec("create database book;").next())
            {
                m_db.close();
                QMessageBox::critical(this, "错误", "数据库创建失败");
            }
        m_db.close();
    }

    m_db.setDatabaseName("book");

    if (!m_db.open())
    {
        QMessageBox::critical(this, "错误", "数据库打开失败");
    }
    else
    {
        QSqlQuery query(m_db);
        query.exec("show tables like \"booklist\";");
        if (!query.next())
            if (!query.exec("create table booklist("
                            "id int primary key auto_increment,"
                            "name varchar(20),"
                            "author varchar(20),"
                            "price double);"))
                QMessageBox::critical(this, "错误", "数据表创建失败");

        m_db.close();
    }
}

void Widget::showData(QString command)
{
    m_db.open();
    QSqlQuery query(m_db);
    query.exec(command);
    m_rowCount = query.size();

    if (m_rowCount == -1)
        QMessageBox::critical(this, "错误", "数据显示错误");

    ui->listTable->clearContents();
    if (m_rowCount >= 0)
        ui->listTable->setRowCount(m_rowCount);

    for (int currentRow = 0; query.next(); currentRow++)
    {
        ui->listTable->setItem(currentRow, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->listTable->setItem(currentRow, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->listTable->setItem(currentRow, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->listTable->setItem(currentRow, 3, new QTableWidgetItem(query.value(3).toString()));
    }

    m_db.close();
}

void Widget::showAllData()
{
    showData("select * from booklist;");
}

void Widget::resetSearchState()
{
    m_searchingState = false;
    m_bookSearching.reset();
    ui->searchWidget->setFixedHeight(0);
    ui->idEdit->setText("");
    ui->nameEdit->setText("");
    ui->authorEdit->setText("");
    ui->priceEdit->setText("");
}

void Widget::searchBook(Book book)
{
    QString conditionComd("");
    if (book.id())
    {
        conditionComd.append(" where id = " + QString::number(book.id()));
    }
    if (!book.name().isEmpty())
    {
        if (conditionComd.isEmpty())
        {
            conditionComd.append(" where");
        }
        else
        {
            conditionComd.append(" and");
        }
        conditionComd.append(" name like \"%" + book.name() + "%\"");
    }
    if (!book.author().isEmpty())
    {
        if (conditionComd.isEmpty())
        {
            conditionComd.append(" where");
        }
        else
        {
            conditionComd.append(" and");
        }
        conditionComd.append(" author like \"%" + book.author() + "%\"");
    }
    if (book.price())
    {
        if (conditionComd.isEmpty())
        {
            conditionComd.append(" where");
        }
        else
        {
            conditionComd.append(" and");
        }
        conditionComd.append(" price = " + QString::number(book.price()));
    }
    QString searchComd = QString("select * from booklist" + conditionComd + ";");

    showData(searchComd);
}

void Widget::on_idEdit_textEdited(const QString &arg)
{
    int id = ui->idEdit->text().toInt();
    m_bookSearching.setId(id);
    searchBook(m_bookSearching);
}

void Widget::on_nameEdit_textEdited(const QString &arg)
{
    QString name = ui->nameEdit->text();
    m_bookSearching.setName(name);
    searchBook(m_bookSearching);
}

void Widget::on_authorEdit_textEdited(const QString &arg)
{
    QString author = ui->authorEdit->text();
    m_bookSearching.setAuthor(author);
    searchBook(m_bookSearching);
}

void Widget::on_priceEdit_textEdited(const QString &arg1)
{
    int price = ui->priceEdit->text().toDouble();
    m_bookSearching.setPrice(price);
    searchBook(m_bookSearching);
}
