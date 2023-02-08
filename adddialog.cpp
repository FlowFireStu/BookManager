#include "adddialog.h"
#include "ui_adddialog.h"
#include "book.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

addDialog::addDialog(QSqlDatabase db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDialog)
{
    ui->setupUi(this);
    m_db = db;
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_addButton_clicked()
{
    Book newBook(ui->nameEdit->text(), ui->authorEdit->text(), ui->priceEdit->text().toDouble());
    QString insertComd = QString("insert into booklist(name, author, price) "
                                 "values(\"%1\", \"%2\", \"%3\");")
            .arg(newBook.name(), newBook.author()).arg(newBook.price());

    if (m_db.open())
    {
        QSqlQuery query(m_db);
        if (!query.exec(insertComd))
            QMessageBox::critical(this, "添加失败", "数据写入失败");

        m_db.close();
    }
    else
    {
        QMessageBox::critical(this, "添加失败", "数据库打开失败");
    }
    accept();
}

void addDialog::on_cancelButton_clicked()
{
    reject();
}

