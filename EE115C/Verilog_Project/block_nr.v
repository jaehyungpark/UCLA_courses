`timescale 1ns / 1ps

module block_nr(
	input wire A,
	input wire B,
	input wire Cin,
	input wire Sel,
	output wire Cout,
	output wire Z
	);

	wire X;

	// adder
	assign Z = A^X^Cin;
	assign Cout = A*X+Cin*(A+X);

	// XOR
	assign X = B^Sel;

endmodule
