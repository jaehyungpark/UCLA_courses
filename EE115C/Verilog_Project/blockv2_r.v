`timescale 1ns / 1ps

module blockv2_r(
	input wire A,
	input wire B,
	input wire Cin,
	input wire Sel,
	output wire Cout,
	output wire Z
	);

	wire S;
	wire A_inv;

	// inverter
	assign A_inv = ~A;

	// mirror adder
	ma ma0(.A(A), .B(B), .Cin(Cin), .Cout(Cout), .S(S));
	
	// mux
	s s0(.S0(A_inv), .S1(S), .Sel(Sel), .S(Z));

endmodule
