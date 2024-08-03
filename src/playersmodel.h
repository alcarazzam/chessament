// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QAbstractTableModel>
#include <klocalizedstring.h>

#include "player.h"

class PlayersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum PlayerRoles {
        StartingRankRole = 0,
        TitleRole,
        NameRole,
        RatingRole,
        NationalRatingRole,
        PlayerIdRole,
        BirthDateRole,
        FederationRole,
        OriginRole,
        SexRole,
    };
    Q_ENUM(PlayerRoles)

    explicit PlayersModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setPlayers(QList<Player *> players);
    void addPlayer(Player *player);

private:
    QList<Player *> m_players;
};
