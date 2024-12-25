// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QString>

using namespace Qt::StringLiterals;

const QString OPTIONS_TABLE_SCHEMA =
    u"CREATE TABLE IF NOT EXISTS options("_s
    u"name TEXT PRIMARY KEY,"_s
    u"value TEXT"_s
    u");"_s;

const QString PLAYERS_TABLE_SCHEMA =
    u"CREATE TABLE IF NOT EXISTS players("_s
    u"id INTEGER PRIMARY KEY,"_s
    u"startingRank INTEGER,"_s
    u"title TEXT,"_s
    u"name TEXT,"_s
    u"rating INTEGER,"_s
    u"nationalRating INTEGER,"_s
    u"playerId TEXT,"_s
    u"birthDate TEXT,"_s
    u"federation TEXT,"_s
    u"origin TEXT,"_s
    u"sex TEXT"_s
    u");"_s;

const QString ADD_PLAYER_QUERY =
    u"INSERT INTO players(startingRank, title, name, rating, nationalRating, playerId, birthDate, federation, origin, sex) "_s
    u"VALUES (:startingRank, :title, :name, :rating, :nationalRating, :playerId, :birthDate, :federation, :origin, :sex);"_s;

const QString GET_PLAYERS_QUERY = u"SELECT * FROM players;"_s;

const QString UPDATE_PLAYER_QUERY =
    u"UPDATE players SET (startingRank, title, name, rating, nationalRating, playerId, birthDate, federation, origin, sex) = "_s
    u"(:startingRank, :title, :name, :rating, :nationalRating, :playerId, :birthDate, :federation, :origin, :sex) "_s
    u"WHERE id = :id;"_s;

const QString ROUNDS_TABLE_SCHEMA =
    u"CREATE TABLE IF NOT EXISTS rounds("_s
    u"id INTEGER PRIMARY KEY"_s
    u");"_s;

const QString ADD_ROUND_QUERY =
    u"INSERT OR IGNORE INTO rounds(id) "_s
    u"VALUES (:id);"_s;

const QString PAIRINGS_TABLE_SCHEMA =
    u"CREATE TABLE IF NOT EXISTS pairings("_s
    u"id INTEGER PRIMARY KEY,"_s
    u"board INTEGER NOT NULL,"_s
    u"whitePlayer INTEGER NOT NULL,"_s
    u"blackPlayer INTEGER,"_s
    u"result INTEGER NOT NULL,"_s
    u"round INTEGER NOT NULL,"_s
    u"FOREIGN KEY(whitePlayer) REFERENCES players(id),"_s
    u"FOREIGN KEY(blackPlayer) REFERENCES players(id),"_s
    u"FOREIGN KEY(round) REFERENCES rounds(id)"_s
    u");"_s;

const QString ADD_PAIRING_QUERY =
    u"INSERT INTO pairings(board, whitePlayer, blackPlayer, result, round) "_s
    u"VALUES (:board, :whitePlayer, :blackPlayer, :result, :round);"_s;

const QString GET_PAIRINGS_QUERY = u"SELECT * FROM pairings;"_s;

const QString UPDATE_PAIRING_QUERY =
    u"UPDATE pairings SET (board, whitePlayer, blackPlayer, result) = "_s
    u"(:board, :whitePlayer, :blackPlayer, :result) "
    u"WHERE id = :id;"_s;
