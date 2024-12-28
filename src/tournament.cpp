// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournament.h"

#include <KLocalizedString>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <algorithm>

#include "db.h"
#include "tiebreaks.h"
#include "tournamentstate.h"

Tournament::Tournament(const QString &fileName)
    : m_players(new QList<Player *>())
    , m_rounds(QList<Round *>())
{
    m_tiebreaks = {new Points(), new Buchholz()};

    openDatabase(fileName);

    if (!fileName.isEmpty()) {
        loadTournament();
    }
}

Tournament::~Tournament()
{
    qDebug() << "delete tournament";
}

QString Tournament::name() const
{
    return m_name;
}

void Tournament::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
    m_name = name;
    setOption(u"name"_s, name);
    Q_EMIT nameChanged();
}

QString Tournament::city() const
{
    return m_city;
}

void Tournament::setCity(const QString &city)
{
    if (m_city == city) {
        return;
    }
    m_city = city;
    setOption(u"city"_s, city);
    Q_EMIT cityChanged();
}

QString Tournament::federation() const
{
    return m_federation;
}

void Tournament::setFederation(const QString &federation)
{
    if (m_federation == federation) {
        return;
    }
    m_federation = federation;
    setOption(u"federation"_s, federation);
    Q_EMIT federationChanged();
}

QString Tournament::chiefArbiter() const
{
    return m_chiefArbiter;
}

void Tournament::setChiefArbiter(const QString &chiefArbiter)
{
    if (m_chiefArbiter == chiefArbiter) {
        return;
    }
    m_chiefArbiter = chiefArbiter;
    setOption(u"chief_arbiter"_s, chiefArbiter);
    Q_EMIT chiefArbiterChanged();
}

QString Tournament::deputyChiefArbiter() const
{
    return m_deputyChiefArbiter;
}

void Tournament::setDeputyChiefArbiter(const QString &deputyChiefArbiter)
{
    if (m_deputyChiefArbiter == deputyChiefArbiter) {
        return;
    }
    m_deputyChiefArbiter = deputyChiefArbiter;
    setOption(u"deputy_chief_arbiter"_s, deputyChiefArbiter);
    Q_EMIT deputyChiefArbiterChanged();
}

QString Tournament::timeControl() const
{
    return m_timeControl;
}

void Tournament::setTimeControl(const QString &timeControl)
{
    if (m_timeControl == timeControl) {
        return;
    }
    m_timeControl = timeControl;
    setOption(u"time_control"_s, timeControl);
    Q_EMIT timeControlChanged();
}

int Tournament::numberOfRounds()
{
    return m_numberOfRounds;
}

void Tournament::setNumberOfRounds(int numberOfRounds)
{
    if (m_numberOfRounds == numberOfRounds) {
        return;
    }
    m_numberOfRounds = numberOfRounds;
    setOption(u"number_of_rounds"_s, numberOfRounds);
    Q_EMIT numberOfRoundsChanged();
}

int Tournament::currentRound()
{
    return m_currentRound;
}

void Tournament::setCurrentRound(int currentRound)
{
    if (m_currentRound == currentRound) {
        return;
    }
    m_currentRound = currentRound;
    setOption(u"current_round"_s, currentRound);
    Q_EMIT currentRoundChanged();
}

QList<Tiebreak *> Tournament::tiebreaks()
{
    return m_tiebreaks;
}

void Tournament::setTiebreaks(QList<Tiebreak *> tiebreaks)
{
    if (m_tiebreaks == tiebreaks) {
        return;
    }
    m_tiebreaks = tiebreaks;
    Q_EMIT tiebreaksChanged();
}

QList<Player *> *Tournament::players()
{
    return m_players;
}

void Tournament::setPlayers(QList<Player *> *players)
{
    m_players = players;
    Q_EMIT playersChanged();
}

void Tournament::addPlayer(Player *player)
{
    m_players->append(player);

    QSqlQuery query(getDB());
    query.prepare(ADD_PLAYER_QUERY);
    query.bindValue(u":startingRank"_s, player->startingRank());
    query.bindValue(u":title"_s, Player::titleString(player->title()));
    query.bindValue(u":name"_s, player->name());
    query.bindValue(u":rating"_s, player->rating());
    query.bindValue(u":nationalRating"_s, player->nationalRating());
    query.bindValue(u":playerId"_s, player->playerId());
    query.bindValue(u":birthDate"_s, player->birthDate());
    query.bindValue(u":federation"_s, player->federation());
    query.bindValue(u":origin"_s, player->origin());
    query.bindValue(u":sex"_s, player->sex());
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "add player" << *player << query.lastError();
    }

    player->setId(query.lastInsertId().toInt());
}

