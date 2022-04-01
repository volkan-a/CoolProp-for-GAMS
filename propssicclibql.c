/* QueryLibrary for propssicclib
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
            *iv = 1;
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
      case 1:  /* PropsSI2 */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "PropsSI2";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "CoolProp PropsSI implementation for GAMS";
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
               *iv = 6;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = 6;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = 0;
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARG01 :
               *iv = 0;
               *pv = "Prop";
               break;
            case EXTRFUNC_FUNCQUERY_ARG02 :
               *iv = 0;
               *pv = "Prop1";
               break;
            case EXTRFUNC_FUNCQUERY_ARG03 :
               *iv = 1;
               *pv = "Value1";
               break;
            case EXTRFUNC_FUNCQUERY_ARG04 :
               *iv = 0;
               *pv = "Prop2";
               break;
            case EXTRFUNC_FUNCQUERY_ARG05 :
               *iv = 1;
               *pv = "Value2";
               break;
            case EXTRFUNC_FUNCQUERY_ARG06 :
               *iv = 0;
               *pv = "Fluid";
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
