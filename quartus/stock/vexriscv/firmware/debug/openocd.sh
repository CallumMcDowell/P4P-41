openocd -f interface/ftdi/c232hm.cfg \
    -c "adapter speed 1000; transport select jtag" \
    -f vexriscv_init.cfg