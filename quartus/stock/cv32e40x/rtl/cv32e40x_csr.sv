// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * Control / status register primitive
 */


module cv32e40x_csr #(
    parameter int unsigned    WIDTH      = 32,
    parameter bit             SHADOWCOPY = 1'b0,
    parameter bit [WIDTH-1:0] RESETVALUE = '0
 ) (
    input  logic             clk,
    input  logic             rst_n,

    input  logic [WIDTH-1:0] wr_data_i,
    input  logic             wr_en_i,
    output logic [WIDTH-1:0] rd_data_o,

    output logic             rd_error_o
);

  logic [WIDTH-1:0] rdata_q;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      rdata_q <= RESETVALUE;
    end else if (wr_en_i) begin
      rdata_q <= wr_data_i;
    end
  end

  assign rd_data_o = rdata_q;

  if (SHADOWCOPY) begin : gen_shadow
    logic [WIDTH-1:0] shadow_q;

    always_ff @(posedge clk or negedge rst_n) begin
      if (!rst_n) begin
        shadow_q <= ~RESETVALUE;
      end else if (wr_en_i) begin
        shadow_q <= ~wr_data_i;
      end
    end

    assign rd_error_o = rdata_q != ~shadow_q;

  end else begin : gen_no_shadow
    assign rd_error_o = 1'b0;
  end


endmodule
