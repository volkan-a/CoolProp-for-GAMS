#include <iostream>
#include<string>
#include "CoolPropLib.h"
using namespace std;


double PropsSI2(string prop, string prop1, double value1, string prop2, double value2, string fluid) {
    return PropsSI(prop.c_str(),prop1.c_str(),value1,prop2.c_str(),value2,fluid.c_str());
}