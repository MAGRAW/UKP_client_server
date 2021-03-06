#pragma once
#include <QAbstractListModel>
#include <vector>
#include "contact.h"
#include "contactsworker.h"//#include "contactsreader.h"


class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ContactsModel();
    static void registerMe(const std::string& moduleName);
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

private slots:
    void onContactListDownloaded(const std::vector<Contact>& data);

private:
    std::vector<Contact> m_contacts;
    ContactsWorker m_worker; //ContactsReader m_contactsReader;

    enum ContactRoles
    {
        //NameRole = Qt::UserRole + 1,
        NameRole,
        SurnameRole,
        PhoneNumberRole
    };

    bool updateContacts();
};

