# === RISC-V Setup ===
set(PROJECT_SOURCE_DIR                      ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_TOOLCHAIN_FILE                    ${PROJECT_SOURCE_DIR}/riscv_toolchain.cmake)

message("PROJECT_SOURCE_DIR points to "     ${PROJECT_SOURCE_DIR})
message("CMAKE_TOOLCHAIN_FILE points to "   ${CMAKE_TOOLCHAIN_FILE})

# MUST DELETE AND REBUILD CMAKE FILES IF DEPENDENT ENV VARIABLE IS CHANGED
# message("ENV_PATH points to " $ENV{PATH}) #{"riscv64-unknown-elf"}})

# === Parameters ===
set(RISCV_PROJECT_NAME              "vexriscv-firmware")
set(RISCV_PROJECT_VERSION           "0.1")

set(RISCV_EXECUTABLE_SOURCE         "hello.c")
set(RISCV_EXECUTABLE_NAME           "hello.exe")

# === PROJECT ===
# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 3.23)
project (${RISCV_PROJECT_NAME} VERSION ${RISCV_PROJECT_VERSION})

add_executable(${RISCV_EXECUTABLE_NAME} ${RISCV_EXECUTABLE_SOURCE})
# todo: configure to include version number in header
# https://cmake.org/cmake/help/latest/guide/tutorial/index.html