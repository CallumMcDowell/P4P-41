

## NEWLY OUTDATED (for Lite, not micro!)

Working Version: tensorflow-2.9.1 (most recent release version)

The zipped tensorflow source must be downloaded and unpacked.
Due to the size of the tensorflow repo it is not practical
 to store the source within this repo. This may change if/when
 modifications to the source are required to implement macros
 using custom instructions.

The cmake build outputs *are* kept within this repo as they are
 project specific, and should be ignored in the .gitignore anyway.


## OUTDATED (pre-TensorFlow Lite Micro_

To build:

1. Place `CMakeLists.txt` and `riscv_toolchain.cmake` in the top level src folder.
3. Then run the below:

`$ cd "path/to/build"`\
`$ cmake "path/to/src"`\
`$ cmake -- build "path/to/build"`


i.e.,

`$ cd "path/to/vexriscv/firmware/build"`\
`$ cmake "../src"`\
`$ cmake --build .`
s

NOTE:\
The host machine must have an environment variable `RISCV_GCC` which points to 
the /bin location of local installed the gcc/gnu toolchain.