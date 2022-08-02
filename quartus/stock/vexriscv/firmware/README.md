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
