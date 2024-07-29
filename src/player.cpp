// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "player.h"

Player::Player(int startingRank, const QString &name, int rating)
    : QObject()
{
    setStartingRank(startingRank);
    setName(name);
    setRating(rating);
    setNationalRating(0);
}

Player::Player(int startingRank, Title title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &federation, const QString &origin, const QString &sex)
    : QObject()
{
    setStartingRank(startingRank);
    setTitle(title);
    setName(name);
    setRating(rating);
    setNationalRating(nationalRating);
    setPlayerId(playerId);
    setBirthDate(birthDate);
    setFederation(federation);
    setOrigin(origin);
    setSex(sex);
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
