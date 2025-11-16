#include "ContactModel.h"

ContactModel::ContactModel(QObject* parent)
    : QAbstractListModel(parent) {}

int ContactModel::rowCount(const QModelIndex&) const {
    return m_contacts.size();
}

QVariant ContactModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid() || idx.row() >= m_contacts.size())
        return {};

    const Contact& c = m_contacts[idx.row()];

    switch (role) {
        case IndexRole: return c.index;
        case NameRole: return QString(c.name);
        case NumberRole: return QString(c.number);
    }
    return {};
}

QHash<int, QByteArray> ContactModel::roleNames() const {
    return {
        {IndexRole, "index"},
        {NameRole, "name"},
        {NumberRole, "number"}
    };
}

void ContactModel::addContact(const Contact& c) {
    beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
    m_contacts.push_back(c);
    endInsertRows();
}

