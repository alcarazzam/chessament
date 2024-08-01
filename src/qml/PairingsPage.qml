// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick
import QtQuick.Controls as QQC2

import org.kde.kitemmodels
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.tableview as Tables

import org.kde.chessament
import org.kde.chessament.PlayersModel

Kirigami.ScrollablePage {
    id: root

    QQC2.Label {
        text: "Pairings"
    }
}