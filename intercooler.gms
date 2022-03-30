$Title Test extrinsic functions in tricclib (TRILIB01,SEQ=521)

$ontext
This test makes sure that the extrinsic cosine and sine (both using rad and
grad) implemented in a C library work in the same way as the intrinsic versions.
Additionally the functions are used in a simple model.

Contributor: L. Westermann
$offtext


$ifThen set nocomp
*  Use precompiled library provided by testlib
$  batinclude precomp.inc tricclib
$else
*  Compile library from source code
$  batinclude compilec.inc tri
$endIf

function myCos        / myLib.Cosine /
         mySin        / myLib.Sine   /
         myPi         / myLib.Pi     /;

set g  / 1*360 /
    h  / CosInt      Cosine Intrinsic
         CosExtRad   Cosine Extrinsic (Radian)
         CosExtDeg   Cosine Extrinsic (Degree)
         SinInt      Sine Intrinsic
         SinExtRad   Sine Extrinsic (Radian)
         SinExtDeg   Sine Extrinsic (Degree)   /
    hh / F           Function Value
         G           Gradient Value
         H           Hessian Value
         GN          Gradient numeric
         HN          Hessian numeric   /;

parameter Deg(g)       Degree Value
          Rad(g)       Radian Value
          Test(g,h,hh);

Deg(g) = ord(g);
Rad(g) = ord(g)*pi/180;

option FDDelta=1e-3;
Test(g,'CosInt'   ,'F') = cos  (Rad(g));
Test(g,'CosExtRad','F') = mycos(Rad(g));
Test(g,'CosExtDeg','F') = mycos(Deg(g),1);
Test(g,'SinInt'   ,'F') = sin  (Rad(g));
Test(g,'SinExtRad','F') = mysin(Rad(g));
Test(g,'SinExtDeg','F') = mysin(Deg(g),1);

Test(g,'CosInt'   ,'G') = cos.grad  (1: Rad(g));
Test(g,'CosExtRad','G') = mycos.grad(1: Rad(g));
Test(g,'CosExtDeg','G') = mycos.grad(1: Deg(g),1)*180/pi;
Test(g,'SinInt'   ,'G') = sin.grad  (1: Rad(g));
Test(g,'SinExtRad','G') = mysin.grad(1: Rad(g));
Test(g,'SinExtDeg','G') = mysin.grad(1: Deg(g),1)*180/pi;

Test(g,'CosInt'   ,'H') = cos.hess  (1:1: Rad(g));
Test(g,'CosExtRad','H') = mycos.hess(1:1: Rad(g));
Test(g,'CosExtDeg','H') = mycos.hess(1:1: Deg(g),1)*180/pi*180/pi;
Test(g,'SinInt'   ,'H') = sin.hess  (1:1: Rad(g));
Test(g,'SinExtRad','H') = mysin.hess(1:1: Rad(g));
Test(g,'SinExtDeg','H') = mysin.hess(1:1: Deg(g),1)*180/pi*180/pi;

Test(g,'CosInt'   ,'GN') = cos.gradn  (1: Rad(g));
Test(g,'CosExtRad','GN') = mycos.gradn(1: Rad(g));
Test(g,'CosExtDeg','GN') = mycos.gradn(1: Deg(g),1)*180/pi;
Test(g,'SinInt'   ,'GN') = sin.gradn  (1: Rad(g));
Test(g,'SinExtRad','GN') = mysin.gradn(1: Rad(g));
Test(g,'SinExtDeg','GN') = mysin.gradn(1: Deg(g),1)*180/pi;

Test(g,'CosInt'   ,'HN') = cos.hessn  (1:1: Rad(g));
Test(g,'CosExtRad','HN') = mycos.hessn(1:1: Rad(g));
Test(g,'CosExtDeg','HN') = mycos.hessn(1:1: Deg(g),1)*180/pi*180/pi;
Test(g,'SinInt'   ,'HN') = sin.hessn  (1:1: Rad(g));
Test(g,'SinExtRad','HN') = mysin.hessn(1:1: Rad(g));
Test(g,'SinExtDeg','HN') = mysin.hessn(1:1: Deg(g),1)*180/pi*180/pi;

scalar
    error01 'mypi <> pi';
set error02 'cos/sin <> mycos/mysin (rad)'
    error03 'cos/sin <> mycos/mysin (grad)';

error01 = abs(pi <> mypi) > 1e-12;

error02(g,'cos',hh) = abs(Test(g,'CosInt',hh) - Test(g,'CosExtRad',hh)) > 1e-5;
error02(g,'sin',hh) = abs(Test(g,'SinInt',hh) - Test(g,'SinExtRad',hh)) > 1e-5;
error03(g,'cos',hh) = abs(Test(g,'CosInt',hh) - Test(g,'CosExtDeg',hh)) > 1e-5;
error03(g,'sin',hh) = abs(Test(g,'SinInt',hh) - Test(g,'SinExtDeg',hh)) > 1e-5;

abort$(error01+card(error02)+card(error03))
       error01, error02, error03;

********************************************************************************

Scalar trimode /0/;

variable x;
equation e;

e..   sqr(mysin(x,trimode)) + sqr(mycos(x,trimode)) =e= 1;

model m /e/;

x.lo = 0; x.l=3*pi
solve m min x using nlp;

abort$(abs(x.l-0)>1e-12) 'x<>0';

* Now do the same using degree instead of radian
trimode = 1;
x.lo = 0; x.l=540;
solve m min x using nlp;

abort$(abs(x.l-0)>1e-12) 'x<>0';
