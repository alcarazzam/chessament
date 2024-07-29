// SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "chessamentapplication.h"
#include <KAuthorized>
#include <KLocalizedString>

using namespace Qt::StringLiterals;

ChessamentApplication::ChessamentApplication(QObject *parent)
    : AbstractKirigamiApplication(parent)
{
    setupActions();
}

void ChessamentApplication::setupActions()
{
    AbstractKirigamiApplication::setupActions();

    auto actionName = "import_trf"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = mainCollection()->addAction(actionName, this, &ChessamentApplication::importTrf);
        action->setText(i18nc("@action:inmenu", "Import tournament report"));
        action->setIcon(QIcon::fromTheme(u"document-import-symbolic"_s));
        mainCollection()->addAction(action->objectName(), action);
        mainCollection()->setDefaultShortcut(action, Qt::CTRL | Qt::Key_I);
    }

    readSettings();
}

#include "moc_chessamentapplication.cpp"
