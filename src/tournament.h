// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QString>

#include "player.h"

class Tournament : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString federation READ federation WRITE setFederation NOTIFY federationChanged)
    Q_PROPERTY(QString chiefArbiter READ chiefArbiter WRITE setChiefArbiter NOTIFY chiefArbiterChanged)
    Q_PROPERTY(QString deputyChiefArbiter READ deputyChiefArbiter WRITE setDeputyChiefArbiter NOTIFY deputyChiefArbiterChanged)

    Q_PROPERTY(QList<Player *> players READ players WRITE setPlayers NOTIFY playersChanged)

    explicit Tournament();

    QString name() const;
    QString city() const;
    QString federation() const;
    QString chiefArbiter() const;
    QString deputyChiefArbiter() const;

    QList<Player *> players() const;

    enum ReportFields {
        PlayerField = 1,
        TournamentNameField,
        CityField,
        FederationField,
        StartDateField,
        EndDateField,
        NumberOfPlayersField,
        NumberOfRatedPlayersField,
        NumberOfTeamsField,
        TournamentTypeField,
        ChiefArbiterField,
        DeputyChiefArbiterField,
        TimeControlField,
        CalendarField,
        Unknown,
    };
    Q_ENUM(ReportFields)

    static ReportFields getReportField(const QString &number);
    

public Q_SLOTS:
    void setName(const QString &name);
    void setCity(const QString &city);
    void setFederation(const QString &federation);
    void setChiefArbiter(const QString &chiefArbiter);
    void setDeputyChiefArbiter(const QString &deputyChiefArbiter);
    
    void setPlayers(QList<Player *> players);

Q_SIGNALS:
    void nameChanged();
    void cityChanged();
    void federationChanged();
    void chiefArbiterChanged();
    void deputyChiefArbiterChanged();

    void playersChanged();

private:
    QString m_name;
    QString m_city;
    QString m_federation;
    QString m_chiefArbiter;
    QString m_deputyChiefArbiter;

    QList<Player *> m_players;
};
