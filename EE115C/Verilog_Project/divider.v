`timescale 1ns / 1ps

module divider(
	input wire [7:0] A,
	input wire [7:0] B,
	output wire [7:0] Q,
	output wire [7:0] R
	);

	assign Q = B == 0 ? 255 : A/B;
	assign R = A - Q*B;

endmodule
