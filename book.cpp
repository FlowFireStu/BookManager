#include "book.h"

Book::Book(QString name, QString author, double price)
{
    new (this) Book(0, name, author, price);
}

Book::Book(int id, QString name, QString author, double price)
{
    m_id = id;
    m_name = name;
    m_author = author;
    m_price = price;
}

int Book::id() const
{
    return m_id;
}

void Book::setId(int newId)
{
    m_id = newId;
}

QString Book::name() const
{
    return m_name;
}

void Book::setName(QString name)
{
    m_name = name;
}

QString Book::author() const
{
    return m_author;
}

void Book::setAuthor(QString author)
{
    m_author = author;
}

double Book::price() const
{
    return m_price;
}

void Book::setPrice(double price)
{
    m_price = price;
}

void Book::reset()
{
    m_id = 0;
    m_name = "";
    m_author = "";
    m_price = 0;
}
