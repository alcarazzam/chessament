// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

#include "tournamentstate.h"

TournamentState::TournamentState(Tournament *tournament, int maxRound)
{
    m_tournament = tournament;
    m_maxRound = maxRound;
    m_pairingsByPlayer = m_tournament->getPairingsByPlayer(m_maxRound);
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

uint TournamentState::getPointsForTiebreaks(Player *player)
{
    uint points = 0;
    bool hadVUR = false;
    const auto pairings = m_pairingsByPlayer.value(player);
    for (auto pairing = pairings.rbegin(), rend = pairings.rend(); pairing != rend; ++pairing) {
        if (Pairing::isUnplayed((*pairing)->result())) {
            if (Pairing::isRequestedBye((*pairing)->result()) && (hadVUR || pairing == pairings.rbegin())) {
                points += 5;
                hadVUR = true;
            } else {
                points += (*pairing)->getPointsOfPlayer(player);
                hadVUR = ((*pairing)->result() == Pairing::Result::BothForfeit)
                    || ((*pairing)->result() == Pairing::Result::WhiteWinsForfeit && (*pairing)->blackPlayer() == player)
                    || ((*pairing)->result() == Pairing::Result::BlackWinsForfeit && (*pairing)->whitePlayer() == player);
            }
        } else {
            points += (*pairing)->getPointsOfPlayer(player);
            hadVUR = false;
        }
    }
    return points;
}
