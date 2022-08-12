# ArieEmbedded Wrapper Adoptation

This is the project implementing a simple Vexriscv system through qsys with the generated `.v` file and `tcl` component from `Qsysfy()`.

## Structure

```
vexriscv-master
│
...
├cores                  // generated core sources of the CPU
├firmware               // cmake firmware built from scratch. 
├sw                     // makefile firmware taken from AriesEmbedded project.
│└out                   // binary output (we are using .mif)
```

## VexRiscv Core (Scala project)

Core used is a modified version of `GenFullNoMmuMaxPerf` modified as `GenAvalonFullNoMmuMaxPerf`. The generated files is placed under `cores/VexRiscvAvalonMaxPerf` to be included by qsys (to include in Quartus, open `qsys->tools->options` and add the path to `cores/`).

### Before Using the generated core in Qsys

- Comment out `urand` under the `SYSTHESIS` blocks in the generated `.v` file. 
- Ensure `iorange` and `resetvector` is correcly configured with the software HAL specifications (for gpio for example). Check both the scala and the generated `.v` for:
  - SimplePlugin
    - C_RESET_VECTOR
      - `IBusSimplePlugin_fetchPc_pcReg`
    - C_EXCEPTION_VECTOR
      - `CsrPlugin_mtvec_mode`
      - `CsrPlugin_mtvec_base`
  - Cached Plugin
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

## Quartus QNA:

Q: Quartus throws syntax error, everything is fine!!
A: Check that Quartus is phasing the HDL in its correct version. Note that Quartus Lite will not support `VHDL 2008` and have trouble phasing `.sv`. However, if the code was able to compile before, but cannot now, try restarting Quartus, and if that does not work, restart your computer.

Q: Cannot phase some `.v` file properly!
A: If you have included the qsys via the `.qip`, try removing the inclusiong and included the `.qsys` file directly. Note this will regenerate the qsys system every time you compile.