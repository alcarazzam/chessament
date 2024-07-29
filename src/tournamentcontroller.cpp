// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournamentcontroller.h"
#include "trf.h"

TournamentController::TournamentController(QObject *parent)
    : QObject(parent)
    , m_players({})
    , m_playersModel(new PlayersModel(this))
{
}

Tournament* TournamentController::tournament() const
{
    return m_tournament;
}

void TournamentController::setTournament(Tournament *tournament)
{
    if (m_tournament == tournament) {
        return;
    }
    m_tournament = tournament;
    Q_EMIT tournamentChanged();
    setHasOpenTournament(true);
}

QString TournamentController::tournamentPath() const
{
    return m_tournamentPath;
}

void TournamentController::setTournamentPath(const QString &tournamentPath)
{
    if (m_tournamentPath == tournamentPath) {
        return;
    }
    m_tournamentPath = tournamentPath;
    Q_EMIT tournamentPathChanged();
}

bool TournamentController::hasOpenTournament()
{
    return m_hasOpenTournament;
}

void TournamentController::setHasOpenTournament(bool hasOpenTournament)
{
    if (m_hasOpenTournament == hasOpenTournament) {
        return;
    }
    m_hasOpenTournament = hasOpenTournament;
    Q_EMIT hasOpenTournamentChanged();
}

int TournamentController::currentPlayerIndex()
{
    return m_currentPlayerIndex;
}

void TournamentController::setCurrentPlayerByIndex(int currentPlayerIndex)
{
    if (m_currentPlayerIndex == currentPlayerIndex) {
        return;
    }
    m_currentPlayerIndex = currentPlayerIndex;
    m_currentPlayer = m_players.at(m_currentPlayerIndex);
    Q_EMIT currentPlayerChanged();
}

Player* TournamentController::currentPlayer() const
{
    return m_currentPlayer;
}

void TournamentController::setCurrentPlayer(Player *currentPlayer)
{
    if (m_currentPlayer == currentPlayer) {
        return;
    }
    m_currentPlayer = currentPlayer;
    m_currentPlayerIndex = m_players.indexOf(m_currentPlayer);
    Q_EMIT currentPlayerChanged();
}

void TournamentController::importTrf(const QUrl &fileUrl)
{
    auto tournament = loadTournamentReport(fileUrl);

    if (tournament != nullptr) {
        setTournament(tournament);
        setPlayers(tournament->players());
    }
}

void TournamentController::addPlayer(const QString &title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &origin, const QString &sex)
{
    auto player = new Player(m_players.size() + 1, Player::titleForString(title), name, rating, nationalRating, playerId, birthDate, {}, origin, sex);

    m_players.append(player);
    m_playersModel->setPlayers(m_players);
}

void TournamentController::savePlayer()
{
    m_playersModel->setPlayers(m_players);
}

PlayersModel* TournamentController::playersModel() const
{
    return m_playersModel;
}

void TournamentController::setPlayers(QList<Player *> players)
{
    m_players = players;
    m_playersModel->setPlayers(m_players);
}
