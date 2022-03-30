/* QueryLibrary for {{lib['name']}}
 * This file is created by ql.py.
 */

{{'#'}}include <stdlib.h>

{{'#'}}include "extrfunc.h"

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
            *iv = {{lib['apiversion']}};
            *pv = NULL;
            break;

         case EXTRFUNC_LIBQUERY_LIBRARY :
            *iv = {{lib['libraryversion']}};
            *pv = "{{lib['vendor']}}";
            break;

         case EXTRFUNC_LIBQUERY_NFUNCTIONS :
            *iv = {{ funcs|length }};
            *pv = "{{lib['description']}}";
            break;

         case EXTRFUNC_LIBQUERY_NEEDLICENSE :
            *iv = {{lib['needlicense']}};
            *pv = NULL;
            break;

         default :
            return EXTRFUNC_QUERYRETURN_ERROR;
      }

      return EXTRFUNC_QUERYRETURN_OK;
   }

   switch( funcnr )
   {
      # for funcname, funcprop in funcs.items()
      case {{loop.index}}:  /* {{funcname}} */
         switch( (EXTRFUNC_FUNCQUERY)query )
         {
            case EXTRFUNC_FUNCQUERY_FUNCNAME :
               *iv = 0;
               *pv = "{{funcname}}";
               break;

            case EXTRFUNC_FUNCQUERY_FUNCDESCR :
               *iv = 0;
               *pv = "{{funcprop['description']}}";
               break;

            case EXTRFUNC_FUNCQUERY_NOTINEQU :
               *iv = {{funcprop['notinequation']}};
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_CONTINDERIV :
               *iv = {%if funcprop['derivative'] == 'continuous'%}1{%else%}0{%endif%};
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ZERORIPPLE :
               *iv = {{funcprop['zeroripple']}};
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMIN :
               *iv = {{funcprop['argmin']}};
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_ARGMAX :
               *iv = {{funcprop['argmax']}};
               *pv = NULL;
               break;

            case EXTRFUNC_FUNCQUERY_MAXDERIV :
               *iv = {{funcprop['maxderivative']}};
               *pv = NULL;
               break;

            # for arg in funcprop['args']
            # if loop.index <= funcprop['argmax']
            case EXTRFUNC_FUNCQUERY_ARG{{"%02d"|format(loop.index)}} :
               *iv = {{funcprop['endogenous'][arg]}};
               *pv = "{{arg}}";
               break;
            # endif
            # endfor

            default :
               return EXTRFUNC_QUERYRETURN_ERROR;
         }
         break;
      # endfor

      default:
         return EXTRFUNC_QUERYRETURN_ERROR;
   }

   return EXTRFUNC_QUERYRETURN_OK;
}
