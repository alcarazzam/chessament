// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QString>
#include <QQmlEngine>

class Player : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(int startingRank READ startingRank NOTIFY startingRankChanged)
    Q_PROPERTY(Title title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int nationalRating READ nationalRating WRITE setNationalRating NOTIFY nationalRatingChanged)
    Q_PROPERTY(QString playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
    Q_PROPERTY(QString birthDate READ birthDate WRITE setBirthDate NOTIFY birthDateChanged)
    Q_PROPERTY(QString federation READ federation WRITE setFederation NOTIFY federationChanged)
    Q_PROPERTY(QString origin READ origin WRITE setOrigin NOTIFY originChanged)
    Q_PROPERTY(QString sex READ sex WRITE setSex NOTIFY sexChanged)

public:

    enum Title { GM, IM, FM, CM, WGM, WIM, WFM, WCM, None };
    Q_ENUM(Title)

    static QString titleString(Title title) {
        switch (title) {
        case Title::GM:
            return QLatin1String("GM");
        case Title::IM:
            return QLatin1String("IM");
        case Title::FM:
            return QLatin1String("FM");
        case Title::CM:
            return QLatin1String("CM");
        case Title::WGM:
            return QLatin1String("WGM");
        case Title::WIM:
            return QLatin1String("WIM");
        case Title::WFM:
            return QLatin1String("WFM");
        case Title::WCM:
            return QLatin1String("WCM");
        default:
            return {};
        }
    };

    static Title titleForString(const QString &title) {
        auto t = title.toLower();
        if (t == QStringLiteral("gm")) {
            return Title::GM;
        }
        if (t == QStringLiteral("im")) {
            return Title::IM;
        }
        if (t == QStringLiteral("fm")) {
            return Title::FM;
        }
        if (t == QStringLiteral("cm")) {
            return Title::CM;
        }
        if (t == QStringLiteral("wgm")) {
            return Title::WGM;
        }
        if (t == QStringLiteral("wim")) {
            return Title::WIM;
        }
        if (t == QStringLiteral("wfm")) {
            return Title::WFM;
        }
        if (t == QStringLiteral("wcm")) {
            return Title::WCM;
        }
        return Title::None;
    }

    explicit Player(int startingRank, const QString &name, int rating);
    explicit Player(int startingRank, Title title, const QString &name, int rating, int nationalRating, const QString &playerId, const QString &birthDate, const QString &federation, const QString &origin, const QString &sex);

    int startingRank();
    Title title() const;
    QString name() const;
    int rating();
    int nationalRating();
    QString playerId() const;
    QString birthDate() const;
    QString federation() const;
    QString origin() const;
    QString sex() const;

    Q_INVOKABLE QString titleString() {
        return Player::titleString(m_title);
    };

public Q_SLOTS:
    void setStartingRank(int startingRank);
    void setTitle(Title title);
    void setTitle(const QString &titleString);
    void setName(const QString &name);
    void setRating(int rating);
    void setNationalRating(int nationalRating);
    void setPlayerId(const QString &playerId);
    void setBirthDate(const QString &birthDate);
    void setFederation(const QString &federation);
    void setOrigin(const QString &origin);
    void setSex(const QString &sex);

Q_SIGNALS:
    void startingRankChanged();
    void titleChanged();
    void nameChanged();
    void ratingChanged();
    void nationalRatingChanged();
    void playerIdChanged();
    void birthDateChanged();
    void federationChanged();
    void originChanged();
    void sexChanged();

private:
    int m_startingRank;
    Title m_title;
    QString m_name;
    int m_rating;
    int m_nationalRating;
    QString m_playerId;
    QString m_birthDate;
    QString m_federation;
    QString m_origin;
    QString m_sex;
};
