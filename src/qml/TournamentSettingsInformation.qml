// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick

import org.kde.kirigamiaddons.formcard as FormCard

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
        }
        
        FormCard.FormTextFieldDelegate {
            id: city
            label: i18nc("@label:textbox", "City")
        }

        FormCard.FormTextFieldDelegate {
            id: federation
            label: i18nc("@label:textbox", "Federation")
        }

        FormCard.FormTextFieldDelegate {
            id: chiefArbiter
            label: i18nc("@label:textbox", "Chief arbiter")
        }

        FormCard.FormTextFieldDelegate {
            id: deputyChiefArbiter
            label: i18nc("@label:textbox", "Deputy chief arbiter")
        }
    }

    FormCard.FormHeader {
        title: i18nc("@title:group", "Start date")
    }
    FormCard.FormCard {
        FormCard.FormDateTimeDelegate {
            dateTimeDisplay: FormCard.FormDateTimeDelegate.DateTimeDisplay.Date
        }
    }

    FormCard.FormHeader {
        title: i18nc("@title:group", "End date")
    }
    FormCard.FormCard {
        FormCard.FormDateTimeDelegate {
            dateTimeDisplay: FormCard.FormDateTimeDelegate.DateTimeDisplay.Date
        }
    }
}
