// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtCore
import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates

import org.kde.chessament

Delegates.RoundedItemDelegate {
    id: root

    required property string name
    required property string viewName
    required property string iconName
    required property var model

    icon.name: iconName
    text: name
    highlighted: Controller.currentView == viewName
    activeFocusOnTab: true

    Layout.fillWidth: true

    contentItem: Item {
        implicitHeight: icon.height + label.height + 3 * Kirigami.Units.smallSpacing
        Kirigami.Icon {
            id: icon
            source: root.icon.name
            y: Kirigami.Units.smallSpacing
            x: (80 - Kirigami.Units.largeSpacing * 2) / 2 - width / 2
        }
        Controls.Label {
            id: label
            text: root.name
            width: Math.min(parent.width, implicitWidth)
            elide: Text.ElideRight
            y: icon.height + icon. y + Kirigami.Units.smallSpacing
            x: (80 - Kirigami.Units.largeSpacing * 2) / 2 - Math.min((80 - Kirigami.Units.largeSpacing * 2), implicitWidth) / 2
        }
    }

    TapHandler {
        onTapped: {
            if (applicationWindow().pageStack.depth > 1) {
                applicationWindow().pageStack.pop(null)
            }
            Controller.currentView = viewName
        }
    }
}
