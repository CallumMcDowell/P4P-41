package vexriscv.demo.P4PCustomInstructions

import spinal.core._
import vexriscv.Riscv.IMM
import vexriscv.plugin.Plugin
import vexriscv.{DecoderService, Stageable, VexRiscv}

import scala.collection.mutable.ListBuffer

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
    - ret: (32-bit) vector of 4*8-bit segments.
*/

// Note :  RS1, RS2, RD positions follow the RISC-V spec and are common for all instruction of the ISA

class VectorPlugin extends Plugin[VexRiscv]{

  // Opcode decoding formats (from unprivileged base map of RVG calling convention)
  // | reservation | (inst[6:2]) |
  // | ----------- | ----------- |
  // |  custom-0   |   (01010)   |
  // |  custom-1   |   (01001)   |
  // The inst[14:12] A.K.A. func3 still act as operation distinguishing field
  def CUSTOM0_RS1        = M"000000000000-----010-----0001011" //
  def CUSTOM0_RS1_RS2    = M"-----------------011-----0001011"
  def CUSTOM0_RD         = M"-----------------100-----0001011"
  def CUSTOM0_RD_RS1     = M"-----------------110-----0001011"
  def CUSTOM0_RD_RS1_RS2 = M"-----------------111-----0001011"
  def CUSTOM1            = M"-----------------000-----0101011"
  def CUSTOM1_RS1_VMAXE  = M"0000000----------010-----0101011" //
  def CUSTOM1_RS1_VMINE  = M"0000001----------010-----0101011" //
  def CUSTOM1_RS1_RS2    = M"-----------------011-----0101011" //
  def CUSTOM1_RD         = M"-----------------100-----0101011"
  def CUSTOM1_RD_RS1     = M"-----------------110-----0101011"
  def CUSTOM1_RD_RS1_RS2 = M"-----------------111-----0101011"

  /*
  custom-2, custom-3 Reservations
  def CUSTOM0            = M"-----------------000-----0001011"
  def CUSTOM0_RS1        = M"-----------------010-----0001011"
  def CUSTOM0_RS1_RS2    = M"-----------------011-----0001011"
  def CUSTOM0_RD         = M"-----------------100-----0001011"
  def CUSTOM0_RD_RS1     = M"-----------------110-----0001011"
  def CUSTOM0_RD_RS1_RS2 = M"-----------------111-----0001011"
  def CUSTOM1            = M"-----------------000-----0101011"
  def CUSTOM1_RS1        = M"-----------------010-----0101011"
  def CUSTOM1_RS1_RS2    = M"-----------------011-----0101011"
  def CUSTOM1_RD         = M"-----------------100-----0101011"
  def CUSTOM1_RD_RS1     = M"-----------------110-----0101011"
  def CUSTOM1_RD_RS1_RS2 = M"-----------------111-----0101011"
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
  object IS_VMUL extends Stageable(Bool)
  object IS_VACC extends Stageable(Bool)
  object IS_VMAXE extends Stageable(Bool)
  object IS_VMINE extends Stageable(Bool)
  object IS_VMAX_X extends Stageable(Bool)
  object IS_VSLRI extends  Stageable(Bool)

  //Callback to setup the plugin and ask for different services
  override def setup(pipeline: VexRiscv): Unit = {
    import pipeline.config._

    //Retrieve the DecoderService instance
    val decoderService = pipeline.service(classOf[DecoderService])

    /*
    --------------------------------------------------------------
      VMUL
    --------------------------------------------------------------
      RS1: regfile source, vector of 4*8-bit elements
      RS2: regfile source, vector of 4*8-bit elements
      Element-wise vector-vector multiplication, returning an vector of
      (in 32-bit register) the results for each position at RD.

      Overflow undertermined, expects valid range of inputs.

      Instruction encoding :
      -----------------111-----0001011
        func7|RS2||RS1|   |RD |CUSTOM1_RS1
    --------------------------------------------------------------
    */

