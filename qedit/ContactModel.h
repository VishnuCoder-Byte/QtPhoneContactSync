#pragma once
#include <QAbstractListModel>
#include <vector>
#include <QMetaType>

struct Contact {
    int index;
    char name[64];
    char number[32];
};

Q_DECLARE_METATYPE(Contact)

class ContactModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { IndexRole = Qt::UserRole + 1, NameRole, NumberRole };

    ContactModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addContact(const Contact& c);

private:
    std::vector<Contact> m_contacts;
};
