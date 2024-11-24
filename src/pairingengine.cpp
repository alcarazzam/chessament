// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pairingengine.h"
#include "pairing.h"

PairingEngine::PairingEngine()
    : QObject()
{
}

QCoro::Task<std::expected<QList<Pairing *>, QString>> PairingEngine::pair(int round, Tournament *tournament)
{
    Q_UNUSED(round)
    QProcess process;
    auto proc = qCoro(process);

    QTemporaryFile file;
    file.open();

    // TODO: initial color
    // TODO: round number
    auto trf = tournament->toTrf(Tournament::TrfOption::NumberOfRounds | Tournament::TrfOption::InitialColorWhite);
    file.write(trf.toUtf8());
    file.flush();

    co_await proc.start(QStringLiteral("bbpPairings.exe"), {QStringLiteral("--dutch"), file.fileName(), QStringLiteral("-p")});
    co_await proc.waitForFinished();

    if (process.exitCode() != 0) {
        co_return std::unexpected(QStringLiteral("Non zero exit code"));
    }

    QList<Pairing *> pairings;
    const auto players = tournament->getPlayersByStartingRank();

    const auto output = QString::fromUtf8(process.readAll());
    const auto lines = output.split(u'\n');

    uint board = 1;
    for (const auto &line : lines.mid(1)) {
        if (line.isEmpty()) {
            continue;
        }
        const auto playerIds = line.split(u' ');

        if (playerIds.size() != 2) {
            co_return std::unexpected(QStringLiteral("Invalid pairing: ") + line);
        }

        // TODO: check for multiple byes
        const auto whiteId = playerIds[0].toUInt();
        const auto blackId = playerIds[1].toUInt();

        Pairing::Result result = Pairing::Result::PairingBye;

        const auto white = players[whiteId];
        Player *black = nullptr;

        if (blackId != 0) {
            black = players[blackId];
            result = Pairing::Result::Unknown;
        }

        pairings << new Pairing(board++, white, black, result);
    }

    co_return pairings;
}
