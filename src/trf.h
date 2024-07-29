// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFile>
#include <QTextStream>

#include "tournament.h"

extern Tournament* loadTournamentReport(const QUrl &fileUrl);
