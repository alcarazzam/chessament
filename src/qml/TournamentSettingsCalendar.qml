// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>

import QtQuick
import QtQuick.Layouts

import org.kde.kirigamiaddons.formcard as FormCard

import org.kde.chessament

FormCard.FormCardPage {
    id: root

    title: i18nc("@title", "Calendar")

    Repeater {
        model: TournamentController.tournament.numberOfRounds

        ColumnLayout {
            required property int index

            FormCard.FormHeader {
                title: i18nc("round number", "Round %1", index + 1)
            }
            FormCard.FormCard {
                FormCard.FormDateTimeDelegate {

                }
            }
        }
    }
}
