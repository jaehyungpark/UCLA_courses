`timescale 1ns / 1ps

module tb(
	);
	reg clk;
	reg rst;

	wire [7:0] A;
	wire [7:0] B;
	wire [7:0] Q[5:0];
	wire [7:0] R[5:0];
	
	reg [15:0] index;
	
	// ===========================================================================
   // clock and reset signals
   // ===========================================================================
	
	initial begin
		rst <= 1;
		clk <= 0;
		#10 rst <= 0;
	end
	
	always begin
		#5 clk <= ~clk;
	end
	
   // ===========================================================================
   // all combinations of dividend and divisor
   // ===========================================================================
	
	always @(posedge clk) begin
		if (rst) begin
			index <= 0;
		end else if (index == 16'b1111111111111111) begin
			$finish;
		end else begin
			index <= index + 1;
		end
	end
	
	assign A = index[15:8];
	assign B = index[7:0];
	
   // ===========================================================================
   // compare outputs of all dividers
   // ===========================================================================
	
	always @(posedge clk) begin
		if (rst) begin
			;
		end else begin
			if (Q[0] != Q[1] || R[0] != R[1]) begin
				$display("D1 ERROR: %d/%d != %d R %d", A, B, Q[1], B[1]);
			end
			if (Q[0] != Q[2] || R[0] != R[2]) begin
				$display("D2 ERROR: %d/%d != %d R %d", A, B, Q[2], B[2]);
			end
			if (Q[0] != Q[3] || R[0] != R[3]) begin
				$display("D2 ERROR: %d/%d != %d R %d", A, B, Q[3], B[3]);
			end
		end
	end
	
   // ===========================================================================
   // modules of different dividers
   // ===========================================================================
	
	// ideal divider
	divider D0(
	.A(A),
	.B(B),
	.Q(Q[0]),
	.R(R[0])
	);
	
	// restoring divider
	divider_r D1(
	.A(A),
	.B(B),
	.Q(Q[1]),
	.R(R[1])
	);
	
	// nonrestoring divider
	divider_nr D2(
	.A(A),
	.B(B),
	.Q(Q[2]),
	.R(R[2])
	);
	
	// restoring divider v2, with mirror adder
	dividerv2_r D3(
	.A(A),
	.B(B),
	.Q(Q[3]),
	.R(R[3])
	);
	
	// restoring divider v3, with conditional sum
	dividerv3_r D4(
	.A(A),
	.B(B),
	.Q(Q[4]),
	.R(R[4])
	);
	
	// restoring divider v4, with delay (NOT IMPLEMENTED)
	// syntax for adding delay : assign #delay A = B;
	dividerv4_r D5(
	.A(A),
	.B(B),
	.Q(Q[5]),
	.R(R[5])
	);

endmodule
