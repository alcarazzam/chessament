// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "round.h"

Round::Round()
    : QObject()
{
}

QList<Pairing *> Round::pairings()
{
    return m_pairings;
}

void Round::setPairings(QList<Pairing *> pairings)
{
    if (m_pairings == pairings) {
        return;
    }
    m_pairings = pairings;
    Q_EMIT pairingsChanged();
}

Pairing *Round::getPairing(int board)
{
    const auto it = std::find_if(m_pairings.begin(), m_pairings.end(), [board](Pairing *p) {
        return p->board() == board;
    });

    Q_ASSERT(it != m_pairings.end());

    return *it;
}

void Round::addPairing(Pairing *pairing)
{
    m_pairings.append(pairing);
}
