# 2022 P4P #41 @ University of Auckland

**Research Peers:**

- Callum McDowell
- Hao Lin

**Supervisor:**

- Morteza Biglari-Abhari 

This is the project implementing a simple Vexriscv system through qsys with the generated `.v` file and `tcl` component from `Qsysfy()`.

This project includes an [ArieEmbedded Wrapper Adoptation](https://github.com/ARIES-Embedded/riscv-on-max10) of the Vexriscv core to as an example to compare to test, develope software on when our custom core is not avaliable.

**Note:** We did not utilise any SW ISA simulation (i.e. spike) or HW simulator (i.e. verilator).

## Equipment list

- **HW:**
  - De1-SoC
  - FDFI CS323HM DDHSL-0 Cable
- **SW (macro, for other programs, see the installation log):**
  - Ubuntu 22.04 LTS
  - Quartus (Quartus Prime 21.1) Lite Edition
  - **Others:** A typed up log of all installations ever done on Hao Lin's Ubuntu 22.04 LTS system during the lifespan of this project is provided in `../Workstation Setup`.

---

# Start Here

- [HW](#hw)
  1. Build the qsys system.
  2. Build the core (see [VexRiscv Core](#vexriscv-core-scala-project)).
  3. Compile the bitstream.
  4. Load the bitstream onto the target board.
- [SW](#sw)
  1. Setup makefile, write platform depenedent c program (with respect to memory mapped vectors of HW resources & pheripherials).
  2. Make and load the binaries into on-chip instruction memory (currently shared with data).
- [Debugging (openOCD+GDB)](#debugging-with-openocdgdb)
- [Custom Instruction Design](-)
  1. Build the generation definitions as a `CustomInstruction` plugin extension.
  2. Accomedate the custom instruction opcode in assembly.
  3. Validate and test the instruction in SW.

## Project Folder Structure

```
├doc                     // documentation resources (for this README)
├vexriscv-master         // local clone of Vexriscv repo
│
...
├openOCD                 // cfg files for openOCD
│├interface
││├...                          // cfg for the JTAG probe used
│└cfg                
││├vexriscv_init.cfg            // sets up parameters that are specific to the VexRiscv configuration of design
││└vexriscv_init.yaml           // Vexriscv design info (generated in Spinal)
├cores                   // generated core sources of the CPU
├firmware                // cmake firmware built from scratch (might be outdated? @Callum?). 
├sw                      // makefile firmware taken from AriesEmbedded project.
│├...
│└out                    // binary output (we are using .mif)
...
# The following are either project files or generated files by Quartus/Qsys
├db
├incremental_db
├output_files
├vexriscv_system    
```

**Note:**
`sw` and `openOCD` is expected to be run from their respective folders (in terminal).

---

# HW

There are three major regions of work:

1. **Vexriscv Core design:** in scala/Spinal, in `./VexRiscv-master`. Generates the CPU HDL & `.tcl` script for qsys.
2. **Qsys Setup:** in qsys, in `./vexriscv_system.qsys`. generates the qsys system for HW synthesis.
3. **Quartus Setup+Toplevel:** in Quartus Prime Project and `toplevel.vhd`. Generates `.sof` bitstream to configure the FPGA.

## Method

If something Goes wrong, check out the [QNA](#quartus-qna) at the bottem of this document.

0. Convince yourself you know:
   - How to use the de1-SoC's cyclone V FPGA part (hint: there is a lot of stuff on the evaluation board, pheriherals etc). We will *not be using the HPS system*.
   - How to deal with Quartus prime.
1. Generate the core in the scala project under `VexRiscv-master` and copy to `cores`.
2. Find and include the core in qsys. Convince yourself of the wirings of the conduits. See the [vexriscv_system.qsys](#qsys-vexriscvsystem).
3. Code up the qsys syetem entity/component and wire up the ports appropriately.

## VexRiscv Core (Scala project)

Core used is a modified version of `GenFullNoMmuMaxPerf` modified as `GenAvalonFullNoMmuMaxPerf`. The generated files is placed under `cores/VexRiscvAvalonMaxPerf` to be included by qsys (to include in Quartus, open `qsys->tools->options` and add the path to `cores/`).

**Notes:**

- Vexriscv is **active low**.

### Before Using the generated HDL core in Qsys

- Define ``define SYNTHESIS` in file.
- Ensure `iorange` and `resetvector` is correcly configured with the software HAL specifications found in `./sw/FpgaConfig.h` (for gpio for example). For **BOTH** the scala and the generated `.v`, check for:
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

### Vectors

0. Vexriscv CPU is **active low**, wire all neglected input ports to ‘0’ (i.e. `Softwareinterrupt`)
    **Note:** interrupt_receive port through the `VexInterruptController.vhd` is threaded through many intermediate signals, but generally: `top-level input -> interrupt_controller -[back out as exception]-> CPU`

1. Assign instruction vector. There are three vectors to be aware of: `ibus_reset`, `ibus_exception`, and `static_io` regions. The SW need to be aware of ibus reset vector, OCRM origin vector for the dual port onchip-RAM (found in `./sw/link.ld`), and the memroy mapped `gpio` vector (instantiated in quartus).

## Qsys: VexRiscv_system

Borrowed & modified from the Aries Embedded project. Some considerations to keep in mind:

- Included a pll for future `clk` changes.
- JTAG interface need to be exported to be connected for [debugging](#debugging-with-openocdgdb).
- The Interrupts (`timerInterrupt`, `externalInterrupt`, `softwareInterrupt`) are currently left open. need to port the software controller if we are going to use the functions that depends on the interrupt.
- **OCRAM:** A single dual port access on-chip memory serves as both the data and the instructions memory. The settings are consistent with the `./sw/link.ld` settings (32K). This memory will be loaded with an `.mif` executable binary in the project root at synthesis.

## TopLevel VHDL

Most documentation in the commenbs. More verbose notes documented here.

### To switch between the aries_embedded core and our own cores:

0. We must do this change because Quartus does not interpert the `.v` description of the AriesEmbedded core correctly with `.qip`.
1. Switch wiring between the cores in Qsys (just enable and disable the cores if the wiring had been done).
   - Might need to reconnect some conduits to get rid of the warnings.
2. If:
   - **Using our own core:**
      1. Switch the included files to the `.qip` for the inclusion of the qsys system.
      2. Include the system as an entity.
   - **Using the `GEN_ARIES_EMBEDDED`**.
      1. Switch the included files to the `.qsys` for the inclusion of the qsys system.
      2. Include the system as an entity.

---

# SW

There are four major regions of work:

1. **Toolchain process:** from c code to executable. Sets up the compilation tools.
2. **Software project Setup:** we are doing "barebone" programming. We'd have to cealry setup the memory mapped pheripherials (OCRAM, gpio etc.) as `.S`, `.h`, `.ld` and `makefile`. So that we actually uses the tools from (1.) to build the executable binaries.
3. **Firmware program Developement:** in c. using the setup of (2.).
4. **Debugging:** in openOCD+GDB directly tapping into the JTAG brideg on the debugger implemented internally by Vexriscv. Also performs validation on our memory usage.
5. ... Any more?

## RISC-V Toolchain

All toolchain is installed under `/opt` such as the `riscv64-unknown-elf-*` [toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain). Everything needed to perform cross-compilation and GDB debugging is included with this package. A prebuilt version of this toolchain is shipped with this repo in `./prebuilt/riscv`.
-  **Includes but is not limited to (in RISC_V flavour):** GNU Compiler Collection (gcc), build tools like make, binUtil, newlib (if you chose to install), glibc, and the GNU Debugger (gdb)
-  The toolchain configuration used is:

```sh
./configure --prefix=/opt/riscv --with-multilib-generator="rv32i-ilp32--;rv32im-ilp32-rv32ima-;rv32imc-ilp32-rv32imac-;rv32imafc-ilp32f--"

sudo make
```

### Toolchain Resources

- [Getting Started with the RISC-V Open Source GNU Toolchain](https://mindchasers.com/dev/rv-getting-started)
- [RISC-V Toolchain Conventions](https://github.com/riscv-non-isa/riscv-toolchain-conventions)

### C Library Choice(s)

Some options:

- newlib
- glibc/libc
- newlib-nanolib/nanolib

## sw Program Folder/C Project

To do.

1. Just `make` the project, dump the `.mif` output into root for the synthesis to load it into the OCRAM.

---

# Debugging with openOCD+GDB

**VERY IMPORTENT Notes:**

- Power off both target board and cable before plugging.
- de1-SoC I/O Interface: 3.3V TTL

## Setup

- Install [openOCD (Vexriscv port)](https://github.com/SpinalHDL/openocd_riscv). Note that only this port will work, as Vexriscv debug module does not adhere to the RISC-V Debug Specification.
- Install the [D2XX drivers](http://ftdichip.com/Drivers/D2XX.htm) for the adaptor.
- (If you already installed the riscv tool chain, the `riscv64-unknown-elf-gdb` application would be under `/opt/riscv/bin`)
- All operations for debugging done in the `./openOCD` folder. Copy the `.elf` executable program that weas loaded onto the FPGA into the folder for use.

> It looks like following this (https://tomverbeure.github.io/2021/07/18/VexRiscv-OpenOCD-and-Traps.html) (and > https://github.com/SpinalHDL/VexRiscv/tree/master/doc/nativeJtag) is the way to go.

**Note:**
- CPU clock must be higher than the JTAG clock

## JTAG Pins Wiring

![](./doc/images/SoC%20GPIO.png)

```vhdl
-- in toplevel:
vexriscvavalon_0_jtag_tck => GPIO_0(1),
vexriscvavalon_0_jtag_tdi => GPIO_0(3),            
vexriscvavalon_0_jtag_tdo => GPIO_0(5),  
vexriscvavalon_0_jtag_tms => GPIO_0(7), 
-- and GND
```

## openOCD Steps

0. Build the sw and relocate `bootrom.elf` into `./openocd`. Make sure the program you want to debug was compiled with debug symbols with `-ggdb3` and lowest optimisation `-O0`.

1. Wire theJATG adaptor to GPIO_0 (above)
    > Sanity Check (find USB): lsusb

2. Open a terminal in `/openOCD` and run:
    - Intially, the commands defined in `vexriscv_init.cfg` will halt the CPU.
  
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


Intially, the commands defined in `vexriscv_init.cfg` will halt the CPU.

3. Open a seperate terminal in the same location as the program's `.elf` executable. Run and start the gdb (or through gdbgui):
   - `.elf` file: Info on the program running on the target.

```sh
# don't print header
# connect to port opened by openOCD
# not the spacing after \ matters
/opt/riscv/bin/riscv64-unknown-elf-gdb -q \
		bootrom.elf \
		-ex "target extended-remote localhost:3333"
```

```sh
gdbgui -g '/opt/riscv/bin/riscv64-unknown-elf-gdb -q bootrom.elf -ex "target extended-remote localhost:3333"'
```

### openOCD Resources:

- [openOCD, Vexrescv and Traps](https://github.com/tomverbeure/vexriscv_ocd_blog)
- [OpenOCD Project Setup](https://openocd.org/doc/html/OpenOCD-Project-Setup.html)

## GDB (RISC-V, shipped with the GNU toolchain)

### GDB Resources:

- [Debugging with GDB](https://sourceware.org/gdb/onlinedocs/gdb/index.html)
- [Tutorial](https://www.usna.edu/Users/cs/lmcdowel/courses/ic220/S20/resources/gdb.html)

### GDBGUI

Install [gdbgui](https://www.gdbgui.com/) via `pip3`.

**Notes:**
- [Register UI Broken (29/08/22)](https://github.com/cs01/gdbgui/issues/406).
- The GUI is greate of looking, not so great for issuing commands (such as breaking the program).
- Currently a bit confused over the `interrupt` and `ctrl+c` halts. Need to inestigate GDB more.
- You can `load` an `.elf` into OCRAM. Launch the gdbgui, load via the GUI first, then input the `load` command via terminal.
- `monitor reset halt` will halt the CPU.
- `target extended-remote localhost:3333`

### GDB Resources

- [gdb QuickStart](https://web.eecs.umich.edu/~sugih/pointers/gdbQS.html)

## SW Accomedation for custom instruction extension

There are three major regions of work:

1. **CustomInstructionPlugin:** from scala design to verilog design. Implements the instructions.
2. **SW Accomedation for Included Instruction:** in assembly macro. Adds machine code for instruction in sw.
   - Combination of riscv assembly and c program passings.

And Read [RISC-V Assembly Programmer's Manual](https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md). And [Riscv Machines Depenednt Asembly Features](https://www.rowleydownload.co.uk/arm/documentation/gnu/as/RISC_002dV_002dDependent.html#RISC_002dV_002dDependent).

### Rational

- **Recall: A machine instruction is used/represented as:**
  - A sequence of binary number. Each divided section represents a particular label (to be passed & used in datapath).
  - A symbolic representatiion (name) of the instruction that can be used in assembly files `.s` or `.S` (wt. preprocessing) to define the code of 'instruction data' to be executed by the CPU.  

**tldr:** After the HW for the instruction is designed, we'd have to compile the SW that uses said instruction. The SW assembler must be made aware of the newly added instruction AND include the representation of this awareness into its application code.

**Two Options:**

  1. Incorperate opcode and instruction encoding into the assembler. Such that the riscv GNU is able to recognise the instruction if used in assembly.
     - Add to gcc (riscv-opcode) to be automatically inferred.
       - [](https://groups.google.com/a/groups.riscv.org/g/sw-dev/c/sL_OHXYj3LY/m/Gsm6sBc9BQAJ)
  2. Incorperate opcode in an assembly code form, defining the usages as a function/macro call that loads the appropriate data into a memory space. Either as:
    - Construct instruction encoding with `.insn` (binutil) directive.
        - > inline assembly is treated as an extension in C. It is conditionally supported and implementation defined, meaning that it may not be present and, even when provided by the implementation, it does not have a fixed meaning.

    - Assembly line `.word` with hex encosing of instruction bit representation. As:
      - a seperate naming label/macro.
      - [Inline assembly](https://en.cppreference.com/w/c/language/asm).
        - [Linux Assembly HOWTO](https://tldp.org/HOWTO/Assembly-HOWTO/gcc.html)
        - [GCC-Inline-Assembly-HOWTO](http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html)

> For example, the instruction ‘add a0, a1, a2’ could be written as ‘.insn r 0x33, 0, 0, a0, a1, a2’
> -- "Using as" User guide to the GNU assembler as (GNU Binutils) version 2.31.

```c
// Example Given by Jim Wilson @https://groups.google.com/a/groups.riscv.org/g/isa-dev/c/fONrcF2lxhQ/m/sb2ZN5Q_AQAJ
rohan:2183$ cat tmp.c

int sub (int a, int b)
{
  int result;
  asm (".insn r 0x33, 0, 0, %0, %1, %2" : "=r" (result) : "r" (a), "r" (b));
  return result;
}

rohan:2184$ riscv32-unknown-elf-gcc -O2 -c tmp.c
rohan:2185$ riscv32-unknown-elf-objdump -dr tmp.o

tmp.o: file format elf32-littleriscv

Disassembly of section .text:

00000000 <sub>:
0: 00b50533 add a0,a0,a1
4: 8082 ret
rohan:2186$ 
```

### Method

  0. Design & implement the custom instruction detapath in HW.
  1. Implement assembly function definitons of the custom instruction.
     1. Define a macro to construct a bit sequence as the instructions.
     2. Under `.text` region, define the name label for the function representing the instruction. 
  2. Use the function in sw.

From [Pass an argument from C to assembly?](https://stackoverflow.com/questions/4330604/pass-an-argument-from-c-to-assembly)
```c
// main.c
extern void myFunc(char * somedata);

void main(){
    myFunc("Hello World");
}
```

```S
// myFunc.S
section .text
    global myFunc
    extern printf

    myFunc:
        push ebp
        mov  ebp, esp

        push dword [ebp+8]
        call printf 

        mov esp, ebp
        pop ebp
        ret
// We have to export myFunc in the assembly file and declare myFunc as an extrnal function in the main.c file. In myFunc.asm we are also importing the printf function from stdlib so that we can output the message as simply as possible.
```

# Quartus QNA:

Q: In Quartus [insert stupidly obvious things] (i.e. syntax error) that doesn't work and you believe everything is fine.

A: Check that Quartus is phasing the HDL in its correct version. Note that **Quartus Lite** will not support `VHDL 2008` and have trouble phasing `.sv`. However, if the code was able to compile before, but cannot now, try restarting Quartus, and if that does not work, restart your computer.

Q: How to add qsys componenets?

A: Adding Search IP from `Assignments --> settings --> IP Search Path (global and/or project)`
Adding Search path to Qsys window: `Tools --> options`

Format is: `/opt/riscv-cores/**/*`

Q: Cannot phase some `.v` file properly!

A: If you have included the qsys via the `.qip`, try removing the inclusiong and included the `.qsys` file directly. Note this will regenerate the qsys system every time you compile.

Q: What's up with `irq_soft` in the AriesEmbedded `irq_controller`?

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

# Useful Tools

- [This is useful for viewing and generating assembly from high-level code](https://godbolt.org/)
- [This is useful for understanding unreadable c](https://cdecl.org/)