// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QString>

#include "player.h"

class Pairing : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(int board READ board NOTIFY boardChanged)
    Q_PROPERTY(Player *whitePlayer READ whitePlayer NOTIFY whitePlayerChanged)
    Q_PROPERTY(Player *blackPlayer READ blackPlayer NOTIFY blackPlayerChanged)
    Q_PROPERTY(Result result READ result NOTIFY resultChanged)

public:
    enum class Color { Unknown, White, Black };
    Q_ENUM(Color)

    static QString colorString(Color color)
    {
        switch (color) {
        case Color::White:
            return QStringLiteral("w");
        case Color::Black:
            return QStringLiteral("b");
        case Color::Unknown:
            return {};
        }
    }

    static Color colorForString(const QString &color)
    {
        auto c = color.toLower();
        if (c == QStringLiteral("w")) {
            return Color::White;
        }
        if (c == QStringLiteral("b")) {
            return Color::Black;
        }
        return Color::Unknown;
    }

    enum class Result {
        Unknown,
        // Normal results
        WhiteWins,
        BlackWins,
        Draw,
        BothLost,
        // Forfeit results
        WhiteWinsForfeit,
        BlackWinsForfeit,
        BothForfeit,
        // Unusual results
        WhiteDraws,
        BlackDraws,
        // Unrated
        WhiteWinsUnrated,
        BlackWinsUnrated,
        DrawUnrated,
        // Byes
        HalfBye,
        FullBye,
        ZeroBye,
        PairingBye,
    };
    Q_ENUM(Result)

    static QString resultString(Result result)
    {
        switch (result) {
        case Result::WhiteWins:
            return QStringLiteral("1-0");
        case Result::BlackWins:
            return QStringLiteral("0-1");
        case Result::Draw:
            return QStringLiteral("½-½");
        case Result::BothLost:
            return QStringLiteral("0-0");
        case Result::WhiteWinsForfeit:
            return QStringLiteral("+/-");
        case Result::BlackWinsForfeit:
            return QStringLiteral("-/+");
        case Result::BothForfeit:
            return QStringLiteral("-/-");
        case Result::WhiteDraws:
            return QStringLiteral("½/0");
        case Result::BlackDraws:
            return QStringLiteral("0/½");
        case Result::WhiteWinsUnrated:
            return QStringLiteral("1U-0U");
        case Result::BlackWinsUnrated:
            return QStringLiteral("0U-1U");
        case Result::DrawUnrated:
            return QStringLiteral("½U-½U");
        case Result::HalfBye:
            return QStringLiteral("½");
        case Result::FullBye:
            return QStringLiteral("1");
        case Result::ZeroBye:
            return QStringLiteral("0");
        case Result::PairingBye:
            return QStringLiteral("Bye");
        default:
            return QStringLiteral("Unknown");
        }
    }

    static QString whiteResultString(Result result)
    {
        switch (result) {
        case Result::WhiteWins:
            return QStringLiteral("1");
        case Result::BlackWins:
            return QStringLiteral("0");
        case Result::Draw:
            return QStringLiteral("=");
        case Result::BothLost:
            return QStringLiteral("0");
        case Result::WhiteWinsForfeit:
            return QStringLiteral("+");
        case Result::BlackWinsForfeit:
            return QStringLiteral("-");
        case Result::BothForfeit:
            return QStringLiteral("-");
        case Result::WhiteDraws:
            return QStringLiteral("=");
        case Result::BlackDraws:
            return QStringLiteral("0");
        case Result::WhiteWinsUnrated:
            return QStringLiteral("W");
        case Result::BlackWinsUnrated:
            return QStringLiteral("L");
        case Result::DrawUnrated:
            return QStringLiteral("D");
        case Result::HalfBye:
            return QStringLiteral("H");
        case Result::FullBye:
            return QStringLiteral("F");
        case Result::ZeroBye:
            return QStringLiteral("Z");
        case Result::PairingBye:
            return QStringLiteral("U");
        default:
            return QStringLiteral("X");
        }
    }

    static QString blackResultString(Result result)
    {
        switch (result) {
        case Result::WhiteWins:
            return QStringLiteral("0");
        case Result::BlackWins:
            return QStringLiteral("1");
        case Result::Draw:
            return QStringLiteral("=");
        case Result::BothLost:
            return QStringLiteral("0");
        case Result::WhiteWinsForfeit:
            return QStringLiteral("-");
        case Result::BlackWinsForfeit:
            return QStringLiteral("+");
        case Result::BothForfeit:
            return QStringLiteral("-");
        case Result::WhiteDraws:
            return QStringLiteral("0");
        case Result::BlackDraws:
            return QStringLiteral("=");
        case Result::WhiteWinsUnrated:
            return QStringLiteral("L");
        case Result::BlackWinsUnrated:
            return QStringLiteral("W");
        case Result::DrawUnrated:
            return QStringLiteral("D");
        case Result::HalfBye:
            return QStringLiteral("H");
        case Result::FullBye:
            return QStringLiteral("F");
        case Result::ZeroBye:
            return QStringLiteral("Z");
        case Result::PairingBye:
            return QStringLiteral("U");
        default:
            return QStringLiteral("X");
        }
    }

    static int whitePointsForResult(Result result)
    {
        switch (result) {
        case Result::WhiteWins:
            return 10;
        case Result::BlackWins:
            return 0;
        case Result::Draw:
            return 5;
        case Result::BothLost:
            return 0;
        case Result::WhiteWinsForfeit:
            return 10;
        case Result::BlackWinsForfeit:
            return 0;
        case Result::BothForfeit:
            return 0;
        case Result::WhiteDraws:
            return 5;
        case Result::BlackDraws:
            return 5;
        case Result::WhiteWinsUnrated:
            return 10;
        case Result::BlackWinsUnrated:
            return 0;
        case Result::DrawUnrated:
            return 5;
        case Result::HalfBye:
            return 5;
        case Result::FullBye:
            return 10;
        case Result::ZeroBye:
            return 0;
        case Result::PairingBye:
            return 10; // TODO
        default:
            return 0;
        }
    }

    enum class PartialResult {
        Unknown,
        // Normal results
        Win,
        Lost,
        Draw,
        // Forfeit results
        WinForfeit,
        LostForfeit,
        // Unrated
        WinUnrated,
        LostUnrated,
        DrawUnrated,
        // Byes
        HalfBye,
        FullBye,
        ZeroBye,
        PairingBye,
    };
    Q_ENUM(PartialResult)

    static PartialResult partialResultForString(const QString &partialResult)
    {
        if (partialResult == QStringLiteral("1")) {
            return PartialResult::Win;
        }
        if (partialResult == QStringLiteral("0")) {
            return PartialResult::Lost;
        }
        if (partialResult == QStringLiteral("=")) {
            return PartialResult::Draw;
        }
        if (partialResult == QStringLiteral("+")) {
            return PartialResult::WinForfeit;
        }
        if (partialResult == QStringLiteral("-")) {
            return PartialResult::LostForfeit;
        }
        if (partialResult == QStringLiteral("W")) {
            return PartialResult::WinUnrated;
        }
        if (partialResult == QStringLiteral("L")) {
            return PartialResult::LostUnrated;
        }
        if (partialResult == QStringLiteral("D")) {
            return PartialResult::DrawUnrated;
        }
        if (partialResult == QStringLiteral("H")) {
            return PartialResult::HalfBye;
        }
        if (partialResult == QStringLiteral("F")) {
            return PartialResult::FullBye;
        }
        if (partialResult == QStringLiteral("Z")) {
            return PartialResult::ZeroBye;
        }
        if (partialResult == QStringLiteral("U")) {
            return PartialResult::PairingBye;
        }
        return PartialResult::Unknown;
    }

    static Result resultFromPartialResults(PartialResult white, PartialResult black)
    {
        if (white == PartialResult::Win && black == PartialResult::Lost) {
            return Result::WhiteWins;
        }
        if (white == PartialResult::Lost && black == PartialResult::Win) {
            return Result::BlackWins;
        }
        if (white == PartialResult::Draw && black == PartialResult::Draw) {
            return Result::Draw;
        }
        if (white == PartialResult::Lost && black == PartialResult::Lost) {
            return Result::BothLost;
        }
        if (white == PartialResult::WinForfeit && black == PartialResult::LostForfeit) {
            return Result::WhiteWinsForfeit;
        }
        if (white == PartialResult::LostForfeit && black == PartialResult::WinForfeit) {
            return Result::BlackWinsForfeit;
        }
        if (white == PartialResult::LostForfeit && black == PartialResult::WinForfeit) {
            return Result::BothForfeit;
        }
        if (white == PartialResult::Draw && black == PartialResult::Lost) {
            return Result::WhiteDraws;
        }
        if (white == PartialResult::Lost && black == PartialResult::Draw) {
            return Result::BlackDraws;
        }
        if (white == PartialResult::WinUnrated && black == PartialResult::LostUnrated) {
            return Result::WhiteWinsUnrated;
        }
        if (white == PartialResult::LostUnrated && black == PartialResult::WinUnrated) {
            return Result::BlackWinsUnrated;
        }
        if (white == PartialResult::DrawUnrated && black == PartialResult::DrawUnrated) {
            return Result::DrawUnrated;
        }
        if (white == PartialResult::HalfBye && black == PartialResult::Unknown) {
            return Result::HalfBye;
        }
        if (white == PartialResult::FullBye && black == PartialResult::Unknown) {
            return Result::FullBye;
        }
        if (white == PartialResult::ZeroBye && black == PartialResult::Unknown) {
            return Result::ZeroBye;
        }
        if (white == PartialResult::PairingBye && black == PartialResult::Unknown) {
            return Result::PairingBye;
        }
        return Result::Unknown;
    }

    explicit Pairing(int board, Player *whitePlayer, Player *blackPlayer, Result result);

    int board();
    Player *whitePlayer();
    Player *blackPlayer();
    Result result();

public Q_SLOTS:
    void setBoard(int board);
    void setWhitePlayer(Player *whitePlayer);
    void setBlackPlayer(Player *blackPlayer);
    void setResult(Result result);

Q_SIGNALS:
    void boardChanged();
    void whitePlayerChanged();
    void blackPlayerChanged();
    void resultChanged();

private:
    int m_board;
    Player *m_whitePlayer;
    Player *m_blackPlayer;
    Result m_result;
};