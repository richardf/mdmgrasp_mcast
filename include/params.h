
/**************************************************************************************
 * Arquivo     : params.cpp                                                           *
 * Dependencias: params.h, utils.h, types.h                                           *
 * Autor       : Eyder Rios (erios@ic.uff.br)                                         *
 * Data        : 27/02/2002                                                           *
 * Descricao   : Implementa funcoes para manipulacao de parametros de linha de coman- *
 *               do.                                                                  *
 **************************************************************************************/


#ifndef __params_h__
#define __params_h__

#include <float.h>
#include <limits.h>

#include "types.h"

/** utilizado para saber quantos tags contem o vetor 'info' */
#define tag_count(info)      (sizeof(info)/sizeof (PARAMINFO))

// utilizado para inicializar os valores default das tags 
#define pr_bool_any          { pv_bool (FALSE),    pv_bool (TRUE)    }
#define pr_char_any          { pv_char (32),       pv_char (126)     }
#define pr_char(min,max)     { pv_char (min),      pv_char (max)     }
#define pr_str_any           { pv_str  (0),        pv_str  (0)       }
#define pr_str(min,max)      { pv_str  (min),      pv_str  (max)     }
#define pr_int_any           { pv_int  (-MAXLONG), pv_int  (MAXLONG) }
#define pr_int(min,max)      { pv_int  (min),      pv_int  (max)     }

#define pv_bool(v)           { (PCHAR) (v) }
#define pv_int(v)            { (PCHAR) (v) }
#define pv_char(v)           { (PCHAR) (v) }
#define pv_str(v)            { (PCHAR) (v) }

/** tipos de argumentos de tag's */
typedef enum { pt_bool,       /**< logico */
               pt_int,        /**< inteiro (int ou long) */
               pt_char,       /**< caractere (char) */
               pt_str         /**< string */
              } PARAMTYPE;

// tipo de dado generico (uso interno)
// typedef PCHAR VARIANT;

/** especifica um valor de qualquer tipo (string, char, inteiro ou bool) */
typedef union tagPARAMVALUE {
          PCHAR  u_str;
          CHAR   u_char;
          LONG   u_int;
          BOOL   u_bool;
        } PARAMVALUE, *PPARAMVALUE;

/** especifica uma faixa de valor de qualquer tipo */
typedef struct tagPARAMRANGE {
          PARAMVALUE  r_min;      /**< valor inicial */
          PARAMVALUE  r_max;      /**< valor final */
        } PARAMRANGE, *PPARAMRANGE;

/** especifica as informacoes sobre um unico tag */
typedef struct tagPARAMINFO {
          PCHAR        p_tag;       /**<  identificador da tag (ex: -x, --del) */
          PCHAR        p_help;      /**<  texto explicativo sobre a tag */
          PARAMTYPE    p_type;      /**<  tipo do argumento associado 'a tag */
          PARAMVALUE   p_default;   /**<  valor default do argumento associado */
          PARAMRANGE   p_range;     /**<  faixa de valores valida */
        } PARAMINFO, *PPARAMINFO;


void   param_init    (PPARAMINFO info, UINT count, PCHAR argv[], int argc);
BOOL   param_check   (PCHAR msg);
void   param_help    (PCHAR appname, PCHAR par_text);

PCHAR  param_appfile ();
BOOL   param_asbool  (PCHAR tag);
CHAR   param_aschar  (PCHAR tag);
INT    param_asint   (PCHAR tag);
LONG   param_aslong  (PCHAR tag);
PCHAR  param_asstr   (PCHAR tag);

PCHAR  param_arg_first ();
PCHAR  param_arg_next  ();

#endif
