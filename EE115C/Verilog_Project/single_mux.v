`timescale 1ns / 1ps

module s(
	input wire S0,
	input wire S1,
	input wire Sel,
	output wire S
	);

	assign S = Sel ? S1 : S0;

endmodule
