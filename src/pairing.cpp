// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pairing.h"

Pairing::Pairing(int board, Player *whitePlayer, Player *blackPlayer, Result result)
    : QObject()
{
    m_board = board;
    m_whitePlayer = whitePlayer;
    m_blackPlayer = blackPlayer;
    m_result = result;
}

int Pairing::id()
{
    return m_id;
}

void Pairing::setId(int id)
{
    if (m_id == id) {
        return;
    }
    m_id = id;
    Q_EMIT idChanged();
}

int Pairing::board()
{
    return m_board;
}

void Pairing::setBoard(int board)
{
    if (m_board == board) {
        return;
    }
    m_board = board;
    Q_EMIT boardChanged();
}

Player *Pairing::whitePlayer()
{
    return m_whitePlayer;
}

void Pairing::setWhitePlayer(Player *whitePlayer)
{
    if (m_whitePlayer == whitePlayer) {
        return;
    }
    m_whitePlayer = whitePlayer;
    Q_EMIT whitePlayerChanged();
}

Player *Pairing::blackPlayer()
{
    return m_blackPlayer;
}

void Pairing::setBlackPlayer(Player *blackPlayer)
{
    if (m_blackPlayer == blackPlayer) {
        return;
    }
    m_blackPlayer = blackPlayer;
    Q_EMIT blackPlayerChanged();
}

Pairing::Result Pairing::result()
{
    return m_result;
}

void Pairing::setResult(Result result)
{
    if (m_result == result) {
        return;
    }
    m_result = result;
    Q_EMIT resultChanged();
}

bool Pairing::operator<(Pairing other)
{
    return std::min(m_whitePlayer->startingRank(), m_blackPlayer->startingRank())
        < std::min(other.whitePlayer()->startingRank(), other.blackPlayer()->startingRank());
}

QDebug operator<<(QDebug dbg, Pairing &pairing)
{
    dbg.nospace() << "Pairing(" << pairing.board() << "," << *pairing.whitePlayer() << "," << pairing.blackPlayer() << ")";
    return dbg;
}
