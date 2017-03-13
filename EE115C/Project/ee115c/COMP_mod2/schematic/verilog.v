// Verilog netlist generated by RFSiP netlister
// Cadence Design Systems, Inc.

module COMP_mod2 (
Vdd,Out,B,GND,A );
input  Vdd;
output  Out;
input  B;
input  GND;
input  A;
wire Vdd;
wire A;
wire B;
wire GND;
wire net7;
wire Out;

NOT    
 I7  ( .Vdd( Vdd ), .In( B ), .GND( GND ), .Out( net7 ) );

AND    
 I8  ( .Vdd( Vdd ), .A( A ), .GND( GND ), .B( net7 ), .Out( Out ) );

endmodule

