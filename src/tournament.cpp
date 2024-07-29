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

QList<Player *> Tournament::players() const
{
    return m_players;
}

void Tournament::setPlayers(QList<Player *> players)
{
    m_players = players;
    Q_EMIT playersChanged();
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
