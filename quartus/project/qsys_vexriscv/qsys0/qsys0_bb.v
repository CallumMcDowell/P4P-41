
module qsys0 (
	clk_clk,
	gpio_export,
	jtag_tms,
	jtag_tdi,
	jtag_tdo,
	jtag_tck,
	reset_reset_n,
	uart_rxd,
	uart_txd);	

	input		clk_clk;
	inout	[31:0]	gpio_export;
	input		jtag_tms;
	input		jtag_tdi;
	output		jtag_tdo;
	input		jtag_tck;
	input		reset_reset_n;
	input		uart_rxd;
	output		uart_txd;
endmodule
