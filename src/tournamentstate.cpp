// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

#include "tournamentstate.h"

TournamentState::TournamentState(Tournament *tournament)
{
    m_tournament = tournament;
    m_pairingsByPlayer = m_tournament->getPairingsByPlayer();
}

QList<Pairing *> TournamentState::getPairings(Player *player)
{
    return m_pairingsByPlayer.value(player);
}

uint TournamentState::getPoints(Player *player)
{
    uint points = 0;
    for (const auto &pairing : m_pairingsByPlayer.value(player)) {
        points += pairing->getPointsOfPlayer(player);
    }
    return points;
}
