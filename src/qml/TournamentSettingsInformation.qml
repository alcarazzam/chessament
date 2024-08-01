// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick

import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament

FormCard.FormCardPage {
    id: root

    title: i18nc("@title", "Information")

    FormCard.FormHeader {
        title: i18nc("@title:group", "Basic details")
    }
    FormCard.FormCard {
        FormCard.FormTextFieldDelegate {
            id: name
            label: i18nc("@label:textbox", "Tournament name")
            text: TournamentController.tournament.name
            onEditingFinished: {
                TournamentController.tournament.name = text
            }
        }
        
        FormCard.FormTextFieldDelegate {
            id: city
            label: i18nc("@label:textbox", "City")
            text: TournamentController.tournament.city
            onEditingFinished: {
                TournamentController.tournament.city = text
            }
        }

        FormCard.FormTextFieldDelegate {
            id: federation
            label: i18nc("@label:textbox", "Federation")
            text: TournamentController.tournament.federation
            onEditingFinished: {
                TournamentController.tournament.federation = text
            }
        }

        FormCard.FormTextFieldDelegate {
            id: chiefArbiter
            label: i18nc("@label:textbox", "Chief arbiter")
            text: TournamentController.tournament.chiefArbiter
            onEditingFinished: {
                TournamentController.tournament.chiefArbiter = text
            }
        }

        FormCard.FormTextFieldDelegate {
            id: deputyChiefArbiter
            label: i18nc("@label:textbox", "Deputy chief arbiter")
            text: TournamentController.tournament.deputyChiefArbiter
            onEditingFinished: {
                TournamentController.tournament.deputyChiefArbiter = text
            }
        }
    }
}
