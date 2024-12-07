// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject(parent)
    , m_playersModel(new PlayersModel(this))
    , m_pairingModel(new PairingModel(this))
    , m_standingsModel(new StandingsModel(this))
{
}

Tournament *Controller::tournament() const
{
    return m_tournament;
}

void Controller::setTournament(Tournament *tournament)
{
    if (m_tournament == tournament) {
        return;
    }
    m_tournament = tournament;

    m_playersModel->setPlayers(m_tournament->players());
    m_pairingModel->setPairings(m_tournament->getPairings(1));
    m_standingsModel->setTournament(m_tournament);

    setHasOpenTournament(true);
    setCurrentPlayerByIndex(-1);

    Q_EMIT tournamentChanged();
}

QString Controller::tournamentPath() const
{
    return m_tournamentPath;
}

void Controller::setTournamentPath(const QString &tournamentPath)
{
    if (m_tournamentPath == tournamentPath) {
        return;
    }
    m_tournamentPath = tournamentPath;
    Q_EMIT tournamentPathChanged();
}

bool Controller::hasOpenTournament()
{
    return m_hasOpenTournament;
}

void Controller::setHasOpenTournament(bool hasOpenTournament)
{
    if (m_hasOpenTournament == hasOpenTournament) {
        return;
    }
    m_hasOpenTournament = hasOpenTournament;
    Q_EMIT hasOpenTournamentChanged();
}

int Controller::currentPlayerIndex()
{
    return m_currentPlayerIndex;
}

void Controller::setCurrentPlayerByIndex(int currentPlayerIndex)
{
    if (m_currentPlayerIndex == currentPlayerIndex) {
        return;
    }
    m_currentPlayerIndex = currentPlayerIndex;
    if (m_currentPlayerIndex >= 0) {
        m_currentPlayer = m_tournament->players()->at(m_currentPlayerIndex);
    } else {
        m_currentPlayer = nullptr;
    }
    Q_EMIT currentPlayerChanged();
}

Player *Controller::currentPlayer() const
{
    return m_currentPlayer;
}

void Controller::setCurrentPlayer(Player *currentPlayer)
{
    if (m_currentPlayer == currentPlayer) {
        return;
    }
    m_currentPlayer = currentPlayer;
    m_currentPlayerIndex = m_tournament->players()->indexOf(m_currentPlayer);
    Q_EMIT currentPlayerChanged();
}

int Controller::currentRound()
{
    return m_currentRound;
}

void Controller::setCurrentRound(int currentRound)
{
    if (m_currentRound == currentRound) {
        return;
    }
    m_currentRound = currentRound;
    m_pairingModel->setPairings(m_tournament->getPairings(currentRound));
    Q_EMIT currentRoundChanged();
}

void Controller::importTrf(const QUrl &fileUrl)
{
    auto tournament = new Tournament();
    auto error = tournament->loadTrf(fileUrl.toLocalFile());

    if (error.has_value()) {
        setTournament(tournament);
        setTournamentPath({});

        setCurrentView(QStringLiteral("players"));
    } else {
        setError(error.error());
    }
}

void Controller::exportTrf(const QUrl &fileUrl)
{
    if (!m_tournament->exportTrf(fileUrl.toLocalFile())) {
        setError(i18n("Couldn't export tournament."));
    }
}

QCoro::Task<void> Controller::pairRound()
{
    const auto pairings = co_await m_tournament->pairRound(m_tournament->currentRound() + 1);

    if (!pairings.has_value()) {
        setError(pairings.error());
    }
}

QString Controller::getPlayersListDocument()
{
    return m_tournament->getPlayersListDocument();
}

void Controller::addPlayer(const QString &title,
                           const QString &name,
                           int rating,
                           int nationalRating,
                           const QString &playerId,
                           const QString &birthDate,
                           const QString &origin,
                           const QString &sex)
{
    auto startingRank = m_tournament->players()->size() + 1;
    auto player = new Player(startingRank, Player::titleForString(title), name, rating, nationalRating, playerId, birthDate, {}, origin, sex);

    m_tournament->addPlayer(player);
    m_playersModel->addPlayer(player);
}

void Controller::savePlayer()
{
    m_playersModel->updatePlayer(m_currentPlayerIndex, m_currentPlayer);
}

void Controller::newTournament(const QUrl &fileUrl, const QString &name, int numberOfRounds)
{
    auto tournament = new Tournament();
    tournament->setName(name);
    tournament->setNumberOfRounds(numberOfRounds);

    setTournament(tournament);
    setTournamentPath(fileUrl.toLocalFile());
    setCurrentView(QStringLiteral("players"));

    saveTournament();
}

void Controller::openTournament(const QUrl &fileUrl)
{
    auto tournament = new Tournament();
    tournament->loadTournament(fileUrl.toLocalFile());

    setTournament(tournament);
    setTournamentPath(fileUrl.toLocalFile());
    setCurrentView(QStringLiteral("players"));
}

void Controller::saveTournament()
{
    if (!m_tournamentPath.isEmpty()) {
        m_tournament->save(m_tournamentPath);
    } else {
        qWarning() << "Trying to save tournament without one open";
    }
}

void Controller::saveTournamentAs(const QUrl &fileUrl)
{
    if (m_tournament->save(fileUrl.toLocalFile())) {
        setTournamentPath(fileUrl.toLocalFile());
    }
}

PlayersModel *Controller::playersModel() const
{
    return m_playersModel;
}

PairingModel *Controller::pairingModel() const
{
    return m_pairingModel;
}

StandingsModel *Controller::standingsModel() const
{
    return m_standingsModel;
}

QString Controller::currentView() const
{
    return m_currentView;
}

void Controller::setCurrentView(const QString &currentView)
{
    if (m_currentView == currentView) {
        return;
    }
    m_currentView = currentView;
    Q_EMIT currentViewChanged();
}

QString Controller::error() const
{
    return m_error;
}

void Controller::setError(const QString &error)
{
    m_error = error;
    Q_EMIT errorChanged();
}
