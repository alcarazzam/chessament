// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QObject>
#include <QString>
#include <QTest>

#include "event.h"

using namespace Qt::Literals::StringLiterals;

class TournamentTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testNewTournament();
    void testToJson();
    void testTrf();
    void testImportTrf();
};

void TournamentTest::testNewTournament()
{
    auto e = new Event();
    auto t = e->createTournament();
    QCOMPARE(t->name(), u""_s);
}

void TournamentTest::testToJson()
{
    auto e = new Event();
    auto t = e->createTournament();
    t->setName(u"Test tournament"_s);

    auto json = t->toJson();

    QCOMPARE(json[u"tournament"_s].toObject()[u"name"_s], u"Test tournament"_s);
}

void TournamentTest::testTrf()
{
    auto e = new Event();
    auto t = e->createTournament();
    t->setName(u"Test tournament"_s);

    auto trf = t->toTrf();

    QVERIFY(trf.contains(u"012 Test tournament"_s));

    e = new Event();
    t = e->createTournament();
    auto ok = t->readTrf(QTextStream(&trf));

    QVERIFY(ok.has_value() && ok);
}

void TournamentTest::testImportTrf()
{
    auto e = new Event();
    auto tournament = e->importTournament(QLatin1String(DATA_DIR) + u"/tournament_1.txt"_s);

    QVERIFY(tournament.has_value());

    auto t = *tournament;

    QCOMPARE(t->name(), u"Test Tournament"_s);
    QCOMPARE(t->city(), u"Place"_s);
    QCOMPARE(t->federation(), u"ESP"_s);
    QCOMPARE(t->chiefArbiter(), u"Chief Arbiter"_s);
    QCOMPARE(t->deputyChiefArbiter(), u"Arbiter"_s);
    QCOMPARE(t->timeControl(), u"8 min/player + 3 s/move"_s);

    QCOMPARE(t->numberOfPlayers(), 88);
    QCOMPARE(t->numberOfRatedPlayers(), 82);
    QCOMPARE(t->numberOfRounds(), 9);
    QCOMPARE(t->rounds().size(), 9);
}

QTEST_GUILESS_MAIN(TournamentTest)
#include "tournamenttest.moc"
