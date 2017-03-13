// Verilog netlist generated by RFSiP netlister
// Cadence Design Systems, Inc.

module ABSol (
A0,A1,A2,A3,A4,A5,VDD,GND,S0,S1,S2,S3,S4,S5 );
input  A0;
input  A1;
input  A2;
input  A3;
input  A4;
input  A5;
input  VDD;
input  GND;
output  S0;
output  S1;
output  S2;
output  S3;
output  S4;
output  S5;
wire net035;
wire VDD;
wire net031;
wire net032;
wire A0;
wire S4;
wire S3;
wire GND;
wire A3;
wire S5;
wire A1;
wire A4;
wire net030;
wire net18;
wire S2;
wire net17;
wire net034;
wire net19;
wire net033;
wire A2;
wire net21;
wire A5;
wire S1;
wire S0;
wire net20;
wire net22;

NOT    
 I5  ( .Vdd( VDD ), .In( A0 ), .GND( GND ), .Out( net22 ) );

NOT    
 I4  ( .Vdd( VDD ), .In( A5 ), .GND( GND ), .Out( net20 ) );

NOT    
 I3  ( .Vdd( VDD ), .In( A4 ), .GND( GND ), .Out( net19 ) );

NOT    
 I2  ( .Vdd( VDD ), .In( A3 ), .GND( GND ), .Out( net17 ) );

NOT    
 I1  ( .Vdd( VDD ), .In( A2 ), .GND( GND ), .Out( net18 ) );

NOT    
 I0  ( .Vdd( VDD ), .In( A1 ), .GND( GND ), .Out( net21 ) );

HAdder    
 I15  ( .S5( net030 ), .VDD( VDD ), .A1( net21 ), .S0( net035 ), .A0( net22 ), .S4( net031 ), .S3( net032 ), .A2( net18 ), .A5( net20 ), .A4( net19 ), .S1( net034 ), .GND( GND ), .A3( net17 ), .S2( net033 ) );

MUXblock    
 I9  ( .Vdd( VDD ), .C( A5 ), .A( A1 ), .GND( GND ), .B( net034 ), .Out( S1 ) );

MUXblock    
 I14  ( .Vdd( VDD ), .C( A5 ), .A( A4 ), .GND( GND ), .B( net031 ), .Out( S4 ) );

MUXblock    
 I13  ( .Vdd( VDD ), .C( A5 ), .A( A5 ), .GND( GND ), .B( net030 ), .Out( S5 ) );

MUXblock    
 I12  ( .Vdd( VDD ), .C( A5 ), .A( A2 ), .GND( GND ), .B( net033 ), .Out( S2 ) );

MUXblock    
 I11  ( .Vdd( VDD ), .C( A5 ), .A( A3 ), .GND( GND ), .B( net032 ), .Out( S3 ) );

MUXblock    
 I10  ( .Vdd( VDD ), .C( A5 ), .A( A0 ), .GND( GND ), .B( net035 ), .Out( S0 ) );

endmodule

