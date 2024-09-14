// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFile>
#include <QFlags>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTextStream>

#include <algorithm>
#include <format>

#include "player.h"
#include "round.h"

class Tournament : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString federation READ federation WRITE setFederation NOTIFY federationChanged)
    Q_PROPERTY(QString chiefArbiter READ chiefArbiter WRITE setChiefArbiter NOTIFY chiefArbiterChanged)
    Q_PROPERTY(QString deputyChiefArbiter READ deputyChiefArbiter WRITE setDeputyChiefArbiter NOTIFY deputyChiefArbiterChanged)
    Q_PROPERTY(QString timeControl READ timeControl WRITE setTimeControl NOTIFY timeControlChanged)
    Q_PROPERTY(int numberOfRounds READ numberOfRounds WRITE setNumberOfRounds NOTIFY numberOfRoundsChanged)

    Q_PROPERTY(QList<Player *> players READ players WRITE setPlayers NOTIFY playersChanged)
    Q_PROPERTY(QList<Round *> rounds READ rounds WRITE setRounds NOTIFY roundsChanged)

    explicit Tournament();

    QString name() const;
    QString city() const;
    QString federation() const;
    QString chiefArbiter() const;
    QString deputyChiefArbiter() const;
    QString timeControl() const;
    int numberOfRounds();

    QList<Player *> players() const;
    void addPlayer(Player *player);

    QList<Round *> rounds() const;
    void addPairing(int round, Pairing *pairing);
    QList<Pairing *> getPairings(int round) const;
    void sortPairings();

    int numberOfPlayers();
    int numberOfRatedPlayers();

    enum class ReportField {
        Player = 1,
        TournamentName,
        City,
        Federation,
        StartDate,
        EndDate,
        NumberOfPlayers,
        NumberOfRatedPlayers,
        NumberOfTeams,
        TournamentType,
        ChiefArbiter,
        DeputyChiefArbiter,
        TimeControl,
        Calendar,
        Unknown,
    };
    Q_ENUM(ReportField)

    static QString reportFieldString(ReportField field)
    {
        switch (field) {
        case ReportField::Player:
            return QStringLiteral("001");
        case ReportField::TournamentName:
            return QStringLiteral("012");
        case ReportField::City:
            return QStringLiteral("022");
        case ReportField::Federation:
            return QStringLiteral("032");
        case ReportField::StartDate:
            return QStringLiteral("042");
        case ReportField::EndDate:
            return QStringLiteral("052");
        case ReportField::NumberOfPlayers:
            return QStringLiteral("062");
        case ReportField::NumberOfRatedPlayers:
            return QStringLiteral("072");
        case ReportField::NumberOfTeams:
            return QStringLiteral("082");
        case ReportField::TournamentType:
            return QStringLiteral("092");
        case ReportField::ChiefArbiter:
            return QStringLiteral("102");
        case ReportField::DeputyChiefArbiter:
            return QStringLiteral("112");
        case ReportField::TimeControl:
            return QStringLiteral("122");
        case ReportField::Calendar:
            return QStringLiteral("132");
        default:
            return {};
        }
    };

    static ReportField reportFieldForString(const QString &number)
    {
        if (number == QStringLiteral("001")) {
            return ReportField::Player;
        } else if (number == QStringLiteral("012")) {
            return ReportField::TournamentName;
        } else if (number == QStringLiteral("022")) {
            return ReportField::City;
        } else if (number == QStringLiteral("032")) {
            return ReportField::Federation;
        } else if (number == QStringLiteral("042")) {
            return ReportField::StartDate;
        } else if (number == QStringLiteral("052")) {
            return ReportField::EndDate;
        } else if (number == QStringLiteral("062")) {
            return ReportField::NumberOfPlayers;
        } else if (number == QStringLiteral("072")) {
            return ReportField::NumberOfRatedPlayers;
        } else if (number == QStringLiteral("082")) {
            return ReportField::NumberOfTeams;
        } else if (number == QStringLiteral("092")) {
            return ReportField::TournamentType;
        } else if (number == QStringLiteral("102")) {
            return ReportField::ChiefArbiter;
        } else if (number == QStringLiteral("112")) {
            return ReportField::DeputyChiefArbiter;
        } else if (number == QStringLiteral("122")) {
            return ReportField::TimeControl;
        } else if (number == QStringLiteral("132")) {
            return ReportField::Calendar;
        }
        return ReportField::Unknown;
    }

    enum class TrfOption { NumberOfRounds = 1, InitialColorWhite, InitialColorBlack };
    Q_DECLARE_FLAGS(TrfOptions, TrfOption)

    QJsonObject toJson() const;
    QString toTrf(TrfOptions options = {});
    void read(const QJsonObject &json);
    bool loadTournament(const QString &fileName);
    bool save(const QString &fileName);
    bool exportTrf(const QString &fileName);

public Q_SLOTS:
    void setName(const QString &name);
    void setCity(const QString &city);
    void setFederation(const QString &federation);
    void setChiefArbiter(const QString &chiefArbiter);
    void setDeputyChiefArbiter(const QString &deputyChiefArbiter);
    void setTimeControl(const QString &timeControl);
    void setNumberOfRounds(int numberOfRounds);

    void setPlayers(QList<Player *> players);
    void setRounds(QList<Round *> rounds);

Q_SIGNALS:
    void nameChanged();
    void cityChanged();
    void federationChanged();
    void chiefArbiterChanged();
    void deputyChiefArbiterChanged();
    void timeControlChanged();
    void numberOfRoundsChanged();

    void playersChanged();
    void roundsChanged();

private:
    QString m_name;
    QString m_city;
    QString m_federation;
    QString m_chiefArbiter;
    QString m_deputyChiefArbiter;
    QString m_timeControl;
    int m_numberOfRounds = 1;

    QList<Player *> m_players;
    QList<Round *> m_rounds;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Tournament::TrfOptions)
