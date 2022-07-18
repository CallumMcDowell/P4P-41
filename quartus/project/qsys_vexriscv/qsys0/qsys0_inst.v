	qsys0 u0 (
		.clk_clk       (<connected-to-clk_clk>),       //   clk.clk
		.gpio_export   (<connected-to-gpio_export>),   //  gpio.export
		.jtag_tms      (<connected-to-jtag_tms>),      //  jtag.tms
		.jtag_tdi      (<connected-to-jtag_tdi>),      //      .tdi
		.jtag_tdo      (<connected-to-jtag_tdo>),      //      .tdo
		.jtag_tck      (<connected-to-jtag_tck>),      //      .tck
		.reset_reset_n (<connected-to-reset_reset_n>), // reset.reset_n
		.uart_rxd      (<connected-to-uart_rxd>),      //  uart.rxd
		.uart_txd      (<connected-to-uart_txd>)       //      .txd
	);

