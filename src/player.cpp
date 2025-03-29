// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "player.h"

Player::Player()
    : QObject()
{
}

Player::Player(int startingRank, const QString &name, int rating)
    : QObject()
{
    setStartingRank(startingRank);
    setName(name);
    setRating(rating);
}

Player::Player(int startingRank,
               Title title,
               const QString &name,
               const QString &surname,
               int rating,
               int nationalRating,
               const QString &playerId,
               const QString &birthDate,
               const QString &federation,
               const QString &origin,
               const QString &sex)
    : QObject()
{
    setStartingRank(startingRank);
    setTitle(title);
    setName(name);
    setSurname(surname);
    setRating(rating);
    setNationalRating(nationalRating);
    setPlayerId(playerId);
    setBirthDate(birthDate);
    setFederation(federation);
    setOrigin(origin);
    setSex(sex);
}

int Player::id()
{
    return m_id;
}

void Player::setId(int id)
{
    if (m_id == id) {
        return;
    }
    m_id = id;
    Q_EMIT idChanged();
}

int Player::startingRank()
{
    return m_startingRank;
}

void Player::setStartingRank(int startingRank)
{
    if (m_startingRank == startingRank) {
        return;
    }
    m_startingRank = startingRank;
    Q_EMIT startingRankChanged();
}

Player::Title Player::title() const
{
    return m_title;
}

void Player::setTitle(Title title)
{
    if (m_title == title) {
        return;
    }
    m_title = title;
    Q_EMIT titleChanged();
}

void Player::setTitle(const QString &titleString)
{
    auto title = titleForString(titleString);
    setTitle(title);
}

QString Player::name() const
{
    return m_name;
}

void Player::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
    m_name = name;
    Q_EMIT nameChanged();
}

QString Player::surname() const
{
    return m_surname;
}

void Player::setSurname(const QString &surname)
{
    if (m_surname == surname) {
        return;
    }
    m_surname = surname;
    Q_EMIT surnameChanged();
}

int Player::rating()
{
    return m_rating;
}

void Player::setRating(int rating)
{
    if (m_rating == rating) {
        return;
    }
    m_rating = rating;
    Q_EMIT ratingChanged();
}

int Player::nationalRating()
{
    return m_nationalRating;
}

void Player::setNationalRating(int nationalRating)
{
    if (m_nationalRating == nationalRating) {
        return;
    }
    m_nationalRating = nationalRating;
    Q_EMIT nationalRatingChanged();
}

QString Player::playerId() const
{
    return m_playerId;
}

void Player::setPlayerId(const QString &playerId)
{
    if (m_playerId == playerId) {
        return;
    }
    m_playerId = playerId;
    Q_EMIT playerIdChanged();
}

QString Player::birthDate() const
{
    return m_birthDate;
}

void Player::setBirthDate(const QString &birthDate)
{
    if (m_birthDate == birthDate) {
        return;
    }
    m_birthDate = birthDate;
    Q_EMIT birthDateChanged();
}

QString Player::federation() const
{
    return m_federation;
}

void Player::setFederation(const QString &federation)
{
    if (m_federation == federation) {
        return;
    }
    m_federation = federation;
    Q_EMIT federationChanged();
}

QString Player::origin() const
{
    return m_origin;
}

void Player::setOrigin(const QString &origin)
{
    if (m_origin == origin) {
        return;
    }
    m_origin = origin;
    Q_EMIT originChanged();
}

QString Player::sex() const
{
    return m_sex;
}

void Player::setSex(const QString &sex)
{
    if (m_sex == sex) {
        return;
    }
    m_sex = sex;
    Q_EMIT sexChanged();
}

QString Player::fullName() const
{
    if (!m_surname.isEmpty()) {
        return m_surname + u", "_s + m_name;
    }
    return m_name;
}

QJsonObject Player::toJson() const
{
    QJsonObject json;

    json[QStringLiteral("starting_rank")] = m_startingRank;
    json[QStringLiteral("title")] = Player::titleString(m_title);
    json[QStringLiteral("name")] = m_name;
    json[QStringLiteral("surname")] = m_surname;
    json[QStringLiteral("rating")] = m_rating;
    json[QStringLiteral("national_rating")] = m_nationalRating;
    json[QStringLiteral("player_id")] = m_playerId;
    json[QStringLiteral("birth_date")] = m_birthDate;
    json[QStringLiteral("federation")] = m_federation;
    json[QStringLiteral("origin")] = m_origin;
    json[QStringLiteral("sex")] = m_sex;

    return json;
}

Player *Player::fromJson(const QJsonObject &json)
{
    auto *player = new Player();

    if (const auto v = json[QStringLiteral("starting_rank")]; v.isDouble()) {
        player->m_startingRank = v.toInt();
    }
    if (const auto v = json[QStringLiteral("title")]; v.isString()) {
        player->m_title = Player::titleForString(v.toString());
    }
    if (const auto v = json[QStringLiteral("surname")]; v.isString()) {
        player->m_surname = v.toString();
    }
    if (const auto v = json[QStringLiteral("name")]; v.isString()) {
        player->m_name = v.toString();
    }
    if (const auto v = json[QStringLiteral("rating")]; v.isDouble()) {
        player->m_rating = v.toInt();
    }
    if (const auto v = json[QStringLiteral("national_rating")]; v.isDouble()) {
        player->m_nationalRating = v.toInt();
    }
    if (const auto v = json[QStringLiteral("player_id")]; v.isString()) {
        player->m_playerId = v.toString();
    }
    if (const auto v = json[QStringLiteral("birth_date")]; v.isString()) {
        player->m_birthDate = v.toString();
    }
    if (const auto v = json[QStringLiteral("federation")]; v.isString()) {
        player->m_federation = v.toString();
    }
    if (const auto v = json[QStringLiteral("origin")]; v.isString()) {
        player->m_origin = v.toString();
    }
    if (const auto v = json[QStringLiteral("sex")]; v.isString()) {
        player->m_sex = v.toString();
    }

    return player;
}

QDebug operator<<(QDebug dbg, const Player &player)
{
    dbg.nospace() << "Player(" << player.name() << ")";
    return dbg;
}
