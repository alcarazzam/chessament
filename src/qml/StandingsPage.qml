// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
pragma ComponentBehavior: Bound

import QtQuick

import org.kde.kirigami as Kirigami

import dev.alcarazzam.chessament

TablePage {
    id: root

    Kirigami.ColumnView.fillWidth: true

    model: Controller.standingsModel

    selectionBehavior: TableView.SelectRows

    delegate: TableDelegate {
        id: delegate

        required property int index
        required property string displayName
        required property bool editing
        required selected
        required current

        text: displayName
    }
}
