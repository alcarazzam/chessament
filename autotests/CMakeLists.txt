# SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

enable_testing()

add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data" )

ecm_add_test(
    tournamenttest.cpp
    LINK_LIBRARIES chessament_static Qt::Test
    TEST_NAME tournamenttest
)

