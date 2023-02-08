#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
    int m_id;
    QString m_name;
    QString m_author;
    double m_price;

public:
    Book(QString name = "", QString author = "", double price = 0);
    Book(int id, QString name = "", QString author = "", double price = 0);

    int id() const;
    void setId(int newId);
    QString name() const;
    void setName(QString Name);
    QString author() const;
    void setAuthor(QString Author);
    double price() const;
    void setPrice(double Price);

    void reset();
};

#endif // BOOK_H
