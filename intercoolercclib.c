/** GAMS Extrinsic Functions Example
 *
 * C Code Implementation to access trigonometric functions as GAMS extrinsic functions.
 *
 * Compilation:
 *
 *   - GNU Compiler (Linux):
 *     gcc -fPIC -shared -olibtricclib[64].so tricclib.c tricclibql.c -lm
 *
 *   - MS Visual Studio Compiler:
 *     cl.exe -LD -Fetricclib[64].dll tricclib.c tricclibql.c -link -def:tricclib.def
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
#define FLUID      "Air"

/** function library data
 *
 * The struct EXTRFUNC_Data has been predefined in extrfunc.h.
 * An instantiation of this struct is used to store the libraries own data.
 * In this example, we have to store the function mode that the library
 * is currently operating in.
 *
 * The type EXTRFUNC_DATA has been typedef'ed to struct EXTRFUNC_Data.
 */
struct EXTRFUNC_Data
{
};


/* forward declarations
 *
 * The EXTRFUNC_DECL_FUNCCALL macro has been defined in extrfunc.h and
 * simplifies the definition of callbacks that implement extrinsic
 * functions itself.
 */
EXTRFUNC_DECL_FUNCCALL(EnthalpyPT);
EXTRFUNC_DECL_FUNCCALL(EnthalpyPs);
EXTRFUNC_DECL_FUNCCALL(EntropyPT);
EXTRFUNC_DECL_FUNCCALL(Pcrit);
EXTRFUNC_DECL_FUNCCALL(Tcrit);

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
EXTRFUNC_DECL_FUNCCALL(EnthalpyPT)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(data != NULL);
   assert(x != NULL);
   assert(funcvalue != NULL);
   assert(derivrequest <= 2);
   assert(derivrequest <= 1 || hessian  != NULL);
   assert(derivrequest <= 0 || gradient != NULL);
   assert(errorcallback != NULL);

   if( nargs != 2 )
   {
      sprintf(msg+1, "EnthalpyPT: one argument expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }
   // calculate the specific enthalpy of fluid
   *funcvalue = PropsSI("H", "P", x[0], "T", x[1], FLUID);
   // calculate the first derivative with respect to pressure and temperature 
   if(derivrequest>0) {
      gradient[0] = PropsSI("d(H)/d(P)|T", "P", x[0], "T", x[1], FLUID);
      gradient[1] = PropsSI("d(H)/d(T)|P", "P", x[0], "T", x[1], FLUID);
      // calculate the hessian matrix with respect to pressure and temperature
      if(derivrequest>1) {
         hessian[0] = PropsSI("d(d(H)/d(P)|T)/d(P)|T", "P", x[0], "T", x[1], FLUID);
         hessian[1] = PropsSI("d(d(H)/d(P)|T)/d(T)|P", "P", x[0], "T", x[1], FLUID);
         hessian[2] = PropsSI("d(d(H)/d(T)|P)/d(P)|T", "P", x[0], "T", x[1], FLUID);
         hessian[3] = PropsSI("d(d(H)/d(T)|P)/d(T)|P", "P", x[0], "T", x[1], FLUID);
      }
   }
   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function to calculate the specific enthalpy of fluid 
 * for given temperature and specific entropy
 */
EXTRFUNC_DECL_FUNCCALL(EnthalpyPs)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(data != NULL);
   assert(x != NULL);
   assert(funcvalue != NULL);
   assert(derivrequest <= 2);
   assert(derivrequest <= 1 || hessian  != NULL);
   assert(derivrequest <= 0 || gradient != NULL);
   assert(errorcallback != NULL);

   /* check for correct number of arguments */
   if( nargs < 1 || nargs > 2 )
   {
      sprintf(msg+1, "Cosine: two arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   // calculate the specific enthalpy of fluid with respect to pressure and specific entropy
   *funcvalue = PropsSI("H", "P", x[0], "S", x[1], FLUID);
   if( derivrequest > 0 )
   {
      /* evaluate gradient */
      gradient[0] = PropsSI("d(H)/d(P)|S", "P", x[0], "S", x[1], FLUID);
      gradient[1] = PropsSI("d(H)/d(S)|P", "P", x[0], "S", x[1], FLUID);
      /* evaluate Hessian */
      if( derivrequest > 1 )
      {
         hessian[0] = PropsSI("d(d(H)/d(P)|S)/d(P)|S", "P", x[0], "S", x[1],FLUID);
         hessian[1] = PropsSI("d(d(H)/d(P)|S)/d(S)|P", "P", x[0], "S", x[1],FLUID);
         hessian[2] = PropsSI("d(d(H)/d(S)|P)/d(P)|S", "P", x[0], "S", x[1],FLUID);
         hessian[3] = PropsSI("d(d(H)/d(S)|P)/d(S)|P", "P", x[0], "S", x[1],FLUID);
      }
   }

   return EXTRFUNC_RETURN_OK;
}

EXTRFUNC_DECL_FUNCCALL(EntropyPT)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(data != NULL);
   assert(x != NULL);
   assert(funcvalue != NULL);
   assert(derivrequest <= 2);
   assert(derivrequest <= 1 || hessian  != NULL);
   assert(derivrequest <= 0 || gradient != NULL);
   assert(errorcallback != NULL);

   /* check for correct number of arguments */
   if( nargs < 1 || nargs > 2 )
   {
      sprintf(msg+1, "Cosine: two arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   // calculate the specific entropy of fluid with respect to pressure and temperature
   *funcvalue = PropsSI("S", "P", x[0], "T", x[1], FLUID);
   if( derivrequest > 0 )
   {
      /* evaluate gradient */
      gradient[0] = PropsSI("d(S)/d(P)|T", "P", x[0], "T", x[1], FLUID);
      gradient[1] = PropsSI("d(S)/d(T)|P", "P", x[0], "T", x[1], FLUID);
      /* evaluate Hessian */
      if( derivrequest > 1 )
      {
         hessian[0] = PropsSI("d(d(S)/d(P)|T)/d(P)|T", "P", x[0], "T", x[1],FLUID);
         hessian[1] = PropsSI("d(d(H)/d(P)|T)/d(T)|P", "P", x[0], "T", x[1],FLUID);
         hessian[2] = PropsSI("d(d(H)/d(T)|P)/d(P)|T", "P", x[0], "T", x[1],FLUID);
         hessian[3] = PropsSI("d(d(H)/d(T)|P)/d(T)|P", "P", x[0], "T", x[1],FLUID);
      }
   }

   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function implementing Pcrit. */
EXTRFUNC_DECL_FUNCCALL(Pcrit)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(funcvalue != NULL);
   assert(errorcallback != NULL);

   if( nargs != 0 )
   {
      sprintf(msg+1, "Tcrit: zero arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   *funcvalue = PropsSI("Pcrit", "T", 300.0, "P", 101325.0, FLUID);

   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function implementing Tcrit. */
EXTRFUNC_DECL_FUNCCALL(Tcrit)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(funcvalue != NULL);
   assert(errorcallback != NULL);

   if( nargs != 0 )
   {
      sprintf(msg+1, "Tcrit: zero arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   *funcvalue = PropsSI("Tcrit", "T", 300.0, "P", 101325.0, FLUID);

   return EXTRFUNC_RETURN_OK;
}
