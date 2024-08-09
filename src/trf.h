// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <KLocalizedString>
#include <QFile>
#include <QTextStream>
#include <expected>

#include "tournament.h"

extern std::expected<Tournament *, QString> loadTournamentReport(const QUrl &fileUrl);
