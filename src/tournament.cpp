// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournament.h"

Tournament::Tournament()
{
}

QString Tournament::name() const
{
    return m_name;
}

void Tournament::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
    m_name = name;
    Q_EMIT nameChanged();
}

QString Tournament::city() const
{
    return m_city;
}

void Tournament::setCity(const QString &city)
{
    if (m_city == city) {
        return;
    }
    m_city = city;
    Q_EMIT cityChanged();
}

QString Tournament::federation() const
{
    return m_federation;
}

void Tournament::setFederation(const QString &federation)
{
    if (m_federation == federation) {
        return;
    }
    m_federation = federation;
    Q_EMIT federationChanged();
}

QString Tournament::chiefArbiter() const
{
    return m_chiefArbiter;
}

void Tournament::setChiefArbiter(const QString &chiefArbiter)
{
    if (m_chiefArbiter == chiefArbiter) {
        return;
    }
    m_chiefArbiter = chiefArbiter;
    Q_EMIT chiefArbiterChanged();
}

QString Tournament::deputyChiefArbiter() const
{
    return m_deputyChiefArbiter;
}

void Tournament::setDeputyChiefArbiter(const QString &deputyChiefArbiter)
{
    if (m_deputyChiefArbiter == deputyChiefArbiter) {
        return;
    }
    m_deputyChiefArbiter = deputyChiefArbiter;
    Q_EMIT deputyChiefArbiterChanged();
}

int Tournament::numberOfRounds()
{
    return m_numberOfRounds;
}

void Tournament::setNumberOfRounds(int numberOfRounds)
{
    if (m_numberOfRounds == numberOfRounds) {
        return;
    }
    m_numberOfRounds = numberOfRounds;
    Q_EMIT numberOfRoundsChanged();
}

QList<Player *> Tournament::players() const
{
    return m_players;
}

void Tournament::setPlayers(QList<Player *> players)
{
    m_players = players;
    Q_EMIT playersChanged();
}

void Tournament::addPlayer(Player *player)
{
    m_players.append(player);
}

QList<Round *> Tournament::rounds() const
{
    return m_rounds;
}

void Tournament::setRounds(QList<Round *> rounds)
{
    if (m_rounds == rounds) {
        return;
    }
    m_rounds = rounds;
    Q_EMIT roundsChanged();
}

void Tournament::addPairing(int round, Pairing *pairing)
{
    m_rounds.at(round)->addPairing(pairing);
}

QList<Pairing *> Tournament::getPairings(int round) const
{
    if (round <= m_rounds.size()) {
        return m_rounds.at(round - 1)->pairings();
    }
    return {};
}

Tournament::ReportFields Tournament::getReportField(const QString &number)
{
    if (number == QStringLiteral("001")) {
        return PlayerField;
    } else if (number == QStringLiteral("012")) {
        return TournamentNameField;
    } else if (number == QStringLiteral("022")) {
        return CityField;
    } else if (number == QStringLiteral("032")) {
        return FederationField;
    } else if (number == QStringLiteral("042")) {
        return StartDateField;
    } else if (number == QStringLiteral("052")) {
        return EndDateField;
    } else if (number == QStringLiteral("062")) {
        return NumberOfPlayersField;
    } else if (number == QStringLiteral("072")) {
        return NumberOfRatedPlayersField;
    } else if (number == QStringLiteral("082")) {
        return NumberOfTeamsField;
    } else if (number == QStringLiteral("092")) {
        return TournamentTypeField;
    } else if (number == QStringLiteral("102")) {
        return ChiefArbiterField;
    } else if (number == QStringLiteral("112")) {
        return DeputyChiefArbiterField;
    } else if (number == QStringLiteral("122")) {
        return TimeControlField;
    } else if (number == QStringLiteral("132")) {
        return CalendarField;
    } else {
        return Unknown;
    }
}

QJsonObject Tournament::toJson() const
{
    QJsonObject json;

    QJsonObject tournament;
    tournament[QStringLiteral("name")] = m_name;
    tournament[QStringLiteral("city")] = m_city;
    tournament[QStringLiteral("federation")] = m_federation;
    tournament[QStringLiteral("chief_arbiter")] = m_chiefArbiter;
    tournament[QStringLiteral("deputy_chief_arbiter")] = m_deputyChiefArbiter;
    tournament[QStringLiteral("number_of_rounds")] = m_numberOfRounds;

    QJsonArray players;
    for (const auto &player : m_players) {
        players << player->toJson();
    }

    json[QStringLiteral("tournament")] = tournament;
    json[QStringLiteral("players")] = players;

    return json;
}

void Tournament::read(const QJsonObject &json)
{
    if (auto v = json[QStringLiteral("tournament")]; v.isObject()) {
        auto tournament = v.toObject();

        if (const auto v = tournament[QStringLiteral("name")]; v.isString()) {
            m_name = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("city")]; v.isString()) {
            m_city = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("federation")]; v.isString()) {
            m_federation = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("chief_arbiter")]; v.isString()) {
            m_chiefArbiter = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("deputy_chief_arbiter")]; v.isString()) {
            m_deputyChiefArbiter = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("number_of_rounds")]; v.isDouble()) {
            m_numberOfRounds = v.toInt();
        }
    }

    if (auto v = json[QStringLiteral("players")]; v.isArray()) {
        auto players = v.toArray();
        m_players.clear();
        m_players.reserve(players.size());
        for (const auto &player : players) {
            m_players << Player::fromJson(player.toObject());
        }
    }
}

bool Tournament::loadTournament(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open tournament" << fileName;
        return false;
    }

    auto data = file.readAll();
    auto document = QJsonDocument::fromJson(data);

    read(document.object());

    return true;
}

bool Tournament::save(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't save tournament" << fileName;
        return false;
    }

    auto tournamentObj = toJson();
    file.write(QJsonDocument(tournamentObj).toJson());

    return true;
}
