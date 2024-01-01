// CustomTableModel.cpp
#include "CustomTableModel.h"

CustomTableModel::CustomTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Set headers
    headers << "Username" << "Site" << "Encrypted Password" << "Decrypted Password";
}

CustomTableModel::~CustomTableModel()
{
}

void CustomTableModel::setTableData(const QList<QList<QString>>& data)
{
    beginResetModel();
    tableData = data;
    endResetModel();
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return tableData.size();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return tableData[index.row()][index.column()];
    }

    return QVariant();
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return headers[section];
    else
        return QString::number(section + 1);  // Row numbers for vertical header
}
