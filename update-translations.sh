#!/usr/bin/bash

set -e

# Requires git@invent.kde.org:sysadmin/l10n-scripty.git
extract-messages.sh

msgmerge --update --backup=none --previous po/es/chessament.po po/chessament.pot