void Tournament::savePlayer(Player *player)
{
    QSqlQuery query(getDB());
    query.prepare(UPDATE_PLAYER_QUERY);
    query.bindValue(u":id"_s, player->id());
    query.bindValue(u":startingRank"_s, player->startingRank());
    query.bindValue(u":title"_s, Player::titleString(player->title()));
    query.bindValue(u":name"_s, player->name());
    query.bindValue(u":rating"_s, player->rating());
    query.bindValue(u":nationalRating"_s, player->nationalRating());
    query.bindValue(u":playerId"_s, player->playerId());
    query.bindValue(u":birthDate"_s, player->birthDate());
    query.bindValue(u":federation"_s, player->federation());
    query.bindValue(u":origin"_s, player->origin());
    query.bindValue(u":sex"_s, player->sex());
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "save player" << *player << query.lastError();
    }
}

QMap<uint, Player *> Tournament::getPlayersByStartingRank()
{
    QMap<uint, Player *> players;

    for (const auto &player : *m_players) {
        players[player->startingRank()] = player;
    }

    return players;
}

QMap<uint, Player *> Tournament::getPlayersById()
{
    QMap<uint, Player *> players;

    for (const auto &player : *m_players) {
        players[player->id()] = player;
    }

    return players;
}

QMap<Player *, QList<Pairing *>> Tournament::getPairingsByPlayer(uint maxRound)
{
    QMap<Player *, QList<Pairing *>> pairings;

    auto r = maxRound == 0 ? m_rounds.size() : maxRound;
    for (int i = 0; i < r; i++) {
        const auto round = m_rounds.at(i);
        for (const auto &pairing : round->pairings()) {
            pairings[pairing->whitePlayer()] << pairing;
            if (pairing->blackPlayer() != nullptr) {
                pairings[pairing->blackPlayer()] << pairing;
            }
        }
    }

    return pairings;
}

QList<PlayerTiebreaks> Tournament::getStandings(uint round)
{
    Q_UNUSED(round) // TODO: round

    TournamentState state(this);
    QList<PlayerTiebreaks> standings;

    // Sort by tiebreaks
    auto sortStandings = [&standings]() {
        std::sort(standings.begin(), standings.end(), [](PlayerTiebreaks p1, PlayerTiebreaks p2) {
            for (int i = 0; i < p1.second.size(); i++) {
                if (p1.second.at(i) == p2.second.at(i)) {
                    continue;
                }
                return p1.second.at(i) > p2.second.at(i);
            }
            return p1.first->startingRank() < p2.first->startingRank();
        });
    };

    for (auto player = m_players->cbegin(), end = m_players->cend(); player != end; player++) {
        standings << std::make_pair(*player, Tiebreaks{});
    }

    // Calculate tiebreaks
    QList<Player *> players;
    for (const auto &tiebreak : m_tiebreaks) {
        int i = 0;
        players.clear();
        while (i < m_players->size()) {
            if (players.isEmpty()) {
                players << standings.at(i).first;
                i++;
                continue;
            }
            if (standings.at(i - 1).second == standings.at(i).second) {
                players << standings.at(i).first;
                i++;
            } else {
                for (int j = 0; j < players.size(); j++) {
                    /*for (const auto &p : players) {
                        qDebug() << p->name();
                    }
                    qDebug() << "Player" << j << "is" << players.at(j)->name();
                    qDebug() << "SPlayer" << j << "is" << standings.at(i - players.size() + j).first->name();
                    qDebug() << "--";*/
                    standings[i - players.size() + j].second << tiebreak->calculate(this, state, players, players.at(j));
                }
                players.clear();
                players << standings.at(i).first;
                i++;
            }
        }
        for (int j = 0; j < players.size(); j++) {
            /*for (const auto &p : players) {
                qDebug() << p->name();
            }
            qDebug() << "Player" << j << "is" << players.at(j)->name();
            qDebug() << "SPlayer" << j << "is" << standings.at(i - players.size() + j).first->name();
            qDebug() << "--";*/
            standings[i - players.size() + j].second << tiebreak->calculate(this, state, players, players.at(j));
        }
        sortStandings();
    }

    // Print standings for debugging
    /*for (int i = 0; i < standings.size(); i++) {
        const auto s = standings.at(i);
        QString t;
        for (const auto x : s.second) {
            t += QString::number(x);
            t += QStringLiteral(" ");
        }
        qDebug() << i + 1 << s.first->name() << t;
    }*/

    return standings;
}

