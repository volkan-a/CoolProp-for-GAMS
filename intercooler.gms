$Title Optimum pressure ratio of two stage compressor train with intercooling

$ontext
This test makes sure that the extrinsic CoolProp functions are working correctly.
Contributor: V.R. Akkaya 
$offtext

$funcLibIn intercooler libintercooler64.dylib

function
    EnthalpyPT /intercooler.EnthalpyPT/
    EntropyPT  /intercooler.EntropyPT/
    EnthalpyPs /intercooler.EnthalpyPs/;

scalar
  T1 /300/
  P1 /1e6/
  P4 /5e6/
  h1, s1;
  
variables
  P2, h2
  s3, h3
  h4
  wtot;
  
equations
  eq1, eq2, eq3, eq4, eqx, eqx2;
  
h1 = EnthalpyPT(P1, T1);
s1 = EntropyPT(P1, T1);
* wtest = EnthalpyPs(1.5e6, EntropyPT(1.5e6, s1))-EnthalpyPT(P1, T1) -
*        EnthalpyPs(P4, EntropyPT(1.5e6, T1))-EnthalpyPT(1.5E6, T1);

eq1.. h2 =e= EnthalpyPs(P2, s1);
eq2.. h3 =e= EnthalpyPT(P2, T1);
eq3.. s3 =e= EntropyPT(P2, T1);
eq4.. h4 =e= EnthalpyPs(P4, s3);
eqx.. wtot =e= h2-h1 + h4-h3;
eqx2.. wtot =e= EnthalpyPs(P2, EntropyPT(P1, T1))-EnthalpyPT(P1, T1) +
        EnthalpyPs(P4, EntropyPT(P2, T1))-EnthalpyPT(P2, T1);

P2.lo = P1;
P2.up = P4;

model m /all/;
model m2 /eqx2/;

options nlp=CONOPT;
solve m2 using nlp minimizing wtot;

* display P2.l, wtot.l, h1, h2.l, h3.l, h4.l, wtest
display P2.l, wtot.l;


