// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <AbstractKirigamiApplication>
#include <QFileDialog>
#include <QQmlEngine>

using namespace Qt::StringLiterals;

class ChessamentApplication : public AbstractKirigamiApplication
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit ChessamentApplication(QObject *parent = nullptr);

Q_SIGNALS:
    void newTournament();
    void openTournament();
    void saveTournament();
    void saveTournamentAs();
    void importTrf();
    void exportTrf();

private:
    void setupActions() override;
};
