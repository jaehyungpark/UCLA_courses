`timescale 1ns / 1ps

module ma(
	input wire A,
	input wire B,
	input wire Cin,
	output wire Cout,
	output wire S
	);

	// adder
	assign S = ~(A^B^Cin);
	assign Cout = ~(A*B+Cin*(A+B));

endmodule
