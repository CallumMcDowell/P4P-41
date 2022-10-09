package vexriscv.demo

import spinal.core._
import vexriscv.demo.P4PCustomInstructions.VectorPlugin
import vexriscv.ip.{DataCacheConfig, InstructionCacheConfig}
import vexriscv.plugin._
import vexriscv.{VexRiscv, VexRiscvConfig, plugin}
// Avalon imports...
import spinal.lib._
import spinal.lib.bus.avalon.AvalonMM
import spinal.lib.com.jtag.Jtag
import spinal.lib.eda.altera.{InterruptReceiverTag, QSysify, ResetEmitterTag}

// from https://github.com/spinalhdl/vexriscv/issues/224
import spinal.lib.com.jtag.JtagTapInstructionCtrl
import spinal.lib.blackbox.altera.VJTAG

/**
 * Created by cmcd407 on 19.07.22.
 *
 * For use with a DE1-SoC.
 * Based off of VexRiscvAvalonWithIntegratedJtag.scala
 *
 * Note: M10K block RAM instances have a two cycle read delay.
 */

object GenAvalonFullNoMmuMaxPerf {
  def main(args: Array[String]) {
    val verilogReport = SpinalVerilog {
      // CPU Configuration
      val cpuConfig = VexRiscvConfig(
        plugins = List(
          // Deprecated, does nothing
//          new PcManagerSimplePlugin(
//            resetVector = 0x80000000l, // TODO: SET 0x0..0!?
//            relaxedPcCalculation = false
//          ),
//          new SimdAddPlugin,
          new VectorPlugin,
          new IBusCachedPlugin(
            resetVector = 0x00000010l, // Set to same reset vector expected in sw
            prediction = DYNAMIC_TARGET,
            historyRamSizeLog2 = 8,
            config = InstructionCacheConfig(
              cacheSize = 4096*2,
              bytePerLine =32,
              wayCount = 1,
              addressWidth = 32,
              cpuDataWidth = 32,
              memDataWidth = 32,
              catchIllegalAccess = true,
              catchAccessFault = true,
              asyncTagMemory = false,
              twoCycleRam = true, // DE1-SoC M10K RAM has a two cycle delay
              twoCycleCache = true
            )
          ),
          new DBusCachedPlugin(
            config = new DataCacheConfig(
              cacheSize         = 4096*2,
              bytePerLine       = 32,
              wayCount          = 1,
              addressWidth      = 32,
              cpuDataWidth      = 32,
              memDataWidth      = 32,
              catchAccessError  = true,
              catchIllegal      = true,
              catchUnaligned    = true
            )
          ),
          new StaticMemoryTranslatorPlugin(
            ioRange      = _(31 downto 28) === 0xF
          ),
          new DecoderSimplePlugin(
            catchIllegalInstruction = true
          ),
          new RegFilePlugin(
            regFileReadyKind = plugin.SYNC,
            zeroBoot = false
          ),
          new IntAluPlugin,
          new SrcPlugin(
            separatedAddSub = false,
            executeInsertion = true
          ),
          new FullBarrelShifterPlugin(earlyInjection = true),
          new HazardSimplePlugin(
            bypassExecute           = true,
            bypassMemory            = true,
            bypassWriteBack         = true,
            bypassWriteBackBuffer   = true,
            pessimisticUseSrc       = false,
            pessimisticWriteRegFile = false,
            pessimisticAddressMatch = false
          ),
          new MulPlugin,
          new DivPlugin,
          new CsrPlugin(CsrPluginConfig.all),
          new DebugPlugin(ClockDomain.current.clone(reset = Bool().setName("debugReset"))),
          new BranchPlugin(
            earlyBranch = false,
            catchAddressMisaligned = true
          ),
          new YamlPlugin("vexriscv_init.yaml")
        )
      )

      // CPU Instantiation
      val cpu = new VexRiscv(cpuConfig)

      // CPU Tweaking for Avalon
      cpu.setDefinitionName("VexRiscvAvalonMaxPerf")
      cpu.rework {
        var iBus : AvalonMM = null
        for (plugin <- cpuConfig.plugins) plugin match {
          case plugin: IBusSimplePlugin => {
            plugin.iBus.setAsDirectionLess() // Unset IO properties of iBus
            iBus = master(plugin.iBus.toAvalon())
              .setName("iBusAvalon")
              .addTag(ClockDomainTag(ClockDomain.current)) // Specify a clock domain to the iBus (used by QSysify)
          }
          case plugin: IBusCachedPlugin => {
            plugin.iBus.setAsDirectionLess() // Unset IO properties of iBus
            iBus = master(plugin.iBus.toAvalon())
              .setName("iBusAvalon")
              .addTag(ClockDomainTag(ClockDomain.current)) // Specify a clock domain to the iBus (used by QSysify)
          }
          case plugin: DBusSimplePlugin => {
            plugin.dBus.setAsDirectionLess()
            master(plugin.dBus.toAvalon())
              .setName("dBusAvalon")
              .addTag(ClockDomainTag(ClockDomain.current))
          }
          case plugin: DBusCachedPlugin => {
            plugin.dBus.setAsDirectionLess()
            master(plugin.dBus.toAvalon())
              .setName("dBusAvalon")
              .addTag(ClockDomainTag(ClockDomain.current))
          }
          case plugin: DebugPlugin => plugin.debugClockDomain {
            plugin.io.bus.setAsDirectionLess()
            
            // Virtual JTAG Wrapper for Altera
            // Does not seem to work. Need to validate once software build chain is up.
            // val tap = new VJTAG()
            // val jtagCtrl = tap.toJtagTapInstructionCtrl()
            // jtagCtrl <> plugin.io.bus.fromJtagInstructionCtrl(
            //   jtagClockDomain=ClockDomain(tap.tck),
            //   jtagHeaderIgnoreWidth=1)
            
            val jtag = slave(new Jtag())
              .setName("jtag")
            jtag <> plugin.io.bus.fromJtag()
            plugin.io.resetOut
              .addTag(ResetEmitterTag(plugin.debugClockDomain))
              .parent = null // Avoid the io bundle to be interpreted as a QSys conduit
          }
          case _ =>
        }
        for (plugin <- cpuConfig.plugins) plugin match {
          case plugin: CsrPlugin => {
            plugin.externalInterrupt
              .addTag(InterruptReceiverTag(iBus, ClockDomain.current))
            plugin.timerInterrupt
              .addTag(InterruptReceiverTag(iBus, ClockDomain.current))
          }
          case _ =>
        }
      }
      cpu
    }

    // Generate Qsys .tcl File for the CPU
    QSysify(verilogReport.toplevel)
  }
}
