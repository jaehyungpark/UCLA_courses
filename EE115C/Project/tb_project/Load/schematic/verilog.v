// Verilog netlist generated by RFSiP netlister
// Cadence Design Systems, Inc.

module Load (
VDD,GND,OUT );
input  VDD;
input  GND;
input  OUT;
wire VDD;
wire net8;
wire net7;
wire GND;
wire OUT;

INVX32    
 I1  ( .VDD( VDD ), .Y( net7 ), .A( net8 ), .VSS( GND ) );

INVX32    
 I0  ( .VDD( VDD ), .Y( net8 ), .A( OUT ), .VSS( GND ) );

endmodule
