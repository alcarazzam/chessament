// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournament.h"

std::expected<bool, QString> Tournament::readTrf(QTextStream trf)
{
    QMap<int, Player *> players = {};
    QList<Round *> rounds = {};
    QMap<std::tuple<int, int, int>, std::pair<Pairing::PartialResult, Pairing::PartialResult>> pairingsToAdd = {};

    while (!trf.atEnd()) {
        auto line = trf.readLine();

        if (line.isEmpty()) {
            continue;
        }

        auto fieldType = line.mid(0, 3);
        auto field = Tournament::reportFieldForString(fieldType);

        if (field == Tournament::ReportField::TournamentName) {
            auto name = line.mid(4).trimmed();
            setName(name);
        } else if (field == Tournament::ReportField::City) {
            auto city = line.mid(4).trimmed();
            setCity(city);
        } else if (field == Tournament::ReportField::Federation) {
            auto federation = line.mid(4).trimmed();
            setFederation(federation);
        } else if (field == Tournament::ReportField::ChiefArbiter) {
            auto chiefArbiter = line.mid(4).trimmed();
            setChiefArbiter(chiefArbiter);
        } else if (field == Tournament::ReportField::DeputyChiefArbiter) {
            auto deputyChiefArbiter = line.mid(4).trimmed();
            setDeputyChiefArbiter(deputyChiefArbiter);
        } else if (field == Tournament::ReportField::TimeControl) {
            auto timeControl = line.mid(4).trimmed();
            setTimeControl(timeControl);
        } else if (field == Tournament::ReportField::Player) {
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

                if (round.size() != 8) {
                    return std::unexpected(i18n("Invalid pairing \"%1\"", round));
                }

                bool ok;
                auto opponent = round.first(4).toInt(&ok);
                if (!ok) {
                    return std::unexpected(i18n("Invalid player for pairing \"%1\"", round));
                }

                auto color = Pairing::colorForString(round.at(5));
                auto result = Pairing::partialResultForString(round.at(7));
                if (result == Pairing::PartialResult::Unknown) {
                    return std::unexpected(i18n("Unknown result for pairing \"%1\"", round));
                }

                std::tuple<int, int, int> pairing{};
                if (color == Pairing::Color::White || opponent == 0) {
                    pairing = {roundNumber, startingRank, opponent};
                } else {
                    pairing = {roundNumber, opponent, startingRank};
                }

                auto newPairing = pairingsToAdd[pairing];
                if (color == Pairing::Color::White || opponent == 0) {
                    newPairing.first = result;
                } else {
                    newPairing.second = result;
                }
                pairingsToAdd[pairing] = newPairing;
            }
        }
    }

    setPlayers(new QList(players.values()));
    setRounds(rounds);
    setNumberOfRounds(rounds.size());

    for (auto pairing = pairingsToAdd.cbegin(), end = pairingsToAdd.cend(); pairing != end; ++pairing) {
        const auto [r, w, b] = pairing.key();

        if (!players.contains(w)) {
            return std::unexpected(i18n("Player %1 not found", w));
        }
        auto whitePlayer = players.value(w);

        Player *blackPlayer = nullptr;
        if (b != 0) {
            if (!players.contains(b)) {
                return std::unexpected(i18n("Player %1 not found", b));
            }
            blackPlayer = players.value(b);
        }

        auto result = Pairing::resultFromPartialResults(pairing.value().first, pairing.value().second);
        if (result == Pairing::Result::Unknown) {
            return std::unexpected(i18n("Unknown result on pairing %1 with %2", w, b));
        }
        auto par = new Pairing(1, whitePlayer, blackPlayer, result);

        addPairing(r, par);
    }

    sortPairings();

    return true;
}
