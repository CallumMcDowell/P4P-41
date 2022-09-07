package vexriscv.demo.P4PCustomInstructions

import spinal.core._
import vexriscv.plugin.Plugin
import vexriscv.{DecoderService, Stageable, VexRiscv}

//This plugin example will add a new instruction named VACC which do the following :
//
// RS1: regfile source, RS2 not used.
// Accumulates sum of 8-bit segments to RD.
//
//Instruction encoding :
//000000000000-----010-----0001011
//     ??|RS2||RS1|   |RD |CUSTOM0_RS1
//
//Note :  RS1, RS2, RD positions follow the RISC-V spec and are common for all instruction of the ISA

class VectorPlugin extends Plugin[VexRiscv]{
  //Define the concept of IS_VMUL signals, which specify if the current instruction is destined for ths plugin
  object IS_VMUL extends Stageable(Bool)

  //Callback to setup the plugin and ask for different services
  override def setup(pipeline: VexRiscv): Unit = {
    import pipeline.config._

    //Retrieve the DecoderService instance
    val decoderService = pipeline.service(classOf[DecoderService])

    //Specify the VMUL default value when instruction are decoded
    decoderService.addDefault(IS_VMUL, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new SIMD_ADD instruction
      key = M"000000000000-----010-----0001011",

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VMUL                  -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> False  //Same than above but for RS2.
      )
    )
  }

  override def build(pipeline: VexRiscv): Unit = {
    import pipeline._
    import pipeline.config._

    //Add a new scope on the execute stage (used to give a name to signals)
    execute plug new Area {
      //Define some signals used internally to the plugin
      val rs1 = execute.input(RS1).asUInt //32 bits UInt value of the regfile[RS1]
      val rd = SInt(32 bits)

      val temp0, temp1, temp2, temp3 = SInt(32 bits)

      temp0 := rs1(7 downto 0).asSInt.resized
      temp1 := rs1(15 downto 8).asSInt.resized
      temp2 := rs1(23 downto 16).asSInt.resized
      temp3 := rs1(31 downto 24).asSInt.resized
      rd := temp0 + temp1 + temp2 + temp3

      //When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMUL)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
  }
}