QList<Round *> Tournament::rounds() const
{
    return m_rounds;
}

void Tournament::setRounds(QList<Round *> rounds)
{
    if (m_rounds == rounds) {
        return;
    }
    m_rounds = rounds;
    Q_EMIT roundsChanged();
}

void Tournament::addPairing(int round, Pairing *pairing)
{
    while (m_rounds.size() < round) {
        m_rounds << new Round();
    }
    m_rounds.at(round - 1)->addPairing(pairing);

    QSqlQuery query(getDB());
    query.prepare(ADD_ROUND_QUERY);
    query.bindValue(u":id"_s, round);
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "add pairing : round" << query.lastError();
    }

    query = QSqlQuery(getDB());
    query.prepare(ADD_PAIRING_QUERY);
    query.bindValue(u":board"_s, pairing->board());
    query.bindValue(u":whitePlayer"_s, pairing->whitePlayer()->id());
    if (pairing->blackPlayer() != nullptr) {
        query.bindValue(u":blackPlayer"_s, pairing->blackPlayer()->id());
    } else {
        query.bindValue(u":blackPlayer"_s, QVariant(QMetaType::fromType<int>()));
    }
    query.bindValue(u":result"_s, std::to_underlying(pairing->result()));
    query.bindValue(u":round"_s, round);
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "add pairing : pairing" << query.lastError();
    }

    pairing->setId(query.lastInsertId().toInt());
}

void Tournament::savePairing(Pairing *pairing)
{
    QSqlQuery query(getDB());
    query.prepare(UPDATE_PAIRING_QUERY);
    query.bindValue(u":id"_s, pairing->id());
    query.bindValue(u":board"_s, pairing->board());
    query.bindValue(u":whitePlayer"_s, pairing->whitePlayer()->id());
    if (pairing->blackPlayer() != nullptr) {
        query.bindValue(u":blackPlayer"_s, pairing->blackPlayer()->id());
    } else {
        query.bindValue(u":blackPlayer"_s, QVariant(QMetaType::fromType<int>()));
    }
    query.bindValue(u":result"_s, std::to_underlying(pairing->result()));
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "save pairing" << query.lastError();
    }
}

QList<Pairing *> Tournament::getPairings(int round) const
{
    if (round <= m_rounds.size()) {
        return m_rounds.at(round - 1)->pairings();
    }
    return {};
}

int Tournament::numberOfPlayers()
{
    return m_players->size();
}

int Tournament::numberOfRatedPlayers()
{
    return std::count_if(m_players->cbegin(), m_players->cend(), [](Player *p) {
        return p->rating() > 0;
    });
}

void Tournament::sortPairings()
{
    for (int i = 0; i < m_rounds.size(); i++) {
        m_rounds.at(i)->sortPairings();

        for (const auto &pairing : m_rounds.at(i)->pairings()) {
            savePairing(pairing);
        }
    }
}

QCoro::Task<std::expected<bool, QString>> Tournament::pairRound(int round)
{
    auto engine = new PairingEngine();
    const auto pairings = co_await engine->pair(5, this);

    if (!pairings.has_value()) {
        co_return std::unexpected(pairings.error());
    }

    for (const auto &pairing : *pairings) {
        addPairing(round, pairing);
    }

    co_return true;
}

QVariant Tournament::getOption(const QString &name)
{
    QSqlQuery query(getDB());
    query.prepare(u"SELECT value FROM options WHERE name = :name LIMIT 1;"_s);
    query.bindValue(u":name"_s, name);
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "get option" << query.lastError();
    }

    query.next();
    if (!query.isValid()) {
        return {};
    }
    return query.value(0);
}

void Tournament::setOption(const QString &name, QVariant value)
{
    QSqlQuery query(getDB());
    query.prepare(u"INSERT OR REPLACE INTO options(name, value) VALUES (:name, :value);"_s);
    query.bindValue(u":name"_s, name);
    query.bindValue(u":value"_s, value);
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "set option" << query.lastError();
    }
}

