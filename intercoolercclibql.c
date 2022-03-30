/* QueryLibrary for intercoolercclib
 * This file is created by ql.py.
 */

#include <stdlib.h>

#include "extrfunc.h"

/** Callback function to query function library functionality */
EXTRFUNC_API int EXTRFUNC_CALLCONV querylibrary(
   int                   funcnr,          /**< function number, or <= 0 for library query */
   int                   query,           /**< query number (either EXTRFUNC_LIBQUERY or EXTRFUNC_FUNCQUERY) */
   int*                  iv,              /**< buffer to store integer result */
   const char**          pv               /**< buffer to store string result (as C string!) */
   )
{
   *iv = -1;
   *pv = "Bad parameter(s)";

   if( funcnr <= 0 )
   {
      switch( (EXTRFUNC_LIBQUERY)query )
      {
         case EXTRFUNC_LIBQUERY_API :
            *iv = 2;
            *pv = NULL;
            break;

         case EXTRFUNC_LIBQUERY_LIBRARY :
            *iv = 1;
            *pv = "Volkan Akkaya - volkan.akkaya@mu.edu.tr ©2022";
            break;

         case EXTRFUNC_LIBQUERY_NFUNCTIONS :
            *iv = 5;
            *pv = "Test cases for the extrinsic CoolProp library functions";
            break;

         case EXTRFUNC_LIBQUERY_NEEDLICENSE :
            *iv = 0;
            *pv = NULL;
            break;

         default :
            return EXTRFUNC_QUERYRETURN_ERROR;
      }

      return EXTRFUNC_QUERYRETURN_OK;
   }

   switch( funcnr )
   {
      case 1:  /* Tcrit */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "Tcrit";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "Return the critical temperature of water";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = 1;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 0;
               *pv = NULL;
               break;

            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      case 2:  /* Pcrit */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "Pcrit";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "Return the critical temperature of water";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = 1;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 0;
               *pv = NULL;
               break;

            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      case 3:  /* EnthalpyPT */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "EnthalpyPT";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "EnthalpyPT: Returns specific enthalpy of a fluid at a given pressure and temperature";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = 1;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARG01 :
               *iv = 1;
               *pv = "P";
               break;
            case EXTRFUNC_FUNCQUERY_ARG02 :
               *iv = 1;
               *pv = "T";
               break;
            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      case 4:  /* EnthalpyPs */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "EnthalpyPs";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "EnthalpyPs: Returns specific enthalpy of a fluid at a given pressure and specific entropy";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = 1;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARG01 :
               *iv = 1;
               *pv = "P";
               break;
            case EXTRFUNC_FUNCQUERY_ARG02 :
               *iv = 1;
               *pv = "s";
               break;
            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      case 5:  /* EntropyPT */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "EntropyPT";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "EntropyPT: Returns specific entropy of a fluid at a given pressure and temperature";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = 1;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 2;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARG01 :
               *iv = 1;
               *pv = "P";
               break;
            case EXTRFUNC_FUNCQUERY_ARG02 :
               *iv = 1;
               *pv = "T";
               break;
            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      default:
         return EXTRFUNC_QUERYRETURN_ERROR;
   }

   return EXTRFUNC_QUERYRETURN_OK;
}
