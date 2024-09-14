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

void Round::addPairing(Pairing *pairing)
{
    m_pairings.append(pairing);
}

void Round::sortPairings()
{
    // TODO: sort pairings in the tournament instead
    std::sort(m_pairings.begin(), m_pairings.end());

    for (int i = 0; i < m_pairings.size(); i++) {
        m_pairings.at(i)->setBoard(i + 1);
    }
}
