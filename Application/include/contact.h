#pragma once
#include <QString>
#include <QVariant>
#include "dbtypes.h"

using namespace db;

class Contact
{
public:
    Contact() = default;
    Contact(QString name, QString surname, QString phoneNumber, db::DBIndex id);

    QString name() const;
    QString surname() const;
    QString phoneNumber() const;

private:
    QString m_name;
    QString m_surname;
    QString m_phoneNumber;
    db::DBIndex m_id;
};
