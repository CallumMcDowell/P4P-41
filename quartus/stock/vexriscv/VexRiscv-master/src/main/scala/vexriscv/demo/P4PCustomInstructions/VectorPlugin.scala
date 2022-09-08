package vexriscv.demo.P4PCustomInstructions

import spinal.core._
import vexriscv.plugin.Plugin
import vexriscv.{DecoderService, Stageable, VexRiscv}

/* Vector Plugin

32-bit or 4*8-bit segments vector elements.

This plugin will add the following new instructions:

-  R-Type
  - VMUL: Element-wise vector-vector multiplication
    - Overflow undefined.
    - ret: (32-bit) vector of 4*8-bit segments
  - VACC: Accumulate (8-bit) vector contents
    - ret: (32-bit) scalar
  - VMAXE: Find largest element in vector
    - ret: (32-bit) scalar
  - VMINE: Find smallest element in vector
    - ret: (32-bit) scalar
  - VMAX.X: Element-wise vector-scalar comparision to find larger of the two for that position
    - ret: (32-bit) vector of 4*8-bit segments

- I-Type
  - VSRLI: Element-wise vector-immediate logical shift right.
    - ret: (32-bit) vector of 4*-*8-bit segments.
*/

// Note :  RS1, RS2, RD positions follow the RISC-V spec and are common for all instruction of the ISA

class VectorPlugin extends Plugin[VexRiscv]{

  // Opcode decoding formats (from unprivileged base map of RVG calling convention)
  // | reservation | (inst[6:2]) |
  // | ----------- | ----------- |
  // |  custom-0   |   (01010)   |
  // |  custom-1   |   (01001)   |
  // The inst[14:12] A.K.A. func3 still act as operation distinguishing field
  def OP_CODE_VACC       = M"000000000000-----010-----0001011"
  def OP_CODE_VMAXE      = M"000000000000-----011-----0001011"
  def OP_CODE_VMINE      = M"000000000000-----100-----0001011"
  def CUSTOM0_RD_RS1     = M"-----------------110-----0001011"
  def CUSTOM0_RD_RS1_RS2 = M"-----------------111-----0001011"
  def CUSTOM1            = M"-----------------000-----0101011"
  def CUSTOM1_RS1        = M"000000000000-----010-----0101011"
  def CUSTOM1_RS1_RS2    = M"-----------------011-----0101011"
  def CUSTOM1_RD         = M"-----------------100-----0101011"
  def CUSTOM1_RD_RS1     = M"-----------------110-----0101011"
  def CUSTOM1_RD_RS1_RS2 = M"-----------------111-----0101011"

  /*
  custom-2, custom-3 Reservations

  def CUSTOM2            = M"-----------------000-----1011011"
  def CUSTOM2_RS1        = M"-----------------010-----1011011"
  def CUSTOM2_RS1_RS2    = M"-----------------011-----1011011"
  def CUSTOM2_RD         = M"-----------------100-----1011011"
  def CUSTOM2_RD_RS1     = M"-----------------110-----1011011"
  def CUSTOM2_RD_RS1_RS2 = M"-----------------111-----1011011"
  def CUSTOM3            = M"-----------------000-----1111011"
  def CUSTOM3_RS1        = M"-----------------010-----1111011"
  def CUSTOM3_RS1_RS2    = M"-----------------011-----1111011"
  def CUSTOM3_RD         = M"-----------------100-----1111011"
  def CUSTOM3_RD_RS1     = M"-----------------110-----1111011"
  def CUSTOM3_RD_RS1_RS2 = M"-----------------111-----1111011"
*/

  //Define the concept of IS_VACC signals, which specify if the current instruction is destined for ths plugin
  object IS_VACC extends Stageable(Bool)
  object IS_VMAXE extends Stageable(Bool)

  //Callback to setup the plugin and ask for different services
  override def setup(pipeline: VexRiscv): Unit = {
    import pipeline.config._

    //Retrieve the DecoderService instance
    val decoderService = pipeline.service(classOf[DecoderService])

    /*
    --------------------------------------------------------------
      VACC
    --------------------------------------------------------------
      RS1: regfile source, RS2 not used.
      Accumulates sum of 8-bit segments (signed) to RD.

      Instruction encoding :
      000000000000-----010-----0001011
           ??|RS2||RS1|   |RD |CUSTOM0_RS1
    --------------------------------------------------------------
    */

    //Specify the VACC default value when instruction are decoded
    decoderService.addDefault(IS_VACC, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = OP_CODE_VACC,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VACC                  -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> False  //Same than above but for RS2.
      )
    )

    /*
    --------------------------------------------------------------
      VMAXE
    --------------------------------------------------------------
      RS1: regfile source, RS2 not used.
      Finds largest 8-bit segment (signed) to RD

      Instruction encoding :
      000000000000-----011-----0001011
           ??|RS2||RS1|   |RD |CUSTOM0_RS1
    --------------------------------------------------------------
    */

    //Specify the VACC default value when instruction are decoded
    decoderService.addDefault(IS_VMAXE, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM1_RS1,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VMAXE                 -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> False  //Same than above but for RS2.
      )
    )

    /*
    --------------------------------------------------------------
      VMINE
    --------------------------------------------------------------
      RS1: regfile source, RS2 not used.
      Finds lowest 8-bit segment (signed) to RD

      Instruction encoding :
      000000000000-----100-----0001011
           ??|RS2||RS1|   |RD |CUSTOM0_RS1
    --------------------------------------------------------------
    */

  }

  override def build(pipeline: VexRiscv): Unit = {
    import pipeline._
    import pipeline.config._

    /*
    --------------------------------------------------------------
    VACC
    --------------------------------------------------------------
    */
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
      when(execute.input(IS_VACC)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
    /*
    --------------------------------------------------------------
    VMAXE
    --------------------------------------------------------------
    */
    //Add a new scope on the execute stage (used to give a name to signals)
    execute plug new Area {
      //Define some signals used internally to the plugin
      val rs1 = execute.input(RS1).asUInt //32 bits UInt value of the regfile[RS1]
      val rd = SInt(32 bits)

      val temp0, temp1, temp2, temp3, largest01, largest23 = SInt(32 bits)

      val comp01, comp23, compFinal = Bool

      temp0 := rs1(7 downto 0).asSInt.resized
      temp1 := rs1(15 downto 8).asSInt.resized
      temp2 := rs1(23 downto 16).asSInt.resized
      temp3 := rs1(31 downto 24).asSInt.resized

      // Select signals for mux
      comp01 := temp0 >= temp1
      comp23 := temp2 >= temp3

      largest01 := comp01 ? temp0 | temp1
      largest23 := comp23 ? temp2 | temp3

      compFinal := largest01 >= largest23
      rd := compFinal ? largest01 | largest23

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMAXE)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
  }
}
