// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractTableModel>
#include <klocalizedstring.h>

#include "pairing.h"

class PairingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum PairingRoles {
        BoardRole = 0,
        WhiteStartingRankRole,
        WhiteNameRole,
        ResultRole,
        BlackNameRole,
        BlackStartingRankRole,
    };
    Q_ENUM(PairingRoles)

    explicit PairingModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setPairings(QList<Pairing *> pairings);

private:
    QList<Pairing *> m_pairings;
};
