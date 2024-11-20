// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QObject>
#include <QString>
#include <QTest>

#include "tournament.h"

class TournamentTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testNewTournament();
};

void TournamentTest::testNewTournament()
{
    auto t = new Tournament();
    QCOMPARE(t->name(), QStringLiteral(""));
}

QTEST_GUILESS_MAIN(TournamentTest)
#include "tournamenttest.moc"
