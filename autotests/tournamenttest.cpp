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
    void testToJson();
    void testTrf();
};

void TournamentTest::testNewTournament()
{
    auto t = new Tournament();
    QCOMPARE(t->name(), QStringLiteral(""));
}

void TournamentTest::testToJson()
{
    auto t = new Tournament();
    t->setName(QStringLiteral("Test tournament"));

    auto json = t->toJson();

    QCOMPARE(json[QStringLiteral("tournament")].toObject()[QStringLiteral("name")], QStringLiteral("Test tournament"));
}

void TournamentTest::testTrf()
{
    auto t = new Tournament();
    t->setName(QStringLiteral("Test tournament"));

    auto trf = t->toTrf();

    QVERIFY(trf.contains(QStringLiteral("012 Test tournament")));

    t = new Tournament();
    auto ok = t->readTrf(QTextStream(&trf));

    QVERIFY(ok.has_value() && ok);
}

QTEST_GUILESS_MAIN(TournamentTest)
#include "tournamenttest.moc"
