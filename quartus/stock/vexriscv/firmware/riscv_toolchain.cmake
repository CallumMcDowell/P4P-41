# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
# https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ

# the name of the target operating system
set(CMAKE_SYSTEM_NAME               "Generic")
set(CMAKE_SYSTEM_PROCESSOR          "vexriscv")



# disable self-tests as they are not supported for cross-compilation.
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)


# configure cross-compiler
set(BAREMETAL_RISCV_TOOLCHAIN_PATH "C:/Users/cjamc/Documents/Git/P4P-41/quartus/stock/vexriscv/VexRiscv-master/opt/riscv/bin/")
set(BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME "riscv64-unknown-elf")
set(EXECUTABLE_SUFFIX ".exe")

set(CMAKE_AR                        ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-ar${EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-gcc${EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-gcc${EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-g++${EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-ld${EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-objcopy${EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-ranlib${EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-size${EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     ${BAREMETAL_RISCV_TOOLCHAIN_PATH}${BAREMETAL_RISCV_TOOLCHAIN_BASE_NAME}-strip${EXECUTABLE_SUFFIX} CACHE INTERNAL "")

set(CMAKE_C_FLAGS                   "-Wno-psabi --specs=nosys.specs -fdata-sections -ffunction-sections -Wl,--gc-sections" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${CMAKE_C_FLAGS} -fno-exceptions" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-Os -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-Os -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")


# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)