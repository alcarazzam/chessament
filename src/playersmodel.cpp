// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "playersmodel.h"

PlayersModel::PlayersModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int PlayersModel::rowCount(const QModelIndex &) const
{
    return m_players.size();
}

int PlayersModel::columnCount(const QModelIndex &) const
{
    return 10;
}

QVariant PlayersModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    auto player = m_players.at(index.row());

    switch (index.column()) {
    case StartingRankRole:
        return player->startingRank();
    case TitleRole:
        return Player::titleString(player->title());
    case NameRole:
        return player->name();
    case RatingRole:
        return player->rating();
    case NationalRatingRole:
        return player->nationalRating();
    case PlayerIdRole:
        return player->playerId();
    case BirthDateRole:
        return player->birthDate();
    case FederationRole:
        return player->federation();
    case OriginRole:
        return player->origin();
    case SexRole:
        return player->sex();
    }
    return QVariant();
}

QHash<int, QByteArray> PlayersModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

QVariant PlayersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    switch (section) {
    case StartingRankRole:
        return i18nc("@title:column", "No");
    case TitleRole:
        return i18nc("@title:column", "Title");
    case NameRole:
        return i18nc("@title:column", "Name");
    case RatingRole:
        return i18nc("@title:column", "Rating");
    case NationalRatingRole:
        return i18nc("@title:column", "National rating");
    case PlayerIdRole:
        return i18nc("@title:column", "Player ID");
    case BirthDateRole:
        return i18nc("@title:column", "Birth date");
    case FederationRole:
        return i18nc("@title:column", "Federation");
    case OriginRole:
        return i18nc("@title:column", "Origin");
    case SexRole:
        return i18nc("@title:column", "Sex");
    }
    return QVariant();
}

void PlayersModel::setPlayers(QList<Player *> players)
{
    beginResetModel();
    m_players = players;
    endResetModel();
}
