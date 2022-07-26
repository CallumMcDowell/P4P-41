set(PROJECT_SOURCE_DIR "C:/Users/cjamc/Documents/Git/P4P-41/quartus/stock/vexriscv/firmware")
set(CMAKE_TOOLCHAIN_FILE ${PROJECT_SOURCE_DIR}/riscv_toolchain.cmake)

message("PROJECT_SOURCE_DIR points to " ${PROJECT_SOURCE_DIR})
message("CMAKE_TOOLCHAIN_FILE points to " ${CMAKE_TOOLCHAIN_FILE})

# === PROJECT ===
# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 3.23)
project (vexriscv-firmware)

add_executable(hello hello.c)