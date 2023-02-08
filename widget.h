#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "book.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    QSqlDatabase m_db;
    int m_columnCount;
    int m_rowCount;
    bool m_searchingState;
    Book m_bookSearching;

public:
    Widget(bool user, QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_addButton_clicked();
    void on_delButton_clicked();
    void on_flashButton_clicked();
    void on_searchButton_clicked();
    void on_idEdit_textEdited(const QString &arg);
    void on_nameEdit_textEdited(const QString &arg);
    void on_authorEdit_textEdited(const QString &arg);
    void on_priceEdit_textEdited(const QString &arg);
private:
    Ui::Widget *ui;

    void initWidget();
    void initDatabase();
    void showData(QString command);
    void showAllData();
    void resetSearchState();
    void searchBook(Book book);

};
#endif // WIDGET_H
