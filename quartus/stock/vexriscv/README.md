# ArieEmbedded Wrapper Adoptation

This is the project implementing a simple Vexriscv system through qsys with the generated `.v` file and `tcl` component from `Qsysfy()`.

## Equipment list

- **HW:**
  - De1-SoC
  - FDFI CS323HM DDHSL-0 Cable
- **SW (macro, for other programs, see the installation log):**
  - Ubuntu 22.04 LTS
  - Quartus (Quartus Prime 21.1) Lite Edition

## Start

1) Build the qsys system.
2) Build the core (see [VexRiscv Core](#VexRiscv-Core-(Scala-project))).
3) Compile the bitstream.
4) Load the sitstream ontpo the target board.

## Structure

```
├vexriscv-master        // local clone of Vexriscv repo
│
...
├openOCD                // makefile firmware taken from AriesEmbedded project.
│├interface             
│└cfg                   
├cores                  // generated core sources of the CPU
├firmware               // cmake firmware built from scratch. 
├sw                     // makefile firmware taken from AriesEmbedded project.
│└out                   // binary output (we are using .mif)
```

**Note:**
`sw` and `openOCD` is expected to be run from their respective folders (in terminal).

## VexRiscv Core (Scala project)

Core used is a modified version of `GenFullNoMmuMaxPerf` modified as `GenAvalonFullNoMmuMaxPerf`. The generated files is placed under `cores/VexRiscvAvalonMaxPerf` to be included by qsys (to include in Quartus, open `qsys->tools->options` and add the path to `cores/`).

Notes:
- Vexriscv is **active low**.

### Before Using the generated core in Qsys

- Define ``define SYNTHESIS` in file.
- Ensure `iorange` and `resetvector` is correcly configured with the software HAL specifications (for gpio for example). Check both the scala and the generated `.v` for:
  - **SimplePlugin**
    - C_RESET_VECTOR
      - `IBusSimplePlugin_fetchPc_pcReg`
    - C_EXCEPTION_VECTOR
      - `CsrPlugin_mtvec_mode`
      - `CsrPlugin_mtvec_base`
  - **Cached Plugin**
    - C_RESET_VECTOR
      - `IBusCachedPlugin_fetchPc_pcReg`
      - `IBusCachedPlugin_decodePc_pcReg`
    - C_EXCEPTION_VECTOR
      - `CsrPlugin_mtvec_mode`
      - `CsrPlugin_mtvec_base`
    - C_IO_BEGIN & C_IO_END
      - `IBusCachedPlugin_mmuBus_rsp_isIoAccess`
      - `DBusCachedPlugin_mmuBus_rsp_isIoAccess`

### Method

0. Generate the core in the scala project under `VexRiscv-master` and copy to `cores`.
1. To do

## SW

### Vectors

0) Vexriscv CPU is **active low**, wire all neglected input ports to ‘0’ (i.e. `Softwareinterrupt`)
    **Note:** interrupt_receive port through the `VexInterruptController.vhd` is threaded through many intermediate signals, but generally: `top-level input -> interrupt_controller -[back out as exception]-> CPU`

1) Assign instruction vector. There are three vectors to be aware of: `ibus_reset`, `ibus_exception`, and `static_io` regions. The SW need to be aware of ibus reset vector, OCRM origin vector for the dual port onchip-RAM (found in `sw/link.ld`), and the memroy mapped `gpio` vector (instantiated in quartus).

## Qsys: VexRiscv_system

- To do.

# OpenOCD

- Power off both target board and cable before plugging.
- de1-SoC I/O Interface: 3.3V TTL

## Setup

- Install [openOCD (Vexriscv port)](https://github.com/SpinalHDL/openocd_riscv). Note that only this port will work, as Vexriscv debug module does not adhere to the RISC-V Debug Specification.
- Install the [D2XX drivers](http://ftdichip.com/Drivers/D2XX.htm) for the adaptor.

> It looks like following this (https://tomverbeure.github.io/2021/07/18/VexRiscv-OpenOCD-and-Traps.html) (and > https://github.com/SpinalHDL/VexRiscv/tree/master/doc/nativeJtag) is the way to go.

**Note:**
- CPU clock must be higher than the JTAG clock

## Wiring 

![](./doc/images/SoC%20GPIO.png)

```vhdl
vexriscvavalon_0_jtag_tck => GPIO_0(1),
vexriscvavalon_0_jtag_tdi => GPIO_0(3),            
vexriscvavalon_0_jtag_tdo => GPIO_0(5),  
vexriscvavalon_0_jtag_tms => GPIO_0(7), 
-- and GND
```

## openOCD Steps

1) Wire theJATG adaptor to GPIO_0 (above)
> Sanity Check (find USB): lsusb
2) Open a terminal in `/openOCD` and run 
  
```sh
openocd -f interface/ftdi/c232hm.cfg -c "adapter speed 1000; transport select jtag" \
	-f cfg/vexriscv_init.cfg
