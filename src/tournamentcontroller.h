// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>

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

    Q_PROPERTY(PlayersModel *playersModel READ playersModel CONSTANT)

    Q_PROPERTY(QString currentView READ currentView WRITE setCurrentView NOTIFY currentViewChanged)

public:
    explicit TournamentController(QObject *parent = nullptr);

    Tournament* tournament() const;
    QString tournamentPath() const;
    bool hasOpenTournament();
    int currentPlayerIndex();
    Player *currentPlayer() const;

    Q_INVOKABLE void importTrf(const QUrl &fileUrl);
    Q_INVOKABLE void addPlayer(const QString &title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &origin, const QString &sex);
    Q_INVOKABLE void savePlayer();

    PlayersModel *playersModel() const;

    QString currentView() const;

public Q_SLOTS:
    void setTournament(Tournament *tournament);
    void setTournamentPath(const QString &tournamentPath);
    void setHasOpenTournament(bool hasOpenTournament);
    void setCurrentPlayer(Player *currentPlayer);
    void setCurrentPlayerByIndex(int currentPlayer);
    void setPlayers(QList<Player *> players);
    void setCurrentView(const QString &currentView);

Q_SIGNALS:
    void tournamentChanged();
    void tournamentPathChanged();
    void hasOpenTournamentChanged();
    void currentPlayerChanged();
    void currentViewChanged();

private:
    QString m_tournamentPath;
    Tournament *m_tournament;
    bool m_hasOpenTournament = false;
    int m_currentPlayerIndex = -1;
    Player *m_currentPlayer = nullptr;
    QList<Player *> m_players;
    PlayersModel *m_playersModel;
    QString m_currentView;
};
