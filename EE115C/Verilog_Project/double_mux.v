`timescale 1ns / 1ps

module cs(
	input wire C1,
	input wire S1,
	input wire C0,
	input wire S0,
	input wire Sel,
	output wire C,
	output wire S
	);

	assign C = Sel ? C1 : C0;
	assign S = Sel ? S1 : S0;

endmodule
