# SPDX-FileCopyrightText: 2024 Manuel Alcaraz Zambrano <manuelalcarazzam@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

enable_testing()

ecm_add_test(
    tournamenttest.cpp
    LINK_LIBRARIES chessament_static Qt::Test
    TEST_NAME tournamenttest
)

