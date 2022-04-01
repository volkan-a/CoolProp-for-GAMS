$onText
dylib extension is for macOS
Please change your library name according
to your operating system and arhitecture
$offText

$funcLibIn propssi libpropssi64.dylib

$onText
Two problems are tested:
1) We are finding the pressure of air at 300 K
for specific entropy 4123 kJ/kgK
2) Finding optimum pressure ratio of two stage isentropic
compressor with intercooling
100 kPa and 300 K at inlet and pressure ratio is 30.
Optimum pressure ratio is sqrt(Po/Pi) which is 5.477
for this case.
$offtext

FUNCTION
    PropsSI /propssi.PropsSI2/;
    
POSITIVE VARIABLES
    P pressure for the first test,
    P2, h2 first stage outlet pressure and enthalpy
    h3, s3 intercooler outlet enthalpy and entropy
    h4 second stage outlet enthalpy;

VARIABLES z, wtot;

EQUATIONS
    eq1 Calculate the pressure for given entropy
    eq2 Enthalpy of first stage outlet
    eq3 Enthalpy of intercooler outlet
    eq4 Entropy of intercooler outlet
    eq5 Enthalpy of second stage outlet
    eq6 Total compressor work;

PARAMETERS
   fluid air /2/
   P1 Inlet pressure /101325.0/
   T1 Inlet temperature /300.0/
   h1 Inlet enthalpy
   s1 Inlet entropy
   P4 Outlet pressure /3E6/;

P.lo = 10E3;
P2.lo = P1;
P2.up = P4;
z.lo = 0;
h1 = PropsSI(4, 0, P1, 1, T1, fluid);
s1 = PropsSI(5, 0, P1, 1, T1, fluid);

eq1.. z =E= -4123+PropsSI(5, 0, P, 1, 300, fluid);
eq2.. h2 =E= PropsSI(4, 0, P2, 5, s1, fluid);
eq3.. h3 =E= PropsSI(4, 0, P2, 1, T1, fluid);                                                                                                                                                                                                                                                           
eq4.. s3 =E= PropsSI(5, 0, P2, 1, T1, fluid);
eq5.. h4 =E= PropsSI(4, 0, P4, 5, s3, fluid);
eq6.. wtot =E= h2-h1 + h4-h3;

MODEL calculatepressure /eq1/;
MODEL twostageintercooler /ALL-eq1/;

SOLVE calculatepressure USING nlp MINIMIZING z;
DISPLAY P.l, z.l;

SOLVE twostageintercooler USING nlp MINIMIZING wtot;
DISPLAY P2.l, wtot.l;

