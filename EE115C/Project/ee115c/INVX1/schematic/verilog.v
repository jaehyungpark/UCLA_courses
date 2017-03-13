// Verilog netlist generated by RFSiP netlister
// Cadence Design Systems, Inc.

module INVX1 (
A,VDD,GND,Z );
input  A;
input  VDD;
input  GND;
input  Z;
wire VDD;
wire Z;
wire A;
wire GND;

pmos1v    
 PM0  ( .S( VDD ), .G( A ), .B( VDD ), .D( Z ) );

nmos1v    
 NM0  ( .S( GND ), .G( A ), .B( GND ), .D( Z ) );

endmodule

