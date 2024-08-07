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
    QMap<int, Player *> players = {};
    QList<Round *> rounds = {};
    QMap<std::tuple<int, int, int>, std::pair<Pairing::PartialResult, Pairing::PartialResult>> pairingsToAdd = {};

    while (!fileStream.atEnd()) {
        auto line = fileStream.readLine();

        if (line.isEmpty()) {
            continue;
        }

        auto fieldType = line.mid(0, 3);
        auto field = Tournament::getReportField(fieldType);

        if (field == Tournament::ReportFields::TournamentNameField) {
            auto name = line.mid(4).trimmed();
            tournament->setName(name);
        } else if (field == Tournament::ReportFields::CityField) {
            auto city = line.mid(4).trimmed();
            tournament->setCity(city);
        } else if (field == Tournament::ReportFields::FederationField) {
            auto federation = line.mid(4).trimmed();
            tournament->setFederation(federation);
        } else if (field == Tournament::ReportFields::ChiefArbiterField) {
            auto chiefArbiter = line.mid(4).trimmed();
            tournament->setChiefArbiter(chiefArbiter);
        } else if (field == Tournament::ReportFields::DeputyChiefArbiterField) {
            auto deputyChiefArbiter = line.mid(4).trimmed();
            tournament->setDeputyChiefArbiter(deputyChiefArbiter);
        } else if (field == Tournament::ReportFields::PlayerField) {
            auto startingRank = line.sliced(4, 4).toInt();
            auto sex = line.sliced(9, 1).trimmed();
            auto title = Player::titleForString(line.sliced(10, 3).trimmed());
            auto name = line.sliced(14, 33).trimmed();
            auto rating = line.sliced(48, 4).toInt();
            auto federation = line.sliced(53, 5).trimmed();
            auto playerId = line.sliced(57, 11).trimmed();
            auto birthDate = line.sliced(69, 10).trimmed();

            auto player = new Player(startingRank, title, name, rating, 0, playerId, birthDate, federation, {}, sex);
            players[startingRank] = player;

            // Read round
            auto playerRounds = line.mid(91);
            QString round;

            for (int roundNumber = 1;; roundNumber++) {
                round = playerRounds.mid(10 * (roundNumber - 1), 8);
                if (round.isEmpty()) {
                    break;
                }

                auto opponent = round.first(4).toInt();
                auto color = Pairing::colorForString(round.at(5));
                auto result = Pairing::partialResultForString(round.at(7));

                while (rounds.size() < roundNumber) {
                    rounds << new Round();
                }

                std::tuple<int, int, int> pairing{};
                if (color == Pairing::Color::White) {
                    pairing = {roundNumber, startingRank, opponent};
                } else {
                    pairing = {roundNumber, opponent, startingRank};
                }

                auto newPairing = pairingsToAdd[pairing];
                if (color == Pairing::Color::White) {
                    newPairing.first = result;
                } else {
                    newPairing.second = result;
                }
                pairingsToAdd[pairing] = newPairing;
            }
        }
    }

    tournament->setPlayers(players.values());
    tournament->setRounds(rounds);
    tournament->setNumberOfRounds(rounds.size());

    for (auto pairing = pairingsToAdd.cbegin(), end = pairingsToAdd.cend(); pairing != end; ++pairing) {
        const auto [r, w, b] = pairing.key();
        auto whitePlayer = players.value(w);
        auto blackPlayer = players.value(b);
        auto result = Pairing::resultFromPartialResults(pairing.value().first, pairing.value().second);
        auto par = new Pairing(1, whitePlayer, blackPlayer, result);

        tournament->addPairing(r - 1, par);
    }

    return tournament;
}
