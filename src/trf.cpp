// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "trf.h"

Tournament* loadTournamentReport(const QUrl &fileUrl)
{
    QFile file(fileUrl.toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }

    QTextStream fileStream(&file);
    
    auto tournament = new Tournament();
    QList<Player *> players = {};

    while (!fileStream.atEnd()) {
        auto line = fileStream.readLine();

        if (line.isEmpty()) {
            continue;
        }

        auto fieldType = line.mid(0, 3);
        auto field = Tournament::getReportField(fieldType);

        if (field == Tournament::ReportFields::PlayerField) {
            auto startingRank = line.sliced(4, 4).toInt();
            auto sex = line.sliced(9, 1).trimmed();
            auto title = Player::titleForString(line.sliced(10, 3).trimmed());
            auto name = line.sliced(14, 33).trimmed();
            auto rating = line.sliced(48, 4).toInt();
            auto federation = line.sliced(53, 5).trimmed();
            auto playerId = line.sliced(57, 11).trimmed();
            auto birthDate = line.sliced(69, 10).trimmed();

            auto player = new Player(startingRank, title, name, rating, 0, playerId, birthDate, federation, {}, sex);
            players << player;
        }
    }

    tournament->setPlayers(players);

    return tournament;
}
