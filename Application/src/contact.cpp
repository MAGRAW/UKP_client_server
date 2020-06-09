#include "contact.h"

Contact::Contact(QString name, QString surname, QString phoneNumber, db::DBIndex id)
    : m_name {std::move(name)},
      m_surname {std::move(surname)},
      m_phoneNumber {std::move(phoneNumber)},
      m_id {id}
{
}

QString Contact::name() const
{
    return m_name;
}

QString Contact::surname() const
{
    return m_surname;
}

QString Contact::phoneNumber() const
{
    return m_phoneNumber;
}
