`timescale 1ns / 1ps

module rowv3_r(
	input wire [7:0] A,
	input wire [7:0] B,
	input wire Cin,
	input wire Sel,
	output wire Cout,
	output wire [7:0] Z
	);
	
	wire [7:0] A_inv;
	// output wires of components
	wire [7:0] C [6:0];
	wire [7:0] S [6:0];
	
	assign A_inv = ~A;
	
	// row 0
	ma	ma00(.A(A[0]), .B(B[0]), .Cin(Cin), .Cout(C[0][0]), .S(S[0][0]));
	ma	ma01(.A(A[1]), .B(B[1]), .Cin(1'b0), .Cout(C[0][1]), .S(S[0][1]));
	ma	ma02(.A(A[2]), .B(B[2]), .Cin(1'b0), .Cout(C[0][2]), .S(S[0][2]));
	ma	ma03(.A(A[3]), .B(B[3]), .Cin(1'b0), .Cout(C[0][3]), .S(S[0][3]));
	ma	ma04(.A(A[4]), .B(B[4]), .Cin(1'b0), .Cout(C[0][4]), .S(S[0][4]));
	ma	ma05(.A(A[5]), .B(B[5]), .Cin(1'b0), .Cout(C[0][5]), .S(S[0][5]));
	ma	ma06(.A(A[6]), .B(B[6]), .Cin(1'b0), .Cout(C[0][6]), .S(S[0][6]));
	ma	ma07(.A(A[7]), .B(B[7]), .Cin(1'b0), .Cout(C[0][7]), .S(S[0][7]));

	// row 1
	ma	ma11(.A(A[1]), .B(B[1]), .Cin(1'b1), .Cout(C[1][1]), .S(S[1][1]));
	ma	ma12(.A(A[2]), .B(B[2]), .Cin(1'b1), .Cout(C[1][2]), .S(S[1][2]));
	ma	ma13(.A(A[3]), .B(B[3]), .Cin(1'b1), .Cout(C[1][3]), .S(S[1][3]));
	ma	ma14(.A(A[4]), .B(B[4]), .Cin(1'b1), .Cout(C[1][4]), .S(S[1][4]));
	ma	ma15(.A(A[5]), .B(B[5]), .Cin(1'b1), .Cout(C[1][5]), .S(S[1][5]));
	ma	ma16(.A(A[6]), .B(B[6]), .Cin(1'b1), .Cout(C[1][6]), .S(S[1][6]));
	ma	ma17(.A(A[7]), .B(B[7]), .Cin(1'b1), .Cout(C[1][7]), .S(S[1][7]));

	// row 2
	cs cs21(.C1(C[1][1]), .S1(S[1][1]), .C0(C[0][1]), .S0(S[0][1]), .Sel(C[0][0]), .C(C[2][1]), .S(S[2][1]));
	cs cs23(.C1(C[1][3]), .S1(S[1][3]), .C0(C[0][3]), .S0(S[0][3]), .Sel(C[0][2]), .C(C[2][3]), .S(S[2][3]));
	cs cs25(.C1(C[1][5]), .S1(S[1][5]), .C0(C[0][5]), .S0(S[0][5]), .Sel(C[0][4]), .C(C[2][5]), .S(S[2][5]));
	cs cs27(.C1(C[1][7]), .S1(S[1][7]), .C0(C[0][7]), .S0(S[0][7]), .Sel(C[0][6]), .C(C[2][7]), .S(S[2][7]));

	// row 3
	cs cs33(.C1(C[1][3]), .S1(S[1][3]), .C0(C[0][3]), .S0(S[0][3]), .Sel(C[1][2]), .C(C[3][3]), .S(S[3][3]));
	cs cs35(.C1(C[1][5]), .S1(S[1][5]), .C0(C[0][5]), .S0(S[0][5]), .Sel(C[1][4]), .C(C[3][5]), .S(S[3][5]));
	cs cs37(.C1(C[1][7]), .S1(S[1][7]), .C0(C[0][7]), .S0(S[0][7]), .Sel(C[1][6]), .C(C[3][7]), .S(S[3][7]));

	// row 4
	s s42(.S1(S[1][2]), .S0(S[0][2]), .Sel(C[2][1]), .S(S[4][2]));
	cs cs43(.C1(C[3][3]), .S1(S[3][3]), .C0(C[2][3]), .S0(S[2][3]), .Sel(C[2][1]), .C(C[4][3]), .S(S[4][3]));
	s s46(.S1(S[1][6]), .S0(S[0][6]), .Sel(C[2][5]), .S(S[4][6]));
	cs cs47(.C1(C[3][7]), .S1(S[3][7]), .C0(C[2][7]), .S0(S[2][7]), .Sel(C[2][5]), .C(C[4][7]), .S(S[4][7]));

	// row 5
	s s56(.S1(S[1][6]), .S0(S[0][6]), .Sel(C[3][5]), .S(S[5][6]));
	cs cs57(.C1(C[3][7]), .S1(S[3][7]), .C0(C[2][7]), .S0(S[2][7]), .Sel(C[3][5]), .C(C[5][7]), .S(S[5][7]));

	// row 6
	s s64(.S1(S[1][4]), .S0(S[0][4]), .Sel(C[4][3]), .S(S[6][4]));
	s s65(.S1(S[3][5]), .S0(S[2][5]), .Sel(C[4][3]), .S(S[6][5]));
	s s66(.S1(S[5][6]), .S0(S[4][6]), .Sel(C[4][3]), .S(S[6][6]));
	cs cs67(.C1(C[5][7]), .S1(S[5][7]), .C0(C[4][7]), .S0(S[4][7]), .Sel(C[4][3]), .C(C[6][7]), .S(S[6][7]));
	
	assign Cout = C[6][7];
	// restoring
	assign Z[0] = Sel ? S[6][7] : A_inv[0];
	assign Z[1] = Sel ? S[6][6] : A_inv[1];
	assign Z[2] = Sel ? S[6][5] : A_inv[2];
	assign Z[3] = Sel ? S[6][4] : A_inv[3];
	assign Z[4] = Sel ? S[4][3] : A_inv[4];
	assign Z[5] = Sel ? S[4][2] : A_inv[5];
	assign Z[6] = Sel ? S[2][1] : A_inv[6];
	assign Z[7] = Sel ? S[0][0] : A_inv[7];
	
endmodule