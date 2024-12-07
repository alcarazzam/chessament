// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

#pragma once

#include "tiebreaks.h"
#include "tournamentstate.h"

double Points::calculate(Tournament *tournament, TournamentState state, QList<Player *> players, Player *player)
{
    Q_UNUSED(tournament)
    Q_UNUSED(players)

    return state.getPoints(player);
}

double Buchholz::calculate(Tournament *tournament, TournamentState state, QList<Player *> players, Player *player)
{
    Q_UNUSED(tournament)
    Q_UNUSED(players)

    double result = 0.;
    for (const auto &pairing : state.getPairings(player)) {
        double p;
        Player *opponent;

        if (pairing->whitePlayer() == player) {
            opponent = pairing->blackPlayer();
        } else {
            opponent = pairing->whitePlayer();
        }

        // Handle unplayed rounds of player
        if (Pairing::isUnplayed(pairing->result())) {
            p = state.getPoints(player) / 10.;
        } else {
            p = state.getPoints(opponent) / 10.;
        }

        // TODO: handle unplayed rounds of opponents

        result += p;
    }
    return result;
};
