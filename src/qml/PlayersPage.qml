// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as QQC2
import Qt.labs.qmlmodels
import QtQml.Models as Models

import org.kde.kitemmodels
import org.kde.kirigami as Kirigami

import dev.alcarazzam.chessament
import dev.alcarazzam.chessament.PlayersModel

Kirigami.Page {
    id: root

    padding: 0

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
            onTriggered: tournamentSettings.open()
        },
        Kirigami.Action {
            id: printAction
            icon.name: "document-print"
            text: i18nc("@action:button", "Print")
            onTriggered: {
                applicationWindow().pageStack.push(Qt.createComponent("dev.alcarazzam.chessament", "DocsPage").createObject(root));
            }
        }
    ]

    KSortFilterProxyModel {
        id: proxyModel
        sourceModel: Controller.playersModel
        sortColumn: 0
        sortOrder: Qt.AscendingOrder
    }

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
            model: proxyModel

            alternatingRows: true

            selectionBehavior: TableView.SelectCells
            selectionModel: Models.ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            interactive: false

            rowHeightProvider: () => Kirigami.Units.gridUnit * 2
            columnWidthProvider: column => {
                let explicitWidth = explicitColumnWidth(column);
                if (explicitWidth > 0) {
                    return explicitWidth;
                }
                const columnWidths = [];
                columnWidths[0] = 50;
                columnWidths[1] = 70;
                columnWidths[2] = 200;
                columnWidths[3] = 150;
                columnWidths[4] = 100;
                columnWidths[5] = 100;
                columnWidths[6] = 150;
                columnWidths[7] = 150;
                columnWidths[8] = 100;
                columnWidths[9] = 200;
                columnWidths[10] = 60;
                return columnWidths[column];
            }

            Connections {
                target: tableView.selectionModel

                function onCurrentChanged(current, previous) {
                    Controller.setCurrentPlayerByIndex(current.row);
                }
            }

            delegate: QQC2.ItemDelegate {
                id: delegate

                required property var model
                required property int row
                required property int column
                required property bool current
                required property bool selected
                required property bool editing

                text: model.display
                highlighted: selected

                onClicked: {
                    tableView.closeEditor();
                    tableView.selectionModel.select(tableView.model.index(row, column), ItemSelectionModel.ClearAndSelect);
                }
                onDoubleClicked: {
                    tableView.edit(tableView.model.index(row, column));
                }

                TableView.editDelegate: DelegateChooser {
                    DelegateChoice {
                        column: PlayerRoles.RatingRole

                        QQC2.TextField {
                            required property var model

                            anchors.fill: parent
                            text: model.display
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            validator: IntValidator {
                                bottom: 0
                                top: 4000
                            }

                            Component.onCompleted: selectAll()

                            TableView.onCommit: {
                                model.display = text;
                            }
                        }
                    }

                    DelegateChoice {
                        column: PlayerRoles.TitleRole

                        QQC2.ComboBox {
                            id: comboBox

                            anchors.fill: parent
                            model: [" ", "GM", "IM", "FM", "WGM", "CM", "WIM", "WFM", "WCM"]

                            popup.onClosed: tableView.closeEditor()

                            onActivated: {
                                let index = tableView.model.index(delegate.row, delegate.column);
                                tableView.model.setData(index, comboBox.currentText);
                                tableView.closeEditor();
                            }

                            Component.onCompleted: {
                                let data = tableView.model.data(tableView.model.index(delegate.row, delegate.column));
                                comboBox.currentIndex = comboBox.indexOfValue(data);
                                comboBox.popup.open();
                            }
                        }
                    }

                    DelegateChoice {
                        QQC2.TextField {
                            required property var model

                            anchors.fill: parent
                            text: model.display
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter

                            Component.onCompleted: selectAll()

                            TableView.onCommit: {
                                model.display = text;
                            }
                        }
                    }
                }
            }
        }
    }
}