QJsonObject Tournament::toJson() const
{
    QJsonObject json;

    QJsonObject tournament;
    tournament[QStringLiteral("name")] = m_name;
    tournament[QStringLiteral("city")] = m_city;
    tournament[QStringLiteral("federation")] = m_federation;
    tournament[QStringLiteral("chief_arbiter")] = m_chiefArbiter;
    tournament[QStringLiteral("deputy_chief_arbiter")] = m_deputyChiefArbiter;
    tournament[QStringLiteral("time_control")] = m_timeControl;
    tournament[QStringLiteral("number_of_rounds")] = m_numberOfRounds;

    QJsonArray players;
    for (const auto &player : *m_players) {
        players << player->toJson();
    }

    json[QStringLiteral("tournament")] = tournament;
    json[QStringLiteral("players")] = players;

    return json;
}

void Tournament::read(const QJsonObject &json)
{
    if (auto v = json[QStringLiteral("tournament")]; v.isObject()) {
        auto tournament = v.toObject();

        if (const auto v = tournament[QStringLiteral("name")]; v.isString()) {
            m_name = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("city")]; v.isString()) {
            m_city = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("federation")]; v.isString()) {
            m_federation = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("chief_arbiter")]; v.isString()) {
            m_chiefArbiter = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("deputy_chief_arbiter")]; v.isString()) {
            m_deputyChiefArbiter = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("time_control")]; v.isString()) {
            m_timeControl = v.toString();
        }
        if (const auto v = tournament[QStringLiteral("number_of_rounds")]; v.isDouble()) {
            m_numberOfRounds = v.toInt();
        }
    }

    if (auto v = json[QStringLiteral("players")]; v.isArray()) {
        auto players = v.toArray();
        m_players->clear();
        m_players->reserve(players.size());
        for (const auto &player : players) {
            *m_players << Player::fromJson(player.toObject());
        }
    }
}

QString Tournament::toTrf(TrfOptions options)
{
    QString result;
    QTextStream stream(&result);

    const auto space = QStringLiteral(" ");
    const auto newLine = QLatin1Char('\n');

    stream << reportFieldString(ReportField::TournamentName) << space << m_name << newLine;
    stream << reportFieldString(ReportField::City) << space << m_city << newLine;
    stream << reportFieldString(ReportField::Federation) << space << m_federation << newLine;
    stream << reportFieldString(ReportField::NumberOfPlayers) << space << numberOfPlayers() << newLine;
    stream << reportFieldString(ReportField::NumberOfRatedPlayers) << space << numberOfRatedPlayers() << newLine;
    stream << reportFieldString(ReportField::ChiefArbiter) << space << m_chiefArbiter << newLine;
    stream << reportFieldString(ReportField::TimeControl) << space << m_timeControl << newLine;

    if (options.testAnyFlag(TrfOption::NumberOfRounds)) {
        stream << QStringLiteral("XXR ") + QString::number(m_numberOfRounds) << newLine;
    }

    if (options.testAnyFlag(TrfOption::InitialColorWhite)) {
        stream << QStringLiteral("XXC white1\n");
    } else if (options.testAnyFlag(TrfOption::InitialColorBlack)) {
        stream << QStringLiteral("XXC black1\n");
    }

    for (const auto &player : *m_players) {
        const auto title = Player::titleString(player->title()).toStdString();
        const auto result = std::format("001 {:4} {:1}{:3} {:33} {:4} {:3} {:>11} {:10} {:4.1f} {:4}",
                                        player->startingRank(),
                                        player->sex().toStdString(),
                                        title,
                                        player->name().toStdString(),
                                        player->rating(),
                                        player->federation().toStdString(),
                                        player->playerId().toStdString(),
                                        player->birthDate().toStdString(),
                                        0.f, // TODO: real points
                                        player->startingRank() // TODO: real rank
        );

        stream << result.c_str() << newLine;
    }

    return result;
}

void Tournament::saveCopy(const QString &fileName)
{
    QSqlQuery query(getDB());
    query.prepare(u"VACUUM INTO :fileName;"_s);
    query.bindValue(u":fileName"_s, fileName);
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "save copy" << query.lastError();
    }
}

std::expected<bool, QString> Tournament::loadTrf(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return std::unexpected(i18n("Couldn't open file"));
    }

    return readTrf(QTextStream(&file));
}

bool Tournament::exportTrf(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't export tournament report" << fileName;
        return false;
    }

    auto trf = toTrf();
    file.write(trf.toUtf8());

    return true;
}

bool Tournament::openDatabase(const QString &fileName)
{
    QString dbName;

    if (fileName.isEmpty()) {
        m_connName = QUuid::createUuid().toString(QUuid::WithoutBraces);
        dbName = u":memory:"_s;
    } else {
        m_connName = fileName;
        dbName = fileName;
    }
    qDebug() << m_connName;

    auto db = QSqlDatabase::addDatabase(u"QSQLITE"_s, m_connName);
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "error while opening database";
        return false;
    }

    createTables();

    return true;
}

