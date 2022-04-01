/** GAMS Extrinsic Functions Example
 *
 * C Code Implementation to access CoolProp's PropsSI function.
 * Author: Dr. Volkan Ramazan Akkaya
 * Email: volkan.akkaya@mu.edu.tr
 * Compilation:
 *
 *   - GNU Compiler (macOS, Linux, Windows):
 *     gcc -fPIC -shared -olibpropssi[32|64].[dll|so|dylib] 
 *         propssicclib.c propssicclibql.c libCoolProp.dylib 
 *         -lm -arch [x86_64|i386]
 *
 *   - MS Visual Studio Compiler (Windows):
 *     cl.exe -LD -Fepropssilib[64].dll propssicclib.c propssicclibql.c CoolProp.dll  
 *            -link -def:tricclib.def
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/* include GAMS extrinsic functions API definition */
#include "extrfunc.h"
#include "CoolPropLib.h"

#define CMPVER     1

// Since GAMS only accepts floats and integer as function
// argument we need to select the correct index number for
// the fluid.
// This array can be extended to include more fluids.
// Please check the CoolProp documentation for the available fluids.
// http://www.coolprop.org/fluid_properties/PurePseudoPure.html
char* FLUID2[20] = {"Water", "R134a", "Air", "\0"};

// Similarily, we need to select the correct index number
// for the property name.
// This array can be extended to include more properties.
// please check the CoolProp documentation for the list of supported
// properties.
// http://www.coolprop.org/coolprop/HighLevelAPI.html#table-of-string-inputs-to-propssi-function
char* PROPERTY[20] ={"P", "T", "D", "U", "H", "S", "Q", "\0"};

/** function library data
 *
 * The struct EXTRFUNC_Data has been predefined in extrfunc.h.
 * An instantiation of this struct is used to store the libraries own data.
 * In this example, we have to store the function mode that the library
 * is currently operating in.
 *
 * The type EXTRFUNC_DATA has been typedef'ed to struct EXTRFUNC_Data.
 */
struct EXTRFUNC_Data{
};


/* forward declarations
 *
 * The EXTRFUNC_DECL_FUNCCALL macro has been defined in extrfunc.h and
 * simplifies the definition of callbacks that implement extrinsic
 * functions itself.
 */
EXTRFUNC_DECL_FUNCCALL(PropsSI2);

/* implementations */

/** Callback function to create function library data.
 *
 * This function is called by the GAMS execution system after the library
 * has been loaded. Its purpose is to allocate and initialize the
 * library-specific data structure (struct EXTRFUNC_Data).
 */
EXTRFUNC_API void EXTRFUNC_CALLCONV xcreate(
   EXTRFUNC_DATA**       data             /**< buffer to store pointer to function library data structure */
   )
{
   *data = malloc(sizeof(EXTRFUNC_DATA));
   assert(*data != NULL);
}

/** Callback function to free function library data.
 *
 * This function is called by the GAMS execution system before the library
 * is unloaded. Its purpose is to free the library-specific data.
 */
EXTRFUNC_API void EXTRFUNC_CALLCONV xfree(
   EXTRFUNC_DATA**       data             /**< pointer to pointer to function library data structure */
   )
{
   if( data != NULL )
   {
      free(*data);
      *data = NULL;
   }
}

/** Callback function to initialize function library.
 *
 * This function is called by the GAMS execution system after the library
 * has been loaded and library-specific data has been initialized.
 *
 * Its purpose is to check whether the API version of the library
 * is compatible with the GAMS execution system.
 * Additionally, library-specific initializations can be executed in this
 * call.
 *
 * In difference to xcreate, this function can return a message
 * and error code to the GAMS execution system in case of an error.
 *
 * @return 0 if successful, <> 0 in case of error.
 */
EXTRFUNC_API int EXTRFUNC_CALLCONV libinit(
   EXTRFUNC_DATA*        data,            /**< function library data structure */
   int                   version,         /**< version number */
   char*                 msg              /**< buffer of length 255 to store error message (as Delphi string!) */
   )
{
   if( version < CMPVER )
   {
      sprintf(msg+1, "Client is too old for this Library.");
      msg[0] = strlen(msg+1);
      return 1;
   }
   return 0;
}

/** Extrinsic Function to calculate the specific enthalpy of fluid 
 * for given temperature and pressure
 */
EXTRFUNC_DECL_FUNCCALL(PropsSI2)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(data != NULL);
   assert(x != NULL);
   assert(funcvalue != NULL);
   assert(derivrequest <= 2);
   assert(derivrequest <= 1 || hessian  != NULL);
   assert(derivrequest <= 0 || gradient != NULL);
   assert(errorcallback != NULL);

   if( nargs != 6 )
   {
      sprintf(msg+1, "EnthalpyPT: one argument expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }
   char* Prop = PROPERTY[(int)x[0]];
   char* Prop1 = PROPERTY[(int)x[1]];
   double Val1 = x[2];
   char* Prop2 = PROPERTY[(int)x[3]];
   double Val2 = x[4];
   char* Fluid = FLUID2[(int)x[5]];
   *funcvalue = PropsSI(Prop, Prop1, Val1, Prop2, Val2, Fluid);
   // calculate the first derivative with respect to pressure and temperature 
   char* str[80]; //string template for derivatives
   if(derivrequest>0) {
      sprintf(str, "d(%s)/d(%s)|%s", Prop, Prop1, Prop2);
      gradient[0] = PropsSI(str, Prop1, Val1, Prop2, Val2, Fluid);
      sprintf(str, "d(%s)/d(%s)|%s", Prop, Prop2, Prop1);
      gradient[1] = PropsSI("d(H)/d(T)|P", Prop1, Val1, Prop2, Val2, Fluid);
      // calculate the hessian matrix with respect to pressure and temperature
      if(derivrequest>1) {
         sprintf(str, "d(d(%s)/d(%s)|%s)/d(%s)|%s", Prop, Prop1, Prop2, Prop1, Prop2);
         hessian[0] = PropsSI(str, Prop1, Val1, Prop2, Val2, Fluid);
         sprintf(str, "d(d(%s)/d(%s)|%s)/d(%s)|%s", Prop, Prop1, Prop2, Prop2, Prop1);
         hessian[1] = PropsSI(str, Prop1, Val1, Prop2, Val2, Fluid);
         sprintf(str, "d(d(%s)/d(%s)|%s)/d(%s)|%s", Prop, Prop2, Prop1, Prop1, Prop2);
         hessian[2] = PropsSI(str, Prop1, Val1, Prop2, Val2, Fluid);
         sprintf(str, "d(d(%s)/d(%s)|%s)/d(%s)|%s", Prop, Prop2, Prop1, Prop2, Prop1);
         hessian[3] = PropsSI(str, Prop1, Val1, Prop2, Val2, Fluid);
      }
   }
   return EXTRFUNC_RETURN_OK;
}
