module dividerv3_r(
	input wire [7:0] A,
	input wire [7:0] B,
	output wire [7:0] Q,
	output wire [7:0] R
	);

	wire [7:0] B_pt;
	wire [7:0] Z [7:0];
	
	// inverter
	assign B_pt[7:0] = {B[7], ~B[6], B[5], ~B[4], B[3], ~B[2], B[1], ~B[0]};
	
	rowv3_r R0(.A({1'b1, 1'b0, 1'b1, 1'b0, 1'b1, 1'b0, 1'b1, A[7]}), .B(B_pt), .Cin(1'b1), .Sel(Q[0]), .Cout(Q[0]), .Z(Z[0]));
	rowv3_r R1(.A({Z[0][6:0], A[6]}), .B(B_pt), .Cin(1'b1), .Sel(Q[1]), .Cout(Q[1]), .Z(Z[1]));
	rowv3_r R2(.A({Z[1][6:0], A[5]}), .B(B_pt), .Cin(1'b1), .Sel(Q[2]), .Cout(Q[2]), .Z(Z[2]));
	rowv3_r R3(.A({Z[2][6:0], A[4]}), .B(B_pt), .Cin(1'b1), .Sel(Q[3]), .Cout(Q[3]), .Z(Z[3]));
	rowv3_r R4(.A({Z[3][6:0], A[3]}), .B(B_pt), .Cin(1'b1), .Sel(Q[4]), .Cout(Q[4]), .Z(Z[4]));
	rowv3_r R5(.A({Z[4][6:0], A[2]}), .B(B_pt), .Cin(1'b1), .Sel(Q[5]), .Cout(Q[5]), .Z(Z[5]));
	rowv3_r R6(.A({Z[5][6:0], A[1]}), .B(B_pt), .Cin(1'b1), .Sel(Q[6]), .Cout(Q[6]), .Z(Z[6]));
	rowv3_r R7(.A({Z[6][6:0], A[0]}), .B(B_pt), .Cin(1'b1), .Sel(Q[7]), .Cout(Q[7]), .Z(Z[7]));
	
	assign R = {Z[7][7], ~Z[7][6], Z[7][5], ~Z[7][4], Z[7][3], ~Z[7][2], Z[7][1], ~Z[7][0]};

endmodule
