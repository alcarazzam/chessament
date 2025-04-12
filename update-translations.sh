#!/usr/bin/bash
# SPDX-FileCopyrightText: 2025 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
# SPDX-License-Identifier: CC0-1.0

set -e

# Requires git@invent.kde.org:sysadmin/l10n-scripty.git
extract-messages.sh

msgmerge --update --backup=none --previous po/es/chessament.po po/chessament.pot
