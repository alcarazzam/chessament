// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QCoroTask>
#include <QFile>
#include <QFlags>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QTextStream>

#include <expected>

#include "pairingengine.h"
#include "player.h"
#include "round.h"
#include "tiebreak.h"

using namespace Qt::StringLiterals;

class Tournament : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString federation READ federation WRITE setFederation NOTIFY federationChanged)
    Q_PROPERTY(QString chiefArbiter READ chiefArbiter WRITE setChiefArbiter NOTIFY chiefArbiterChanged)
    Q_PROPERTY(QString deputyChiefArbiter READ deputyChiefArbiter WRITE setDeputyChiefArbiter NOTIFY deputyChiefArbiterChanged)
    Q_PROPERTY(QString timeControl READ timeControl WRITE setTimeControl NOTIFY timeControlChanged)
    Q_PROPERTY(int numberOfRounds READ numberOfRounds WRITE setNumberOfRounds NOTIFY numberOfRoundsChanged)
    Q_PROPERTY(int currentRound READ currentRound WRITE setCurrentRound NOTIFY currentRoundChanged)
    Q_PROPERTY(QList<Tiebreak *> tiebreaks READ tiebreaks WRITE setTiebreaks NOTIFY tiebreaksChanged)

    Q_PROPERTY(QList<Player *> *players READ players WRITE setPlayers NOTIFY playersChanged)
    Q_PROPERTY(QList<Round *> rounds READ rounds WRITE setRounds NOTIFY roundsChanged)

    explicit Tournament(const QString &fileName = u""_s);
    ~Tournament();

    QString name() const;
    QString city() const;
    QString federation() const;
    QString chiefArbiter() const;
    QString deputyChiefArbiter() const;
    QString timeControl() const;
    int numberOfRounds();
    int currentRound();
    QList<Tiebreak *> tiebreaks();

    QList<Player *> *players();
    void addPlayer(Player *player);
    void savePlayer(Player *player);
    QMap<uint, Player *> getPlayersByStartingRank();
    QMap<uint, Player *> getPlayersById();
    QMap<Player *, QList<Pairing *>> getPairingsByPlayer(int maxRound = -1);
    QList<PlayerTiebreaks> getStandings(uint round = 0);

    QList<Round *> rounds() const;
    void addPairing(int round, Pairing *pairing);
    void savePairing(Pairing *pairing);
    void setResult(Pairing *pairing, Pairing::Result result);
    QList<Pairing *> getPairings(int round) const;
    void sortPairings();
    Q_INVOKABLE bool isRoundFinished(int round);
    QCoro::Task<std::expected<QList<Pairing *>, QString>> calculatePairings(int round);
    QCoro::Task<std::expected<bool, QString>> pairNextRound();

    int numberOfPlayers();
    int numberOfRatedPlayers();

    QString getPlayersListDocument();

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

    enum class TrfOption {
        NumberOfRounds = 1,
        InitialColorWhite,
        InitialColorBlack
    };
    Q_DECLARE_FLAGS(TrfOptions, TrfOption)

    QVariant getOption(const QString &name);
    void setOption(const QString &name, QVariant value);

    QJsonObject toJson() const;
    void read(const QJsonObject &json);
    void saveCopy(const QString &fileName);

    QString toTrf(TrfOptions options = {}, int maxRound = -1);
    std::expected<bool, QString> readTrf(QTextStream trf);
    std::expected<bool, QString> loadTrf(const QString &filename);
    bool exportTrf(const QString &fileName);

public Q_SLOTS:
    void setName(const QString &name);
    void setCity(const QString &city);
    void setFederation(const QString &federation);
    void setChiefArbiter(const QString &chiefArbiter);
    void setDeputyChiefArbiter(const QString &deputyChiefArbiter);
    void setTimeControl(const QString &timeControl);
    void setNumberOfRounds(int numberOfRounds);
    void setCurrentRound(int currentRound);
    void setTiebreaks(QList<Tiebreak *> tiebreaks);

    void setPlayers(QList<Player *> *players);
    void setRounds(QList<Round *> rounds);

Q_SIGNALS:
    void nameChanged();
    void cityChanged();
    void federationChanged();
    void chiefArbiterChanged();
    void deputyChiefArbiterChanged();
    void timeControlChanged();
    void numberOfRoundsChanged();
    void currentRoundChanged();
    void tiebreaksChanged();

    void playersChanged();
    void roundsChanged();

private:
    bool openDatabase(const QString &fileName = u""_s);
    bool loadTournament();
    QSqlDatabase getDB();
    int getDBVersion();
    void setDBVersion(int version);
    void createTables();
    void loadOptions();
    void loadPlayers();
    void loadPairings();

    QString m_connName;

    QString m_name;
    QString m_city;
    QString m_federation;
    QString m_chiefArbiter;
    QString m_deputyChiefArbiter;
    QString m_timeControl;
    int m_numberOfRounds = 1;
    int m_currentRound = 0;
    QList<Tiebreak *> m_tiebreaks;

    QList<Player *> *m_players;
    QList<Round *> m_rounds;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Tournament::TrfOptions)
