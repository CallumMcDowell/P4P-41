// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// !!! cv32e40x_sim_clock_gate file is meant for simulation only !!!
// !!! It must not be used for ASIC synthesis                    !!!
// !!! It must not be used for FPGA synthesis                    !!!

// File is based on bhv/csv32e40x_sim_clock_gate.sv.
// Below is a naive placeholder implementation added for the purposes of synthesis and
// testing on FPGA. Cyclone V specific IP may be more appropriate.

module cv32e40x_clock_gate
#(
  parameter LIB = 0
  )
(
    input  logic clk_i,
    input  logic en_i,
    input  logic scan_cg_en_i,
    output logic clk_o
  );

  always_comb
  begin
    clk_o <= clk_i & (en_i | scan_cg_en_i);  
  end

  // logic clk_en;

  // always_latch
  // begin
  //    if (clk_i == 1'b0)
  //      clk_en <= en_i | scan_cg_en_i;
  // end

  // assign clk_o = clk_i & clk_en;

endmodule // cv32e40x_clock_gate
