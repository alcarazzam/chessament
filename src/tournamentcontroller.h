// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

#include "pairingmodel.h"
#include "playersmodel.h"
#include "tournament.h"

class TournamentController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Tournament *tournament READ tournament WRITE setTournament NOTIFY tournamentChanged)
    Q_PROPERTY(QString tournamentPath READ tournamentPath WRITE setTournamentPath NOTIFY tournamentPathChanged)
    Q_PROPERTY(bool hasOpenTournament READ hasOpenTournament WRITE setHasOpenTournament NOTIFY hasOpenTournamentChanged)
    Q_PROPERTY(int currentPlayerIndex READ currentPlayerIndex WRITE setCurrentPlayerByIndex NOTIFY currentPlayerChanged)
    Q_PROPERTY(Player *currentPlayer READ currentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(int currentRound READ currentRound WRITE setCurrentRound NOTIFY currentRoundChanged)

    Q_PROPERTY(PlayersModel *playersModel READ playersModel CONSTANT)
    Q_PROPERTY(PairingModel *pairingModel READ pairingModel CONSTANT)

    Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    explicit TournamentController(QObject *parent = nullptr);

    Tournament* tournament() const;
    QString tournamentPath() const;
    bool hasOpenTournament();
    int currentPlayerIndex();
    Player *currentPlayer() const;
    int currentRound();

    Q_INVOKABLE void addPlayer(const QString &title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &origin, const QString &sex);
    Q_INVOKABLE void savePlayer();
    Q_INVOKABLE void openTournament(const QUrl &fileUrl);
    Q_INVOKABLE void saveTournament();
    Q_INVOKABLE void saveTournamentAs(const QUrl &fileUrl);
    Q_INVOKABLE void importTrf(const QUrl &fileUrl);

    PlayersModel *playersModel() const;
    PairingModel *pairingModel() const;

    QString currentView() const;

public Q_SLOTS:
    void setTournament(Tournament *tournament);
    void setTournamentPath(const QString &tournamentPath);
    void setHasOpenTournament(bool hasOpenTournament);
    void setCurrentPlayer(Player *currentPlayer);
    void setCurrentPlayerByIndex(int currentPlayer);
    void setCurrentRound(int currentRound);
    void setPlayers(QList<Player *> players);
    void setCurrentView(const QString &currentView);

Q_SIGNALS:
    void tournamentChanged();
    void tournamentPathChanged();
    void hasOpenTournamentChanged();
    void currentPlayerChanged();
    void currentRoundChanged();
    void currentViewChanged();

private:
    Tournament *m_tournament;

    QString m_tournamentPath;
    bool m_hasOpenTournament = false;
    int m_currentPlayerIndex = -1;
    int m_currentRound = 1;

    Player *m_currentPlayer = nullptr;
    PlayersModel *m_playersModel;
    PairingModel *m_pairingModel;

    QString m_currentView;
};