bool Tournament::loadTournament()
{
    loadOptions();
    loadPlayers();
    loadPairings();

    return true;
}

QSqlDatabase Tournament::getDB()
{
    return QSqlDatabase::database(m_connName);
}

int Tournament::getDBVersion()
{
    QSqlQuery query(u"PRAGMA user_version;"_s, getDB());
    return query.value(0).toInt();
}

void Tournament::setDBVersion(int version)
{
    QSqlQuery query(getDB());
    query.prepare(u"PRAGMA user_version = %1;"_s.arg(version)); // Can't bind in a PRAGMA statement
    query.exec();

    if (query.lastError().isValid()) {
        qDebug() << "set db version" << query.lastError();
    }
}

void Tournament::createTables()
{
    QSqlQuery query(u"PRAGMA foreign_keys = ON;"_s, getDB());

    query = QSqlQuery(getDB());
    query.prepare(OPTIONS_TABLE_SCHEMA);
    query.exec();

    query = QSqlQuery(getDB());
    query.prepare(PLAYERS_TABLE_SCHEMA);
    query.exec();

    query = QSqlQuery(getDB());
    query.prepare(ROUNDS_TABLE_SCHEMA);
    query.exec();

    query = QSqlQuery(getDB());
    query.prepare(PAIRINGS_TABLE_SCHEMA);
    query.exec();

    setDBVersion(1);
}

void Tournament::loadOptions()
{
    setName(getOption(u"name"_s).toString());
    setCity(getOption(u"city"_s).toString());
    setFederation(getOption(u"federation"_s).toString());
    setChiefArbiter(getOption(u"chief_arbiter"_s).toString());
    setDeputyChiefArbiter(getOption(u"deputy_chief_arbiter"_s).toString());
    setTimeControl(getOption(u"time_control"_s).toString());
    setNumberOfRounds(getOption(u"number_of_rounds"_s).toInt());
    setCurrentRound(getOption(u"current_round"_s).toInt());
}

void Tournament::loadPlayers()
{
    m_players->clear();

    QSqlQuery query(GET_PLAYERS_QUERY, getDB());

    int idNo = query.record().indexOf("id");
    int stRankNo = query.record().indexOf("startingRank");
    int titleNo = query.record().indexOf("title");
    int nameNo = query.record().indexOf("name");
    int ratingNo = query.record().indexOf("rating");
    int nationalRatingNo = query.record().indexOf("nationalRating");
    int playerIdNo = query.record().indexOf("playerId");
    int birthDateNo = query.record().indexOf("birthDate");
    int federationNo = query.record().indexOf("federation");
    int originNo = query.record().indexOf("origin");
    int sexNo = query.record().indexOf("sex");

    while (query.next()) {
        auto player = new Player(query.value(stRankNo).toInt(),
                                 Player::titleForString(query.value(titleNo).toString()),
                                 query.value(nameNo).toString(),
                                 query.value(ratingNo).toInt(),
                                 query.value(nationalRatingNo).toInt(),
                                 query.value(playerIdNo).toString(),
                                 query.value(birthDateNo).toString(),
                                 query.value(federationNo).toString(),
                                 query.value(originNo).toString(),
                                 query.value(sexNo).toString());
        player->setId(query.value(idNo).toInt());
        m_players->append(player);
    }
}

void Tournament::loadPairings()
{
    auto players = getPlayersById();

    QSqlQuery query(GET_PAIRINGS_QUERY, getDB());

    int idNo = query.record().indexOf("id");
    int boardNo = query.record().indexOf("board");
    int whitePlayerNo = query.record().indexOf("whitePlayer");
    int blackPlayerNo = query.record().indexOf("blackPlayer");
    int resultNo = query.record().indexOf("result");
    int roundNo = query.record().indexOf("round");

    while (query.next()) {
        auto round = query.value(roundNo).toInt();
        auto pairing = new Pairing(query.value(boardNo).toInt(),
                                   players.value(query.value(whitePlayerNo).toInt()),
                                   players.value(query.value(blackPlayerNo).toInt()),
                                   Pairing::Result(query.value(resultNo).toInt()));
        pairing->setId(query.value(idNo).toInt());
        while (m_rounds.size() < round) {
            m_rounds << new Round();
        }
        m_rounds.at(round - 1)->addPairing(pairing);
    }
}
