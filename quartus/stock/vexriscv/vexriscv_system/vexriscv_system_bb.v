
module vexriscv_system (
	clk_clk,
	reset_reset_n,
	vexriscvavalonmaxperf_0_softwareinterrupt_export,
	vexriscvavalonmaxperf_0_jtag_tms,
	vexriscvavalonmaxperf_0_jtag_tdi,
	vexriscvavalonmaxperf_0_jtag_tdo,
	vexriscvavalonmaxperf_0_jtag_tck);	

	input		clk_clk;
	input		reset_reset_n;
	input		vexriscvavalonmaxperf_0_softwareinterrupt_export;
	input		vexriscvavalonmaxperf_0_jtag_tms;
	input		vexriscvavalonmaxperf_0_jtag_tdi;
	output		vexriscvavalonmaxperf_0_jtag_tdo;
	input		vexriscvavalonmaxperf_0_jtag_tck;
endmodule
