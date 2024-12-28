// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2

import org.kde.kitemmodels
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.tableview as Tables

import dev.alcarazzam.chessament
import dev.alcarazzam.chessament.PairingModel

Kirigami.ScrollablePage {
    id: root

    horizontalScrollBarPolicy: QQC2.ScrollBar.AsNeeded

    actions: [
        Kirigami.Action {
            text: i18n("Pair round %1", Controller.tournament.currentRound + 1)
            visible: (Controller.tournament.currentRound + 1 <= Controller.tournament.numberOfRounds) && Controller.tournament.isRoundFinished(Controller.tournament.currentRound)
            onTriggered: Controller.pairRound()
        },
        Kirigami.Action {
            text: i18n("Round %1", Controller.currentRound)
            onTriggered: roundDialog.open()
        }
    ]

    Kirigami.Dialog {
        id: roundDialog
        title: i18nc("@title", "Select round")

        ListView {
            id: listView
            implicitWidth: Kirigami.Units.gridUnit * 12
            implicitHeight: Kirigami.Units.gridUnit * 16

            model: Controller.tournament.numberOfRounds
            delegate: QQC2.RadioDelegate {
                required property int index

                topPadding: Kirigami.Units.smallSpacing * 2
                bottomPadding: Kirigami.Units.smallSpacing * 2
                implicitWidth: listView.width
                text: i18n("Round %1", index + 1)
                checked: Controller.currentRound == index + 1
                onClicked: {
                    Controller.currentRound = index + 1;
                    roundDialog.close();
                }
            }
        }
    }

    KSortFilterProxyModel {
        id: proxyModel
        sourceModel: Controller.pairingModel
        sortColumn: 0
        sortOrder: Qt.AscendingOrder
    }

    Tables.KTableView {
        id: tableView
        model: proxyModel

        clip: true
        sortOrder: Qt.AscendingOrder
        selectionBehavior: TableView.SelectRows
        selectionMode: TableView.SingleSelection

        headerComponents: [
            Tables.HeaderComponent {
                width: 60
                title: i18nc("@title:column", "Board")
                textRole: "board"
                role: PairingRoles.BoardRole
            },
            Tables.HeaderComponent {
                width: 60
                title: i18nc("@title:column", "No")
                textRole: "whiteStartingRank"
                role: PairingRoles.WhiteStartingRankRole
            },
            Tables.HeaderComponent {
                width: 300
                title: i18nc("@title:column", "White player")
                textRole: "whiteName"
                role: PairingRoles.WhiteNameRole
            },
            Tables.HeaderComponent {
                width: 70
                title: i18nc("@title:column", "Result")
                textRole: "result"
                role: PairingRoles.ResultRole
            },
            Tables.HeaderComponent {
                width: 300
                title: i18nc("@title:column", "Black player")
                textRole: "blackName"
                role: PairingRoles.BlackNameRole
            },
            Tables.HeaderComponent {
                width: 60
                title: i18nc("@title:column", "No")
                textRole: "blackStartingRank"
                role: PairingRoles.BlackStartingRankRole
            }
        ]
    }
}
