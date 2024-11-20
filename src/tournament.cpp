// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tournament.h"
#include "player.h"

Tournament::Tournament()
    : m_players(new QList<Player *>())
    , m_rounds(QList<Round *>())
{
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
    Q_EMIT currentRoundChanged();
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
    m_rounds.at(round)->addPairing(pairing);
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

bool Tournament::loadTournament(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open tournament" << fileName;
        return false;
    }

    auto data = file.readAll();
    auto document = QJsonDocument::fromJson(data);

    read(document.object());

    return true;
}

bool Tournament::save(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't save tournament" << fileName;
        return false;
    }

    auto tournamentObj = toJson();
    file.write(QJsonDocument(tournamentObj).toJson());

    return true;
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
