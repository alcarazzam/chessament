// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <algorithm>

#include "pairing.h"

class Round : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QList<Pairing *> pairings READ pairings WRITE setPairings NOTIFY pairingsChanged)

public:
    explicit Round();

    QList<Pairing *> pairings();

    void addPairing(Pairing *pairing);
    void sortPairings();

public Q_SLOTS:
    void setPairings(QList<Pairing *> pairings);

Q_SIGNALS:
    void pairingsChanged();

private:
    QList<Pairing *> m_pairings;
};