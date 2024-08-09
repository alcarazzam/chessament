// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtCore
import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApp
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament
import org.kde.chessament.settings as Settings

StatefulApp.StatefulWindow {
    id: root

    title: i18nc("@title:window", "Chessament")

    windowName: "Main"

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    application: ChessamentApplication {
        configurationView: Settings.ChessamentConfigurationView {}
    }

    Connections {
        target: root.application

        function onOpenTournament() {
            openTournamentFileDialog.open()
        }

        function onSaveTournament() {
            TournamentController.saveTournament()
        }

        function onSaveTournamentAs() {
            saveTournamentFileDialog.open()
        }

        function onImportTrf(fileName) {
            importTournamentFileDialog.open()
        }
    }

    Connections {
        target: TournamentController

        function onCurrentViewChanged() {
            const view = TournamentController.currentView
            if (view == "players") {
                const page = Qt.createComponent("org.kde.chessament", "PlayersPage").createObject(root)
                root.pageStack.replace(page)
            } else if (view == "pairings") {
                const page = Qt.createComponent("org.kde.chessament", "PairingsPage").createObject(root)
                root.pageStack.replace(page)
            }
        }

        function onErrorChanged() {
            errorDialog.open()
        }
    }

    Kirigami.PromptDialog {
        id: errorDialog
        title: i18nc("@title", "Error")
        subtitle: TournamentController.error
        standardButtons: Kirigami.Dialog.Ok
    }

    FileDialog {
        id: openTournamentFileDialog
        parentWindow: root
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            TournamentController.openTournament(selectedFile)
        }
    }

    FileDialog {
        id: saveTournamentFileDialog
        parentWindow: root
        fileMode: FileDialog.SaveFile
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            TournamentController.saveTournamentAs(selectedFile)
        }
    }

    FileDialog {
        id: importTournamentFileDialog
        parentWindow: root
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            TournamentController.importTrf(selectedFile)
        }
    }

    globalDrawer: Kirigami.OverlayDrawer {
        id: drawer

        modal: false
        width: 80
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        Kirigami.Theme.colorSet: Kirigami.Theme.Window

        ListModel {
            id: navigationModel

            ListElement {
                name: "Players"
                viewName: "players"
                iconName: "user-symbolic"
            }
            ListElement {
                name: "Pairings"
                viewName: "pairings"
                iconName: "system-users-symbolic"
            }
        }

        contentItem: ColumnLayout {
            QQC2.ToolBar {
                Layout.fillWidth: true
                Layout.preferredHeight: root.pageStack.globalToolBar.preferredHeight

                leftPadding: 0
                rightPadding: 0

                contentItem: Item {
                    QQC2.ToolButton {
                        id: menuButton
                        icon.name: "application-menu"
                        onClicked: optionPopup.popup()

                        x: 40 - width / 2

                        QQC2.Menu {
                            id: optionPopup

                            Kirigami.Action {
                                fromQAction: root.application.action("file_open")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_save")
                                enabled: TournamentController.tournamentPath
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_save_as")
                                enabled: TournamentController.hasOpenTournament
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("import_trf")
                            }
                            QQC2.MenuSeparator { }
                            Kirigami.Action {
                                fromQAction: root.application.action("options_configure")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("options_configure_keybinding")
                            }
                            QQC2.MenuSeparator { }
                            Kirigami.Action {
                                fromQAction: root.application.action("open_about_page")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("open_about_kde_page")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_quit")
                            }
                        }
                    }
                }
            }
        }

        QQC2.ScrollView {
            visible: TournamentController.hasOpenTournament
            Layout.fillHeight: true
            Layout.fillWidth: true

            QQC2.ScrollBar.vertical.interactive: false

            ListView {
                spacing: 0
                clip: true

                model: navigationModel
                delegate: NavigationButtonDelegate { }
            }
        }

        // HACK: make the toolbar be at the top even when there's no navigation buttons.
        ColumnLayout {
            visible: !TournamentController.hasOpenTournament
        }
    }

    contextDrawer: PlayerDrawer {
        id: contextDrawer

        modal: false
        enabled: TournamentController.currentPlayerIndex >= 0
        onEnabledChanged: drawerOpen = enabled
        handleVisible: false

        Connections {
            target: TournamentController

            function onCurrentPlayerChanged(player) {
                contextDrawer.drawerOpen = TournamentController.currentPlayerIndex >= 0
            }

            function onCurrentViewChanged() {
                contextDrawer.drawerOpen = contextDrawer.drawerOpen && TournamentController.currentView == "players"
            }
        }
    }

    pageStack.defaultColumnWidth: root.width

    pageStack.initialPage: Qt.createComponent("org.kde.chessament", "WelcomePage")
}
