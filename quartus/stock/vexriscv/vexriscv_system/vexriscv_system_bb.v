
module vexriscv_system (
	clk_clk,
	gpio_export,
	reset_reset_n,
	vexriscvavalonmaxperf_0_jtag_tms,
	vexriscvavalonmaxperf_0_jtag_tdi,
	vexriscvavalonmaxperf_0_jtag_tdo,
	vexriscvavalonmaxperf_0_jtag_tck,
	vexriscvavalonmaxperf_0_softwareinterrupt_export);	

	input		clk_clk;
	inout	[31:0]	gpio_export;
	input		reset_reset_n;
	input		vexriscvavalonmaxperf_0_jtag_tms;
	input		vexriscvavalonmaxperf_0_jtag_tdi;
	output		vexriscvavalonmaxperf_0_jtag_tdo;
	input		vexriscvavalonmaxperf_0_jtag_tck;
	input		vexriscvavalonmaxperf_0_softwareinterrupt_export;
endmodule
