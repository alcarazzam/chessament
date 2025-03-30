// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QObject>
#include <QString>
#include <QTest>

#include "tournament.h"

using namespace Qt::Literals::StringLiterals;

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
    QCOMPARE(t->name(), u""_s);
}

void TournamentTest::testToJson()
{
    auto t = new Tournament();
    t->setName(u"Test tournament"_s);

    auto json = t->toJson();

    QCOMPARE(json[u"tournament"_s].toObject()[u"name"_s], u"Test tournament"_s);
}

void TournamentTest::testTrf()
{
    auto t = new Tournament();
    t->setName(u"Test tournament"_s);

    auto trf = t->toTrf();

    QVERIFY(trf.contains(u"012 Test tournament"_s));

    t = new Tournament();
    auto ok = t->readTrf(QTextStream(&trf));

    QVERIFY(ok.has_value() && ok);
}

QTEST_GUILESS_MAIN(TournamentTest)
#include "tournamenttest.moc"
