// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtCore
import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Dialogs as Dialogs
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApp
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament
import org.kde.chessament.settings as Settings

StatefulApp.StatefulWindow {
    id: root

    property var pageCache: Object.create(null)

    title: i18nc("@title:window", "Chessament")

    windowName: "Main"

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    application: ChessamentApplication {
        configurationView: Settings.ChessamentConfigurationView {}
    }

    Connections {
        target: root.application

        function onNewTournament() {
            let dialog = Qt.createComponent("org.kde.chessament", "NewTournamentDialog").createObject(root)
            dialog.create.connect((fileUrl, name, rounds) => {
                Controller.newTournament(fileUrl, name, rounds)
            })
            dialog.open()
        }

        function onOpenTournament() {
            const dialog = fileDialog.createObject(QQC2.Overlay.overlay)
            dialog.accepted.connect(() => {
                Controller.openTournament(dialog.selectedFile)
            })
            dialog.open()
        }

        function onSaveTournament() {
            Controller.saveTournament()
        }

        function onSaveTournamentAs() {
            const dialog = fileDialog.createObject(QQC2.Overlay.overlay)
            dialog.fileMode = Dialogs.FileDialog.SaveFile
            dialog.accepted.connect(() => {
                Controller.saveTournamentAs(dialog.selectedFile)
            })
            dialog.open()
        }

        function onImportTrf() {
            const dialog = fileDialog.createObject(QQC2.Overlay.overlay)
            dialog.accepted.connect(() => {
                Controller.importTrf(dialog.selectedFile)
            })
            dialog.open()
        }

        function onExportTrf() {
            const dialog = fileDialog.createObject(QQC2.Overlay.overlay)
            dialog.fileMode = Dialogs.FileDialog.SaveFile
            dialog.accepted.connect(() => {
                Controller.exportTrf(dialog.selectedFile)
            })
            dialog.open()
        }
    }

    Connections {
        target: Controller

        function onCurrentViewChanged() {
            const view = Controller.currentView
            const page = pageForView(view)
            root.pageStack.replace(page)
        }

        function onErrorChanged() {
            errorDialog.open()
        }
    }

    Kirigami.PromptDialog {
        id: errorDialog
        title: i18nc("@title", "Error")
        subtitle: Controller.error
        standardButtons: Kirigami.Dialog.Ok
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
                                fromQAction: root.application.action("file_new")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_open")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_save")
                                enabled: Controller.tournamentPath
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("file_save_as")
                                enabled: Controller.hasOpenTournament
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("import_trf")
                            }
                            Kirigami.Action {
                                fromQAction: root.application.action("export_trf")
                                enabled: Controller.hasOpenTournament
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
            visible: Controller.hasOpenTournament
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
            visible: !Controller.hasOpenTournament
        }
    }

    pageStack.defaultColumnWidth: root.width

    pageStack.initialPage: Qt.createComponent("org.kde.chessament", "WelcomePage")

    function pageForView(view: string): var {
        if (pageCache[view]) {
            return pageCache[view]
        } else {
            let page
            if (view == "players") {
                page = Qt.createComponent("org.kde.chessament", "PlayersPage").createObject(root)
            } else if (view == "pairings") {
                page = Qt.createComponent("org.kde.chessament", "PairingsPage").createObject(root)
            }
            pageCache[view] = page
            return page
        }
    }

    Component {
        id: fileDialog

        Dialogs.FileDialog {
            currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
        }
    }
}
