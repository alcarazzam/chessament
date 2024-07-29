// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament

Kirigami.OverlayDrawer {
    id: root

    readonly property Player player: TournamentController.currentPlayer
    
    width: Kirigami.Units.gridUnit * 20
    enabled: true
    edge: Qt.application.layoutDirection == Qt.RightToLeft ? Qt.LeftEdge : Qt.RightEdge

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    Kirigami.Theme.colorSet: Kirigami.Theme.View

    contentItem: ColumnLayout {
        spacing: 0

        QQC2.ToolBar {
            Layout.fillWidth: true
            Layout.preferredHeight: applicationWindow().pageStack.globalToolBar.preferredHeight

            contentItem: RowLayout {
                Kirigami.Heading {
                    Layout.fillWidth: true
                    text: i18nc("@title:window", "Edit player")
                }

                QQC2.ToolButton {
                    display: QQC2.AbstractButton.IconOnly
                    text: i18nc("@action:button", "Close edit player drawer")
                    icon.name: 'window-close-symbolic'

                    QQC2.ToolTip.text: text
                    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                    QQC2.ToolTip.visible: hovered

                    onClicked: {
                        root.close()
                    }
                }
            }
        }

        Loader {
            id: drawerItemLoader
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: playerDetails
        }

        Component {
            id: playerDetails
            PlayerDetails { }
        }
    }
}
