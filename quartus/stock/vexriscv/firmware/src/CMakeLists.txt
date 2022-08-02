# === RISC-V Setup ===
set(PROJECT_SOURCE_DIR                      "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_TOOLCHAIN_FILE                    "${PROJECT_SOURCE_DIR}/riscv_toolchain.cmake")

message("PROJECT_SOURCE_DIR points to "     ${PROJECT_SOURCE_DIR})
message("CMAKE_TOOLCHAIN_FILE points to "   ${CMAKE_TOOLCHAIN_FILE})

# MUST DELETE AND REBUILD CMAKE FILES IF DEPENDENT ENV VARIABLE IS CHANGED
# message("ENV_PATH points to " $ENV{PATH}) #{"riscv64-unknown-elf"}})

# === Parameters ===
set(RISCV_PROJECT_NAME              "vexriscv-firmware")
set(RISCV_PROJECT_VERSION           "0.1")

# set(RISCV_EXECUTABLE_SOURCE         "hello.c")
set(RISCV_EXECUTABLE_SOURCE         "minimal.cpp")
set(RISCV_EXECUTABLE_NAME           "hello.exe")

set(TENSORFLOW_SOURCE_DIR           "C:/Users/cjamc/Documents/Git/tensorflow_src")

# === PROJECT ===
# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
message("\n--- GENERATING " ${RISCV_PROJECT_NAME} " (v" ${RISCV_PROJECT_VERSION} ") ---")

cmake_minimum_required (VERSION 3.23)
project (${RISCV_PROJECT_NAME} VERSION ${RISCV_PROJECT_VERSION})


# link TensorFlow Lite library -->
# source: https://www.tensorflow.org/lite/guide/build_cmake#create_a_cmake_project_which_uses_tensorflow_lite
set(TENSORFLOW_SOURCE_DIR "" CACHE PATH
  "Directory that contains the TensorFlow project"
)
if(NOT TENSORFLOW_SOURCE_DIR)
  get_filename_component(TENSORFLOW_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../../../../"
    ABSOLUTE
  )
endif()

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
  "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite"
  EXCLUDE_FROM_ALL
)
# <--


# === EXECUTABLES AND LIBRARIES ===
add_executable(${RISCV_EXECUTABLE_NAME} ${RISCV_EXECUTABLE_SOURCE})
target_link_libraries(${RISCV_EXECUTABLE_NAME} tensorflow-lite)

# todo: configure to include version number in header
# https://cmake.org/cmake/help/latest/guide/tutorial/index.html