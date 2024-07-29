# SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(settings)
# add_subdirectory(players)

qt_add_library(chessament_static STATIC)
ecm_add_qml_module(chessament_static
    URI org.kde.chessament
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/org/kde/chessament
    GENERATE_PLUGIN_SOURCE
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/AddPlayerDialog.qml
        qml/PlayerDetails.qml
        qml/PlayerDrawer.qml
        qml/PlayersPage.qml
        qml/WelcomePage.qml

        # Tournament settings
        qml/TournamentSettings.qml
        qml/TournamentSettingsInformation.qml
        qml/TournamentSettingsFormat.qml
    DEPENDENCIES
        QtCore
        QtQuick
    SOURCES
        player.cpp
        player.h
        playersmodel.cpp
        playersmodel.h
        tournament.cpp
        tournament.h
        tournamentcontroller.cpp
        trf.cpp
        trf.h
    IMPORTS
        org.kde.kirigamiaddons.formcard
        org.kde.kirigamiaddons.statefulapp
        org.kde.kirigamiaddons.tableview
        org.kde.kirigami
#        org.kde.chessament.players
        org.kde.chessament.settings
)

target_sources(chessament_static PUBLIC
    chessamentapplication.cpp
    chessamentapplication.h
)

target_link_libraries(chessament_static PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::ItemModels
    KirigamiAddonsStatefulApp
)
target_include_directories(chessament_static PUBLIC ${CMAKE_BINARY_DIR})

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
        list-add
        help-about
        application-exit
        applications-graphics
    )
else()
    target_link_libraries(chessament_static PUBLIC Qt::Widgets)
endif()

kconfig_add_kcfg_files(chessament_static GENERATE_MOC chessamentconfig.kcfgc)

add_executable(chessament main.cpp)
target_link_libraries(chessament PRIVATE
    chessament_static
    chessament_staticplugin
#    chessament_players
#    chessament_playersplugin
    chessament_settings
    chessament_settingsplugin
)
install(TARGETS chessament ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
