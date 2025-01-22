// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2
import QtQml.Models as Models

import org.kde.kirigami as Kirigami

import dev.alcarazzam.chessament
import dev.alcarazzam.chessament.StandingsModel

Kirigami.Page {
    id: root

    padding: 0

    QQC2.HorizontalHeaderView {
        id: header
        syncView: tableView
        selectionModel: Models.ItemSelectionModel {}

        anchors {
            left: parent.left
            top: parent.top
        }
    }

    QQC2.ScrollView {
        clip: true

        anchors {
            fill: parent
            topMargin: header.height
        }

        TableView {
            id: tableView
            model: Controller.standingsModel

            alternatingRows: true

            selectionBehavior: TableView.SelectRows
            selectionModel: Models.ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            interactive: true

            rowHeightProvider: () => Kirigami.Units.gridUnit * 2
            columnWidthProvider: column => {
                let explicitWidth = explicitColumnWidth(column);
                if (explicitWidth > 0) {
                    return explicitWidth;
                }
                const columnWidths = [];
                columnWidths[0] = 60;
                columnWidths[1] = 60;
                columnWidths[2] = 70;
                columnWidths[3] = 300;
                return column > 3 ? 70 : columnWidths[column];
            }

            delegate: QQC2.ItemDelegate {
                required property var model
                required property int row
                required property int column
                required property bool current
                required property bool selected

                text: model.display
                highlighted: selected || current

                onClicked: {
                    tableView.selectionModel.clearSelection();
                    for (let c = 0; c <= 5; c++) {
                        tableView.selectionModel.select(tableView.model.index(row, c), ItemSelectionModel.Select);
                    }
                }
            }
        }
    }
}
