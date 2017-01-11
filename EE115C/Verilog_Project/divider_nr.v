`timescale 1ns / 1ps

module divider_nr(
	input wire [7:0] A,
	input wire [7:0] B,
	output wire [7:0] Q,
	output wire [7:0] R
	);
	
	wire [8:0] Cout [8:0];
	wire [8:0] Z [8:0];
	
	assign R = Z[8][7:0];
	assign Q = {Cout[0][8], Cout[1][8], Cout[2][8], Cout[3][8], Cout[4][8], Cout[5][8], Cout[6][8], Cout[7][8]};

	// row 0
	block_nr B00(.A(A[7]), .B(B[0]), .Cin(1'b1), .Sel(1'b1), .Cout(Cout[0][0]), .Z(Z[0][0]));
	block_nr B01(.A(1'b0), .B(B[1]), .Cin(Cout[0][0]), .Sel(1'b1), .Cout(Cout[0][1]), .Z(Z[0][1]));
	block_nr B02(.A(1'b0), .B(B[2]), .Cin(Cout[0][1]), .Sel(1'b1), .Cout(Cout[0][2]), .Z(Z[0][2]));
	block_nr B03(.A(1'b0), .B(B[3]), .Cin(Cout[0][2]), .Sel(1'b1), .Cout(Cout[0][3]), .Z(Z[0][3]));
	block_nr B04(.A(1'b0), .B(B[4]), .Cin(Cout[0][3]), .Sel(1'b1), .Cout(Cout[0][4]), .Z(Z[0][4]));
	block_nr B05(.A(1'b0), .B(B[5]), .Cin(Cout[0][4]), .Sel(1'b1), .Cout(Cout[0][5]), .Z(Z[0][5]));
	block_nr B06(.A(1'b0), .B(B[6]), .Cin(Cout[0][5]), .Sel(1'b1), .Cout(Cout[0][6]), .Z(Z[0][6]));
	block_nr B07(.A(1'b0), .B(B[7]), .Cin(Cout[0][6]), .Sel(1'b1), .Cout(Cout[0][7]), .Z(Z[0][7]));
	block_nr B08(.A(1'b0), .B(1'b0), .Cin(Cout[0][7]), .Sel(1'b1), .Cout(Cout[0][8]), .Z(Z[0][8]));
	
	// row 1
	block_nr B10(.A(A[6]), .B(B[0]), .Cin(Cout[0][8]), .Sel(Cout[0][8]), .Cout(Cout[1][0]), .Z(Z[1][0]));
	block_nr B11(.A(Z[0][0]), .B(B[1]), .Cin(Cout[1][0]), .Sel(Cout[0][8]), .Cout(Cout[1][1]), .Z(Z[1][1]));
	block_nr B12(.A(Z[0][1]), .B(B[2]), .Cin(Cout[1][1]), .Sel(Cout[0][8]), .Cout(Cout[1][2]), .Z(Z[1][2]));
	block_nr B13(.A(Z[0][2]), .B(B[3]), .Cin(Cout[1][2]), .Sel(Cout[0][8]), .Cout(Cout[1][3]), .Z(Z[1][3]));
	block_nr B14(.A(Z[0][3]), .B(B[4]), .Cin(Cout[1][3]), .Sel(Cout[0][8]), .Cout(Cout[1][4]), .Z(Z[1][4]));
	block_nr B15(.A(Z[0][4]), .B(B[5]), .Cin(Cout[1][4]), .Sel(Cout[0][8]), .Cout(Cout[1][5]), .Z(Z[1][5]));
	block_nr B16(.A(Z[0][5]), .B(B[6]), .Cin(Cout[1][5]), .Sel(Cout[0][8]), .Cout(Cout[1][6]), .Z(Z[1][6]));
	block_nr B17(.A(Z[0][6]), .B(B[7]), .Cin(Cout[1][6]), .Sel(Cout[0][8]), .Cout(Cout[1][7]), .Z(Z[1][7]));
	block_nr B18(.A(Z[0][7]), .B(1'b0), .Cin(Cout[1][7]), .Sel(Cout[0][8]), .Cout(Cout[1][8]), .Z(Z[1][8]));
	
	// row 2
	block_nr B20(.A(A[5]), .B(B[0]), .Cin(Cout[1][8]), .Sel(Cout[1][8]), .Cout(Cout[2][0]), .Z(Z[2][0]));
	block_nr B21(.A(Z[1][0]), .B(B[1]), .Cin(Cout[2][0]), .Sel(Cout[1][8]), .Cout(Cout[2][1]), .Z(Z[2][1]));
	block_nr B22(.A(Z[1][1]), .B(B[2]), .Cin(Cout[2][1]), .Sel(Cout[1][8]), .Cout(Cout[2][2]), .Z(Z[2][2]));
	block_nr B23(.A(Z[1][2]), .B(B[3]), .Cin(Cout[2][2]), .Sel(Cout[1][8]), .Cout(Cout[2][3]), .Z(Z[2][3]));
	block_nr B24(.A(Z[1][3]), .B(B[4]), .Cin(Cout[2][3]), .Sel(Cout[1][8]), .Cout(Cout[2][4]), .Z(Z[2][4]));
	block_nr B25(.A(Z[1][4]), .B(B[5]), .Cin(Cout[2][4]), .Sel(Cout[1][8]), .Cout(Cout[2][5]), .Z(Z[2][5]));
	block_nr B26(.A(Z[1][5]), .B(B[6]), .Cin(Cout[2][5]), .Sel(Cout[1][8]), .Cout(Cout[2][6]), .Z(Z[2][6]));
	block_nr B27(.A(Z[1][6]), .B(B[7]), .Cin(Cout[2][6]), .Sel(Cout[1][8]), .Cout(Cout[2][7]), .Z(Z[2][7]));
	block_nr B28(.A(Z[1][7]), .B(1'b0), .Cin(Cout[2][7]), .Sel(Cout[1][8]), .Cout(Cout[2][8]), .Z(Z[2][8]));
	
	// row 3
	block_nr B30(.A(A[4]), .B(B[0]), .Cin(Cout[2][8]), .Sel(Cout[2][8]), .Cout(Cout[3][0]), .Z(Z[3][0]));
	block_nr B31(.A(Z[2][0]), .B(B[1]), .Cin(Cout[3][0]), .Sel(Cout[2][8]), .Cout(Cout[3][1]), .Z(Z[3][1]));
	block_nr B32(.A(Z[2][1]), .B(B[2]), .Cin(Cout[3][1]), .Sel(Cout[2][8]), .Cout(Cout[3][2]), .Z(Z[3][2]));
	block_nr B33(.A(Z[2][2]), .B(B[3]), .Cin(Cout[3][2]), .Sel(Cout[2][8]), .Cout(Cout[3][3]), .Z(Z[3][3]));
	block_nr B34(.A(Z[2][3]), .B(B[4]), .Cin(Cout[3][3]), .Sel(Cout[2][8]), .Cout(Cout[3][4]), .Z(Z[3][4]));
	block_nr B35(.A(Z[2][4]), .B(B[5]), .Cin(Cout[3][4]), .Sel(Cout[2][8]), .Cout(Cout[3][5]), .Z(Z[3][5]));
	block_nr B36(.A(Z[2][5]), .B(B[6]), .Cin(Cout[3][5]), .Sel(Cout[2][8]), .Cout(Cout[3][6]), .Z(Z[3][6]));
	block_nr B37(.A(Z[2][6]), .B(B[7]), .Cin(Cout[3][6]), .Sel(Cout[2][8]), .Cout(Cout[3][7]), .Z(Z[3][7]));
	block_nr B38(.A(Z[2][7]), .B(1'b0), .Cin(Cout[3][7]), .Sel(Cout[2][8]), .Cout(Cout[3][8]), .Z(Z[3][8]));	

	// row 4
	block_nr B40(.A(A[3]), .B(B[0]), .Cin(Cout[3][8]), .Sel(Cout[3][8]), .Cout(Cout[4][0]), .Z(Z[4][0]));
	block_nr B41(.A(Z[3][0]), .B(B[1]), .Cin(Cout[4][0]), .Sel(Cout[3][8]), .Cout(Cout[4][1]), .Z(Z[4][1]));
	block_nr B42(.A(Z[3][1]), .B(B[2]), .Cin(Cout[4][1]), .Sel(Cout[3][8]), .Cout(Cout[4][2]), .Z(Z[4][2]));
	block_nr B43(.A(Z[3][2]), .B(B[3]), .Cin(Cout[4][2]), .Sel(Cout[3][8]), .Cout(Cout[4][3]), .Z(Z[4][3]));
	block_nr B44(.A(Z[3][3]), .B(B[4]), .Cin(Cout[4][3]), .Sel(Cout[3][8]), .Cout(Cout[4][4]), .Z(Z[4][4]));
	block_nr B45(.A(Z[3][4]), .B(B[5]), .Cin(Cout[4][4]), .Sel(Cout[3][8]), .Cout(Cout[4][5]), .Z(Z[4][5]));
	block_nr B46(.A(Z[3][5]), .B(B[6]), .Cin(Cout[4][5]), .Sel(Cout[3][8]), .Cout(Cout[4][6]), .Z(Z[4][6]));
	block_nr B47(.A(Z[3][6]), .B(B[7]), .Cin(Cout[4][6]), .Sel(Cout[3][8]), .Cout(Cout[4][7]), .Z(Z[4][7]));
	block_nr B48(.A(Z[3][7]), .B(1'b0), .Cin(Cout[4][7]), .Sel(Cout[3][8]), .Cout(Cout[4][8]), .Z(Z[4][8]));	

	// row 5
	block_nr B50(.A(A[2]), .B(B[0]), .Cin(Cout[4][8]), .Sel(Cout[4][8]), .Cout(Cout[5][0]), .Z(Z[5][0]));
	block_nr B51(.A(Z[4][0]), .B(B[1]), .Cin(Cout[5][0]), .Sel(Cout[4][8]), .Cout(Cout[5][1]), .Z(Z[5][1]));
	block_nr B52(.A(Z[4][1]), .B(B[2]), .Cin(Cout[5][1]), .Sel(Cout[4][8]), .Cout(Cout[5][2]), .Z(Z[5][2]));
	block_nr B53(.A(Z[4][2]), .B(B[3]), .Cin(Cout[5][2]), .Sel(Cout[4][8]), .Cout(Cout[5][3]), .Z(Z[5][3]));
	block_nr B54(.A(Z[4][3]), .B(B[4]), .Cin(Cout[5][3]), .Sel(Cout[4][8]), .Cout(Cout[5][4]), .Z(Z[5][4]));
	block_nr B55(.A(Z[4][4]), .B(B[5]), .Cin(Cout[5][4]), .Sel(Cout[4][8]), .Cout(Cout[5][5]), .Z(Z[5][5]));
	block_nr B56(.A(Z[4][5]), .B(B[6]), .Cin(Cout[5][5]), .Sel(Cout[4][8]), .Cout(Cout[5][6]), .Z(Z[5][6]));
	block_nr B57(.A(Z[4][6]), .B(B[7]), .Cin(Cout[5][6]), .Sel(Cout[4][8]), .Cout(Cout[5][7]), .Z(Z[5][7]));
	block_nr B58(.A(Z[4][7]), .B(1'b0), .Cin(Cout[5][7]), .Sel(Cout[4][8]), .Cout(Cout[5][8]), .Z(Z[5][8]));	

	// row 6
	block_nr B60(.A(A[1]), .B(B[0]), .Cin(Cout[5][8]), .Sel(Cout[5][8]), .Cout(Cout[6][0]), .Z(Z[6][0]));
	block_nr B61(.A(Z[5][0]), .B(B[1]), .Cin(Cout[6][0]), .Sel(Cout[5][8]), .Cout(Cout[6][1]), .Z(Z[6][1]));
	block_nr B62(.A(Z[5][1]), .B(B[2]), .Cin(Cout[6][1]), .Sel(Cout[5][8]), .Cout(Cout[6][2]), .Z(Z[6][2]));
	block_nr B63(.A(Z[5][2]), .B(B[3]), .Cin(Cout[6][2]), .Sel(Cout[5][8]), .Cout(Cout[6][3]), .Z(Z[6][3]));
	block_nr B64(.A(Z[5][3]), .B(B[4]), .Cin(Cout[6][3]), .Sel(Cout[5][8]), .Cout(Cout[6][4]), .Z(Z[6][4]));
	block_nr B65(.A(Z[5][4]), .B(B[5]), .Cin(Cout[6][4]), .Sel(Cout[5][8]), .Cout(Cout[6][5]), .Z(Z[6][5]));
	block_nr B66(.A(Z[5][5]), .B(B[6]), .Cin(Cout[6][5]), .Sel(Cout[5][8]), .Cout(Cout[6][6]), .Z(Z[6][6]));
	block_nr B67(.A(Z[5][6]), .B(B[7]), .Cin(Cout[6][6]), .Sel(Cout[5][8]), .Cout(Cout[6][7]), .Z(Z[6][7]));
	block_nr B68(.A(Z[5][7]), .B(1'b0), .Cin(Cout[6][7]), .Sel(Cout[5][8]), .Cout(Cout[6][8]), .Z(Z[6][8]));	

	// row 7
	block_nr B70(.A(A[0]), .B(B[0]), .Cin(Cout[6][8]), .Sel(Cout[6][8]), .Cout(Cout[7][0]), .Z(Z[7][0]));
	block_nr B71(.A(Z[6][0]), .B(B[1]), .Cin(Cout[7][0]), .Sel(Cout[6][8]), .Cout(Cout[7][1]), .Z(Z[7][1]));
	block_nr B72(.A(Z[6][1]), .B(B[2]), .Cin(Cout[7][1]), .Sel(Cout[6][8]), .Cout(Cout[7][2]), .Z(Z[7][2]));
	block_nr B73(.A(Z[6][2]), .B(B[3]), .Cin(Cout[7][2]), .Sel(Cout[6][8]), .Cout(Cout[7][3]), .Z(Z[7][3]));
	block_nr B74(.A(Z[6][3]), .B(B[4]), .Cin(Cout[7][3]), .Sel(Cout[6][8]), .Cout(Cout[7][4]), .Z(Z[7][4]));
	block_nr B75(.A(Z[6][4]), .B(B[5]), .Cin(Cout[7][4]), .Sel(Cout[6][8]), .Cout(Cout[7][5]), .Z(Z[7][5]));
	block_nr B76(.A(Z[6][5]), .B(B[6]), .Cin(Cout[7][5]), .Sel(Cout[6][8]), .Cout(Cout[7][6]), .Z(Z[7][6]));
	block_nr B77(.A(Z[6][6]), .B(B[7]), .Cin(Cout[7][6]), .Sel(Cout[6][8]), .Cout(Cout[7][7]), .Z(Z[7][7]));
	block_nr B78(.A(Z[6][7]), .B(1'b0), .Cin(Cout[7][7]), .Sel(Cout[6][8]), .Cout(Cout[7][8]), .Z(Z[7][8]));	

	// row 8
	block_r B80(.A(Z[7][0]), .B(B[0]), .Cin(1'b0), .Sel(Z[7][8]), .Cout(Cout[8][0]), .Z(Z[8][0]));
	block_r B81(.A(Z[7][1]), .B(B[1]), .Cin(Cout[8][0]), .Sel(Z[7][8]), .Cout(Cout[8][1]), .Z(Z[8][1]));
	block_r B82(.A(Z[7][2]), .B(B[2]), .Cin(Cout[8][1]), .Sel(Z[7][8]), .Cout(Cout[8][2]), .Z(Z[8][2]));
	block_r B83(.A(Z[7][3]), .B(B[3]), .Cin(Cout[8][2]), .Sel(Z[7][8]), .Cout(Cout[8][3]), .Z(Z[8][3]));
	block_r B84(.A(Z[7][4]), .B(B[4]), .Cin(Cout[8][3]), .Sel(Z[7][8]), .Cout(Cout[8][4]), .Z(Z[8][4]));
	block_r B85(.A(Z[7][5]), .B(B[5]), .Cin(Cout[8][4]), .Sel(Z[7][8]), .Cout(Cout[8][5]), .Z(Z[8][5]));
	block_r B86(.A(Z[7][6]), .B(B[6]), .Cin(Cout[8][5]), .Sel(Z[7][8]), .Cout(Cout[8][6]), .Z(Z[8][6]));
	block_r B87(.A(Z[7][7]), .B(B[7]), .Cin(Cout[8][6]), .Sel(Z[7][8]), .Cout(Cout[8][7]), .Z(Z[8][7]));
	
endmodule