```

```sh
jtag
Info : set servers polling period to 50ms
Info : clock speed 1000 kHz
Info : JTAG tap: vexrisc_ocd.bridge tap/device found: 0x10001fff (mfg: 0x7ff (<invalid>), part: 0x0001, ver: 0x1)
Info : starting gdb server for vexrisc_ocd.cpu0 on 3333
Info : Listening on port 3333 for gdb connections
Halting processor
requesting target halt and executing a soft reset
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

## Quartus QNA:

Q: Quartus throws syntax error, everything is fine!!
A: Check that Quartus is phasing the HDL in its correct version. Note that Quartus Lite will not support `VHDL 2008` and have trouble phasing `.sv`. However, if the code was able to compile before, but cannot now, try restarting Quartus, and if that does not work, restart your computer.

Q: Cannot phase some `.v` file properly!
A: If you have included the qsys via the `.qip`, try removing the inclusiong and included the `.qsys` file directly. Note this will regenerate the qsys system every time you compile.

Q: What's up with `irq_soft`

```vhdl
-- All following defined in VexRiscvAvalon_hw.tcl
avalon_address        => ic_avalon_address,
avalon_write          => ic_avalon_write,
avalon_writedata      => ic_avalon_writedata,
avalon_read           => ic_avalon_read,
avalon_readdata       => ic_avalon_readdata,
avalon_readdatavalid  => ic_avalon_readdatavalid,
avalon_waitrequest    => ic_avalon_waitrequest


resetn      => not reset,      // !!Why? Supposedly resets on `0` soo...
irq_source  => irq_source, // to qsys interface
ext_irq     => irq_external,  // irq_external
tmr_irq     => irq_timer,     // timerInterrupt
sft_irq     => irq_soft,      // softwareInterrupt
```

Add the following connection points in the Vexriscv CPU `hw.tcl` component script:

```tcl

package require -exact qsys 13.1

#
# module def
#
set_module_property DESCRIPTION ""
set_module_property NAME InterruptController
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR "SpinalHDL & ARIES Embedded GmbH & Hao Lin"
set_module_property DISPLAY_NAME External InterruptController
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE false
set_module_property ANALYZE_HDL false
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false

#
# file sets
#
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL VexRiscvAvalonMaxPerf
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
#add_fileset_file VexRiscvAvalonMaxPerf.vhd VHDL PATH VexRiscvAvalonMaxPerf.vhd TOP_LEVEL_FILE

add_fileset SIM_VHDL SIM_VHDL "" ""
set_fileset_property SIM_VHDL TOP_LEVEL VexRiscvAvalonMaxPerf
set_fileset_property SIM_VHDL ENABLE_RELATIVE_INCLUDE_PATHS false
#add_fileset_file VexRiscvAvalonMaxPerf.vhd VHDL PATH VexRiscvAvalonMaxPerf.vhd

#
# connection point irq_controller
#
add_interface irq_controller avalon end
set_interface_property irq_controller addressUnits WORDS
set_interface_property irq_controller associatedClock clock
set_interface_property irq_controller associatedReset reset
set_interface_property irq_controller bitsPerSymbol 8
set_interface_property irq_controller burstOnBurstBoundariesOnly false
set_interface_property irq_controller burstcountUnits WORDS
set_interface_property irq_controller explicitAddressSpan 0
set_interface_property irq_controller holdTime 0
set_interface_property irq_controller linewrapBursts false
set_interface_property irq_controller maximumPendingReadTransactions 1
set_interface_property irq_controller maximumPendingWriteTransactions 0
set_interface_property irq_controller readLatency 0
set_interface_property irq_controller readWaitTime 1
set_interface_property irq_controller setupTime 0
set_interface_property irq_controller timingUnits Cycles
set_interface_property irq_controller writeWaitTime 0
set_interface_property irq_controller ENABLED true
set_interface_property irq_controller EXPORT_OF ""
set_interface_property irq_controller PORT_NAME_MAP ""
set_interface_property irq_controller CMSIS_SVD_VARIABLES ""
set_interface_property irq_controller SVD_ADDRESS_GROUP ""

add_interface_port irq_controller ic_avalon_address address Input 4
add_interface_port irq_controller ic_avalon_write write Input 1
add_interface_port irq_controller ic_avalon_writedata writedata Input 32
add_interface_port irq_controller ic_avalon_read read Input 1
add_interface_port irq_controller ic_avalon_readdata readdata Output 32
add_interface_port irq_controller ic_avalon_readdatavalid readdatavalid Output 1
add_interface_port irq_controller ic_avalon_waitrequest waitrequest Output 1
set_interface_assignment irq_controller embeddedsw.configuration.isFlash 0
set_interface_assignment irq_controller embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment irq_controller embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment irq_controller embeddedsw.configuration.isPrintableDevice 0
```