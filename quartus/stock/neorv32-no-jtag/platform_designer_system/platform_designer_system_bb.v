
module platform_designer_system (
	clk_clk,
	gpio_o_writedata,
	gpio_o_write,
	reset_reset_n);	

	input		clk_clk;
	output	[7:0]	gpio_o_writedata;
	output		gpio_o_write;
	input		reset_reset_n;
endmodule
