`timescale 1ns / 1ps

module block_r(
	input wire A,
	input wire B,
	input wire Cin,
	input wire Sel,
	output wire Cout,
	output wire Z
	);

	wire S;

	// adder
	assign S = A^B^Cin;
	assign Cout = A*B+Cin*(A+B);

	// mux
	assign Z = Sel ? S : A;

endmodule
