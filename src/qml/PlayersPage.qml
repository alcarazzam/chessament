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

    horizontalScrollBarPolicy: QQC2.ScrollBar.AsNeeded

    AddPlayerDialog {
        id: addPlayerDialog
    }

    TournamentSettings {
        id: tournamentSettings
    }

    actions: [
        Kirigami.Action {
            id: addAction
            icon.name: "list-add"
            text: i18nc("@action:button", "Add player")
            onTriggered: addPlayerDialog.open()
        },
        Kirigami.Action {
            id: settingsAction
            icon.name: "settings-configure"
            text: i18nc("@action:button", "Settings")
            onTriggered: tournamentSettings.open() // Qt.createComponent("org.kde.chessament", "TournamentSettings").createObject(root.QQC2.ApplicationWindow.window, {}).open()
        }
    ]

    KSortFilterProxyModel {
        id: proxyModel
        sourceModel: TournamentController.playersModel
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

        Connections {
            target: tableView.selectionModel

            function onCurrentChanged(current, previous) {
                if (current.row === undefined || current.row === null || current.column !== 0) {
                    return
                }

                TournamentController.setCurrentPlayerByIndex(current.row)
            }
        }

        headerComponents: [
            Tables.HeaderComponent {
                width: 50
                title: i18nc("@title:column", "No")
                textRole: "startingRank"
                role: PlayerRoles.StartingRankRole
            },
            Tables.HeaderComponent {
                width: 70
                title: i18nc("@title:column", "Title")
                textRole: "title"
                role: PlayerRoles.TitleRole
            },
            Tables.HeaderComponent {
                width: 300
                title: i18nc("@title:column", "Name")
                textRole: "name"
                role: PlayerRoles.NameRole
            },
            Tables.HeaderComponent {
                width: 100
                resizable: false
                title: i18nc("@title:column", "Rating")
                textRole: "rating"
                role: PlayerRoles.RatingRole
            },
            Tables.HeaderComponent {
                width: 100
                resizable: false
                title: i18nc("@title:column", "National rating")
                textRole: "nationalRating"
                role: PlayerRoles.NationalRatingRole
            },
            Tables.HeaderComponent {
                width: 150
                title: i18nc("@title:column", "Player ID")
                textRole: "playerId"
                role: PlayerRoles.PlayerIdRole
            },
            Tables.HeaderComponent {
                width: 150
                title: i18nc("@title:column", "Birth date")
                textRole: "birthDate"
                role: PlayerRoles.BirthDateRole
            },
            Tables.HeaderComponent {
                width: 100
                title: i18nc("@title:column", "Federation")
                textRole: "federation"
                role: PlayerRoles.FederationRole
            },
            Tables.HeaderComponent {
                width: 200
                title: i18nc("@title:column", "Origin")
                textRole: "origin"
                role: PlayerRoles.OriginRole
            },
            Tables.HeaderComponent {
                width: 60
                resizable: false
                title: i18nc("@title:column", "Sex")
                textRole: "sex"
                role: PlayerRoles.SexRole
            }
        ]
    }
}