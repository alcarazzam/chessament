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

        function onImportTrf(fileName) {
            fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        parentWindow: root
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        onAccepted: {
            TournamentController.importTrf(selectedFile)

            const page = Qt.createComponent("org.kde.chessament", "PlayersPage").createObject(root)
            root.pageStack.replace(page)
        }
    }

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: !Kirigami.Settings.isMobile
        actions: [
            Kirigami.Action {
                fromQAction: root.application.action("import_trf")
            },
            Kirigami.Action {
                fromQAction: root.application.action("options_configure")
            },
            Kirigami.Action {
                fromQAction: root.application.action("options_configure_keybinding")
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                fromQAction: root.application.action("open_about_page")
            },
            Kirigami.Action {
                fromQAction: root.application.action("open_about_kde_page")
            },
            Kirigami.Action {
                fromQAction: root.application.action("file_quit")
            }
        ]
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
        }
    }

    pageStack.defaultColumnWidth: root.width

    pageStack.initialPage: Qt.createComponent("org.kde.chessament", "WelcomePage")
}
