// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament

Flickable {
    id: root

    readonly property Player player: TournamentController.currentPlayer

    height: parent.height
    contentHeight: column.height
    boundsBehavior: Flickable.StopAtBounds
    clip: true

    Connections {
        target: TournamentController

        function onCurrentPlayerChanged() {
            titleField.currentIndex = titleField.indexOfValue(root.player.titleString())
            nameField.text = root.player.name
            ratingField.value = root.player.rating
            nationalRatingField.value = root.player.nationalRating
            playerIdField.text = root.player.playerId
            birthDateField.text = root.player.birthDate
            federationField.text = root.player.federation
            originField.text = root.player.origin
            sexField.text = root.player.sex
        }
    }

    QQC2.ScrollBar.vertical: QQC2.ScrollBar { id: scrollBar }
    
    ColumnLayout {
        id: column
        width: height > root.height ? root.width - scrollBar.width : root.width
        spacing: 0

        FormCard.FormComboBoxDelegate {
            id: titleField
            text: i18nc("@label:textbox", "Title")
            model: ["", "GM", "IM", "FM", "WGM", "CM", "WIM", "WFM", "WCM"]
            onCurrentValueChanged: {
                if (player.titleString() === currentText) {
                    return
                }
                player.setTitle(currentValue)
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: nameField
            label: i18nc("@label:textbox", "Name")
            onEditingFinished: {
                player.name = text
                TournamentController.savePlayer()
            }
        }

        FormCard.FormSpinBoxDelegate {
            id: ratingField
            label: i18nc("@label:textbox", "Rating")
            from: 0
            to: 4000
            onValueChanged: {
                if (player.rating === value) {
                    return
                }
                player.rating = value
                TournamentController.savePlayer()
            }
        }

        FormCard.FormSpinBoxDelegate {
            id: nationalRatingField
            label: i18nc("@label:textbox", "National rating")
            from: 0
            to: 4000
            onValueChanged: {
                if (player.nationalRating === value) {
                    return
                }
                player.nationalRating = value
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: playerIdField
            label: i18nc("@label:textbox", "Player ID")
            placeholderText: i18n("Optional")
            onEditingFinished: {
                player.playerId = text
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: birthDateField
            label: i18nc("@label:textbox", "Birth date")
            placeholderText: i18n("Optional")
            onEditingFinished: {
                player.birthDate = text
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: federationField
            label: i18nc("@label:textbox", "Federation")
            placeholderText: i18n("Optional")
            onEditingFinished: {
                player.federation = text
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: originField
            label: i18nc("@label:textbox", "Origin")
            placeholderText: i18n("Optional")
            onEditingFinished: {
                player.origin = text
                TournamentController.savePlayer()
            }
        }

        FormCard.FormTextFieldDelegate {
            id: sexField
            label: i18nc("@label:textbox", "Sex")
            placeholderText: i18n("Optional")
            onEditingFinished: {
                player.sex = text
                TournamentController.savePlayer()
            }
        }
    }
}