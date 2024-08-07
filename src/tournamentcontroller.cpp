// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournamentcontroller.h"
#include "trf.h"

TournamentController::TournamentController(QObject *parent)
    : QObject(parent)
    , m_playersModel(new PlayersModel(this))
    , m_pairingModel(new PairingModel(this))
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

    setPlayers(m_tournament->players());
    m_pairingModel->setPairings(m_tournament->getPairings(1));
    setHasOpenTournament(true);
    setCurrentPlayerByIndex(-1);

    Q_EMIT tournamentChanged();
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
    if (m_currentPlayerIndex >= 0) {
        m_currentPlayer = m_tournament->players().at(m_currentPlayerIndex);
    } else {
        m_currentPlayer = nullptr;
    }
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
    m_currentPlayerIndex = m_tournament->players().indexOf(m_currentPlayer);
    Q_EMIT currentPlayerChanged();
}

int TournamentController::currentRound()
{
    return m_currentRound;
}

void TournamentController::setCurrentRound(int currentRound)
{
    if (m_currentRound == currentRound) {
        return;
    }
    m_currentRound = currentRound;
    m_pairingModel->setPairings(m_tournament->getPairings(currentRound));
    Q_EMIT currentRoundChanged();
}

void TournamentController::importTrf(const QUrl &fileUrl)
{
    auto tournament = loadTournamentReport(fileUrl);

    if (tournament != nullptr) {
        setTournament(tournament);

        setTournamentPath({});
        setCurrentView(QStringLiteral("players"));
    }
}

void TournamentController::addPlayer(const QString &title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &origin, const QString &sex)
{
    auto startingRank = m_tournament->players().size() + 1;
    auto player = new Player(startingRank, Player::titleForString(title), name, rating, nationalRating, playerId, birthDate, {}, origin, sex);

    m_tournament->addPlayer(player);
    m_playersModel->addPlayer(player);
}

void TournamentController::savePlayer()
{
    m_playersModel->setPlayers(m_tournament->players());
}

void TournamentController::openTournament(const QUrl &fileUrl)
{
    auto tournament = new Tournament();
    tournament->loadTournament(fileUrl.toLocalFile());

    setTournament(tournament);
    setTournamentPath(fileUrl.toLocalFile());
    setCurrentView(QStringLiteral("players"));
}

void TournamentController::saveTournament()
{
    if (!m_tournamentPath.isEmpty()) {
        m_tournament->save(m_tournamentPath);
    } else {
        qWarning() << "Trying to save tournament without one open";
    }
}

void TournamentController::saveTournamentAs(const QUrl &fileUrl)
{
    if (m_tournament->save(fileUrl.toLocalFile())) {
        setTournamentPath(fileUrl.toLocalFile());
    }
}

PlayersModel* TournamentController::playersModel() const
{
    return m_playersModel;
}

PairingModel *TournamentController::pairingModel() const
{
    return m_pairingModel;
}

void TournamentController::setPlayers(QList<Player *> players)
{
    m_playersModel->setPlayers(players);
}

QString TournamentController::currentView() const
{
    return m_currentView;
}

void TournamentController::setCurrentView(const QString &currentView)
{
    if (m_currentView == currentView) {
        return;
    }
    m_currentView = currentView;
    Q_EMIT currentViewChanged();
}