    //Specify the default value when instruction are decoded
    decoderService.addDefault(IS_VMUL, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM0_RD_RS1_RS2,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VMUL                -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> True  //Same than above but for RS2.
      )
    )

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
      key = CUSTOM0_RS1,

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
      0000000----------010-----0101011
        func7|RS2||RS1|   |RD |CUSTOM1_RS1
    --------------------------------------------------------------
    */

    //Specify the default value when instruction are decoded
    decoderService.addDefault(IS_VMAXE, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM1_RS1_VMAXE,

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
      0000001----------010-----0101011
        func7|RS2||RS1|   |RD |CUSTOM1_RS1
    --------------------------------------------------------------
    */

    //Specify the default value when instruction are decoded
    decoderService.addDefault(IS_VMINE, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM1_RS1_VMINE,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VMINE                 -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> False  //Same than above but for RS2.
      )
    )

    /*
    --------------------------------------------------------------
      VMAX.X (VMAX_X)
    --------------------------------------------------------------
      RS1: regfile source, vector of 4*8-bit elements
      RS2: regfile source, vector of 4*8-bit elements
      Compare each element for largest, returning an vector of
      (in 32-bit register) the results for each position at RD.

      Instruction encoding :
      -----------------111-----0101011
        func7|RS2||RS1|   |RD |CUSTOM1_RD_RS1_RS2
    --------------------------------------------------------------
    */

    //Specify the default value when instruction are decoded
    decoderService.addDefault(IS_VMAX_X, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM1_RD_RS1_RS2,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VMAX_X                -> True,
        REGFILE_WRITE_VALID      -> True, //Enable the register file write
        BYPASSABLE_EXECUTE_STAGE -> True, //Notify the hazard management unit that the instruction result is already accessible in the EXECUTE stage (Bypass ready)
        BYPASSABLE_MEMORY_STAGE  -> True, //Same as above but for the memory stage
        RS1_USE                  -> True, //Notify the hazard management unit that this instruction use the RS1 value
        RS2_USE                  -> False //Same than above but for RS2.
      )
    )

    /*
    --------------------------------------------------------------
      VSLRI
    --------------------------------------------------------------
      RS1: regfile source, vector of 4*8-bit elements
      RS2: not used
      Imm[11:0]: Left shift amount (0 to 4)
      Performs element-wise logical shift left, returning an vector of
      (in 32-bit register) the results for each position at RD.

      Instruction encoding :
      -----------------110-----0001011
        Immediate||RS1|   |RD |CUSTOM0_RD_RS1
    --------------------------------------------------------------
    */

    //Specify the default value when instruction are decoded
    decoderService.addDefault(IS_VSLRI, False)

    //Specify the instruction decoding which should be applied when the instruction match the 'key' parttern
    decoderService.add(
      //Bit pattern of the new instruction
      key = CUSTOM0_RD_RS1,

      //Decoding specification when the 'key' pattern is recognized in the instruction
      List(
        IS_VSLRI                 -> True,
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

    /*
    --------------------------------------------------------------
    VMUL
    --------------------------------------------------------------
    */
    //Add a new scope on the execute stage (used to give a name to signals)
    execute plug new Area {
      //Define some signals used internally to the plugin
      val rs1 = execute.input(RS1).asUInt //32 bits UInt value of the regfile[RS1]
      val rs2 = execute.input(RS2).asUInt //32 bits UInt value of the regfile[RS1]
      val rd = UInt(32 bits)

      val rs1_vec = rs1.subdivideIn(8 bits)
      val rs2_vec = rs2.subdivideIn(8 bits)

      val elem = ListBuffer(UInt(0 bits))

      // Reviewed from here: https://stackoverflow.com/questions/61492744/scala-compare-elements-at-same-position-in-two-arrays
      // Tuple of slice pair
      rs1_vec.zip(rs2_vec).foreach {
        case (rs1, rs2) =>
          val result = rs1.asSInt.resize(4 bits) * rs2(3 downto 0).asSInt.resize(4 bits)
          elem.append(result.resize(8 bits).asUInt)
      }

      // Concat over iterable
      rd := Cat(elem).asUInt

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMUL)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }

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

      val rs1_vec = rs1.subdivideIn(8 bits)
      val elems = ListBuffer(UInt(0 bits))

      var sum = S(0, 32 bits)

      // Element-wise shift
      rs1_vec.foreach {
        case (rs1) =>
          sum = sum + rs1.asSInt.resized
      }
//      temp0 := rs1(7 downto 0).asSInt.resized
//      temp1 := rs1(15 downto 8).asSInt.resized
//      temp2 := rs1(23 downto 16).asSInt.resized
//      temp3 := rs1(31 downto 24).asSInt.resized
      rd := sum

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
      compFinal := largest01 >= largest23

      largest01 := comp01 ? temp0 | temp1
      largest23 := comp23 ? temp2 | temp3

      rd := compFinal ? largest01 | largest23

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMAXE)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
    /*
    --------------------------------------------------------------
    VMINE
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
      comp01 := temp0 <= temp1
      comp23 := temp2 <= temp3
      compFinal := largest01 <= largest23

      largest01 := comp01 ? temp0 | temp1
      largest23 := comp23 ? temp2 | temp3

      rd := compFinal ? largest01 | largest23

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMINE)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
    /*
    --------------------------------------------------------------
    VMAX.X
    --------------------------------------------------------------
    */
    //Add a new scope on the execute stage (used to give a name to signals)
    execute plug new Area {
      //Define some signals used internally to the plugin
      val rs1 = execute.input(RS1).asUInt //32 bits UInt value of the regfile[RS1]
      val rs2 = execute.input(RS2).asUInt //32 bits UInt value of the regfile[RS1]
      val rd = UInt(32 bits)

      val rs1_vec = rs1.subdivideIn(8 bits)
      val rs2_vec = rs2.subdivideIn(8 bits)

      val largest_elems = ListBuffer(UInt(0 bits))

      // Reviewed from here: https://stackoverflow.com/questions/61492744/scala-compare-elements-at-same-position-in-two-arrays
      // Tuple of slice pair
      rs1_vec.zip(rs2_vec).foreach {
        case (rs1, rs2) =>
          val comp = rs1.asSInt >= rs2.asSInt
          largest_elems.append(comp ? rs1 | rs2)
      }

      // Concat over iterable
      rd := Cat(largest_elems).asUInt

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VMAX_X)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
    /*
    --------------------------------------------------------------
    VSLRI
    --------------------------------------------------------------
    */
    //Add a new scope on the execute stage (used to give a name to signals)
    execute plug new Area {
      //Define some signals used internally to the plugin
      val rs1 = execute.input(RS1).asUInt //32 bits UInt value of the regfile[RS1]
      val imm = IMM(execute.input(INSTRUCTION)).i.asUInt //11-bits UInt immediate value
      val rd = UInt(32 bits)

      val rs1_vec = rs1.subdivideIn(8 bits)
      val elems = ListBuffer(UInt(0 bits))

      // Element-wise shift
      rs1_vec.foreach {
        case (rs1) =>
          elems.append(rs1 |<< imm)
      }

      rd := Cat(elems).asUInt

      // When the instruction is a SIMD_ADD one, then write the result into the register file data path.
      when(execute.input(IS_VSLRI)) {
        execute.output(REGFILE_WRITE_DATA) := rd.asBits
      }
    }
  }
}
