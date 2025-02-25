###############################################################################
#
# MIT License
#
# Copyright (C) 2021-2025 Advanced Micro Devices, Inc. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
###############################################################################

# Include path for current test files.
# Includes also rely on load_store_matrix_sync_test
set(ROCWMMA_TEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../load_store_matrix_sync_test/ ${ROCWMMA_TEST_INCLUDE_DIRS})

set(LoadStoreMatrixCoopSyncTestSources ${UnitCommonSources}
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_a_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_a_32.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_a_64.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_a_128.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_a_256.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_acc_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_acc_32.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_acc_64.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_acc_128.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_acc_256.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_b_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_b_32.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_b_64.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_b_128.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/load_store_matrix_coop_sync_b_256.cpp

                    # Emulation Smoke
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/smoketest-load_store_matrix_coop_sync_a_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/smoketest-load_store_matrix_coop_sync_b_16.cpp
                    # Emulation regression
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/regressiontest-load_store_matrix_coop_sync_a_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/regressiontest-load_store_matrix_coop_sync_b_16.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/regressiontest-load_store_matrix_coop_sync_a_32.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/test/emulation/regressiontest-load_store_matrix_coop_sync_b_32.cpp
                 )

add_rocwmma_unit_test(load_store_matrix_coop_sync_test ${LoadStoreMatrixCoopSyncTestSources})
