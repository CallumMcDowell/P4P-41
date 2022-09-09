
import spinal.core.{UInt, _}

class playground extends Component {

  val io = new Bundle {
    val a = in UInt(32 bits)
    val b = in UInt(32 bits)
    val c = out UInt(32 bits)
  }

  new Area {

    val temp0, temp1, temp2, temp3, tempf = UInt(32 bits)

    // Cascaded summation
    temp0 := (io.a(7 downto 0) + io.a(15 downto 8)).resized
    temp1 := (io.a(23 downto 16) + io.a(31 downto 24)).resized
    io.c := temp0 + temp1
  }
}

object playground {
  // Let's go
  def main(args: Array[String]) {
    SpinalVhdl(new playground)
  }
}

