// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2

import org.kde.kirigami as Kirigami

import dev.alcarazzam.chessament
import dev.alcarazzam.chessament.PairingModel

TablePage {
    id: root

    Kirigami.ColumnView.fillWidth: true

    model: Controller.pairingModel

    selectionBehavior: TableView.SelectRows

    actions: [
        Kirigami.Action {
            text: i18n("Pair round %1", Controller.tournament.currentRound + 1)
            visible: (Controller.tournament.currentRound + 1 <= Controller.tournament.numberOfRounds) && Controller.tournament.isRoundFinished(Controller.tournament.currentRound)
            onTriggered: Controller.pairRound()
        },
        Kirigami.Action {
            displayComponent: QQC2.ComboBox {
                id: roundSelector
                model: Controller.tournament.numberOfRounds
                currentIndex: Controller.currentRound - 1
                displayText: i18n("Round %1", currentIndex + 1)
                delegate: QQC2.ItemDelegate {
                    required property int index

                    text: index + 1
                    width: roundSelector.width
                    highlighted: roundSelector.highlightedIndex === index
                }
                onActivated: index => {
                    root.tableView.selectionModel.clear();
                    Controller.currentRound = index + 1;
                }
            }
        }
    ]

    delegate: TableDelegate {
        id: delegate

        required property int index
        required property string displayName
        required property bool editing
        required selected
        required current

        text: displayName
    }

    Keys.onPressed: event => {
        let selection = tableView.selectionModel.currentIndex;
        if (selection) {
            let board = selection.row + 1;
            if (board && Controller.setResult(board, event.key)) {
                event.accepted = true;
                root.selectBoard((board % root.model.rowCount()) + 1);
            }
        }
    }

    function selectBoard(board: int) {
        tableView.selectionModel.clear();
        tableView.selectionModel.setCurrentIndex(root.tableView.model.index(board - 1, 0), ItemSelectionModel.SelectCurrent | ItemSelectionModel.Rows);
        if (board != 1) {
            // FIXME: column size streches to fit when moving to the first row
            tableView.positionViewAtRow(board - 1, TableView.Contain);
        }
    }
}
