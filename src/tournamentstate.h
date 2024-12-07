// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

#pragma once

#include "tournament.h"

class TournamentState
{
public:
    explicit TournamentState(Tournament *tournament);

    QList<Pairing *> getPairings(Player *player);
    uint getPoints(Player *player);

private:
    Tournament *m_tournament;

    QMap<Player *, QList<Pairing *>> m_pairingsByPlayer;
};
