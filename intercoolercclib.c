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
EXTRFUNC_DECL_FUNCCALL(EnthalpyPS);

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

/** Extrinsic Function to calculate the enthalpy of water 
 * for given temperature and pressure
 */
EXTRFUNC_DECL_FUNCCALL(EnthalpyPT)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(data != NULL);
   assert(x != NULL);
   assert(funcvalue != NULL);
   assert(errorcallback != NULL);

   if( nargs != 2 )
   {
      sprintf(msg+1, "EnthalpyPT: one argument expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }
   *funcvalue = PropsSI("H", "P", x[0], "T", x[1], "Water");
   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function implementing Cosine
 *
 * This function implements the callback for the Cosine function.
 * It checks whether the correct number of arguments has been passed,
 * the first argument is not too large to be evaluated, and the second
 * argument does not violate its domain.
 *
 * If first or second derivatives are requested, the callback also
 * computes these w.r.t. all endogenous arguments, i.e., the first argument.
 */
EXTRFUNC_DECL_FUNCCALL(Cosine)
{
   char msg[EXTRFUNC_STRSIZE];
   double locX;
   FUNCMODE funcmode;

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

   /* check 2nd argument */
   if( nargs == 2 && x[1] != (double)FUNCMODE_RAD && x[1] != (double)FUNCMODE_DEG )
   {
      sprintf(msg+1, "Cosine: mode has to be %d (rad) or %d (deg)", FUNCMODE_RAD, FUNCMODE_DEG);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   /* decide whether we use radians or degrees */
   funcmode = data->funcmode;
   if( nargs == 2 )
      funcmode = (FUNCMODE)x[1];

   /* convert degrees into radians, if necessary */
   if( funcmode == FUNCMODE_DEG )
      locX = x[0] * M_PI/180.0;
   else
      locX = x[0];

   /* check for sigloss */
   if( fabs(locX) > 1.0e12 )
   {
      sprintf(msg+1, "Cosine: |x| > 1e12");
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_SIGLOSS, msg, errorcbmem);
   }

   /* evaluate function value */
   *funcvalue = cos(locX);

   if( derivrequest > 0 )
   {
      /* evaluate gradient */
      if( funcmode == FUNCMODE_DEG )
         gradient[0] = -sin(locX) * M_PI/180.0;
      else
         gradient[0] = -sin(locX);

      /* evaluate Hessian */
      if( derivrequest > 1 )
      {
         if( funcmode == FUNCMODE_DEG )
            hessian[0] = -(*funcvalue) * M_PI/180.0 * M_PI/180.0;
         else
            hessian[0] = -(*funcvalue);
      }
   }

   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function implementing Sine.
 *
 * The implementation is equivalent to the one of Cosine.
 */
EXTRFUNC_DECL_FUNCCALL(Sine)
{
   char msg[EXTRFUNC_STRSIZE];
   double locX;
   FUNCMODE funcmode;

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
      sprintf(msg+1, "Sine: two arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   /* check 2nd argument */
   if( nargs == 2 && x[1] != (double)FUNCMODE_RAD && x[1] != (double)FUNCMODE_DEG )
   {
      sprintf(msg+1, "Sine: mode has to be %d (rad) or %d (deg)", FUNCMODE_RAD, FUNCMODE_DEG);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   /* decide whether we use radians or degrees */
   funcmode = data->funcmode;
   if( nargs == 2 )
      funcmode = (FUNCMODE)x[1];

   /* convert degrees into radians, if necessary */
   if( funcmode == FUNCMODE_DEG )
      locX = x[0] * M_PI/180.0;
   else
      locX = x[0];

   /* check for sigloss */
   if( fabs(locX) > 1.0e12 )
   {
      sprintf(msg+1, "Sine: |x| > 1e12");
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_SIGLOSS, msg, errorcbmem);
   }

   /* evaluate function value */
   *funcvalue = sin(locX);

/*
   This version of the library leaves the derivative calculation to the finite
   difference method inside GAMS
*/
   if( derivrequest > 0 )
   {
#if 0
      /* evaluate gradient */
      if( funcmode == FUNCMODE_DEG )
         gradient[0] = cos(locX) * M_PI/180.0;
      else
         gradient[0] = cos(locX);

      /* evaluate Hessian */
      if( derivrequest > 1 )
      {
         if( funcmode == FUNCMODE_DEG )
            hessian[0] = -(*funcvalue) * M_PI/180.0 * M_PI/180.0;
         else
            hessian[0] = -(*funcvalue);
      }
#else
      sprintf(msg+1, "No derivatives available for Sine");
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_SYSTEM, EXTRFUNC_EVALERROR_NONE, msg, errorcbmem);
#endif
   }
   return EXTRFUNC_RETURN_OK;
}

/** Extrinsic Function implementing Pi. */
EXTRFUNC_DECL_FUNCCALL(Pi)
{
   char msg[EXTRFUNC_STRSIZE];

   assert(funcvalue != NULL);
   assert(errorcallback != NULL);

   if( nargs != 0 )
   {
      sprintf(msg+1, "Pi: zero arguments expected. Called with %d", nargs);
      msg[0] = strlen(msg+1);
      return errorcallback(EXTRFUNC_RETURN_FUNCTION, EXTRFUNC_EVALERROR_DOMAIN, msg, errorcbmem);
   }

   *funcvalue = M_PI;

   return EXTRFUNC_RETURN_OK;
}
