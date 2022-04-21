// Copyright 2021 Silicon Labs, Inc.
//
// This file, and derivatives thereof are licensed under the
// Solderpad License, Version 2.0 (the "License");
// Use of this file means you agree to the terms and conditions
// of the license and are in full compliance with the License.
// You may obtain a copy of the License at
//
//     https://solderpad.org/licenses/SHL-2.0/
//
// Unless required by applicable law or agreed to in writing, software
// and hardware implementations thereof
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESSED OR IMPLIED.
// See the License for the specific language governing permissions and
// limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// Engineers       Halfdan Bechmann  -   halfdan.bechmann@silabs.com          //
//                                                                            //
// Design Name:    cv32e40x_alu_b_cpop                                        //
// Project Name:   CV32E40X                                                   //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    CPOP for Zbb extension                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


module cv32e40x_alu_b_cpop
  (input  logic [31:0] operand_i,
   output logic [ 5:0] result_o);

  logic [31:0][5:0] sum;

  assign result_o = sum[31];

  generate
    assign sum[0] = {5'h0, operand_i[0]};
    for (genvar i=1; i < 32; i++) begin
      assign sum[i] = sum[i-1] + {5'h0, operand_i[i]};
    end
  endgenerate

endmodule // alu_b_cpop
