// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import dev.alcarazzam.chessament
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root

    required property ChessamentApplication application

    title: i18nc("@title", "Chessament")

    Kirigami.PlaceholderMessage {
        spacing: Kirigami.Units.gridUnit
        width: parent.width - (Kirigami.Units.largeSpacing * 4)
        text: i18nc("@title", "Welcome to Chessament")
        explanation: i18nc("@info", "Chessament is a chess tournament manager")
        helpfulAction: Kirigami.Action {
            fromQAction: root.application.action("file_new")
        }
        anchors {
            centerIn: parent
        }
    }
}
