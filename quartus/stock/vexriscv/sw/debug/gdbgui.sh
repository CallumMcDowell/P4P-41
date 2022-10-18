gdbgui -g "/opt/riscv/bin/riscv64-unknown-elf-gdb \
    -q ./testing/out/bootrom.elf \
    -ex 'target extended-remote localhost:3333'"

# Start gdbgui in a new terminal
# GDBGUI_START="gdbgui \
#     -g '/opt/riscv/bin/riscv64-unknown-elf-gdb \
#     -q ../out/bootrom.elf \
#     -ex 'target extended-remote localhost:3333''"

# x-terminal-emulator -e bash -c ${GDBGUI_START}
