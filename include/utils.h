
/**************************************************************************************
 * Arquivo     : utils.h                                                              *
 * Dependencias: types.h                                                              *
 * Autor       : Eyder Rios (erios@ic.uff.br)                                         *
 * Data        : 01/06/2002                                                           *
 * Descricao   : Implementa varias funcoes para manipulacao de strings, conversao,    *
 *               entrada/saida de dados, etc.                                         *
 **************************************************************************************/

#ifndef __utils_h__
#define __utils_h__

#include <stdio.h>
#include "types.h"
#include <limits.h>
#include <float.h>


// usado para representar o infinito
/*
class __oo
{
private:
  BYTE  signal;

public:
  __oo () { signal = 1; };

  inline operator signed   long  int ()  { return signal ? MAXLONG    : MINLONG;   };
  inline operator signed   short int ()  { return signal ? MAXSHORT   : MINSHORT;  };
  inline operator unsigned long  int ()  { return signal ? MAXULONG   : MINULONG;  };
  inline operator unsigned short int ()  { return signal ? MAXUSHORT  : MINUSHORT; };
  inline operator float              ()  { return signal ? MAXFLOAT   : MINFLOAT;  };
  inline operator double             ()  { return signal ? MAXDOUBLE  : MINDOUBLE; };

  inline __oo &operator- ()     { signal = 0; return *this; };
  inline __oo &operator+ ()     { signal = 1; return *this; };
};
*/

class __oo
{
private:
  BYTE signal,sig;

public:
  __oo () { signal = 0; };

  inline operator signed   long  int ()
          { sig = signal; signal = 0;
            return sig ? MINLONG : MAXLONG;     };
  inline operator signed   short int ()
          { sig = signal; signal = 0;
            return sig ? MINSHORT : MAXSHORT;   };
  inline operator unsigned long  int ()
          { sig = signal; signal = 0;
            return sig ? MINULONG : MAXULONG;   };
  inline operator unsigned short int ()
          { sig = signal; signal = 0;
            return sig ? MINUSHORT : MAXUSHORT; };
  inline operator float              ()
          { sig = signal; signal = 0;
            return sig ? FLT_MIN : FLT_MAX;   };
  inline operator double             ()
          { sig = signal; signal = 0;
            return sig ? DBL_MIN : DBL_MAX; };

  inline __oo &operator- ()     { signal = 1; return *this; };
  inline __oo &operator+ ()     { signal = 0; return *this; };
};

EXTERN   __oo   oo;     // constante infinito
#define  nil    +oo     // constante nulo

// constantes para caracteres especiais mais comunus

#define CHR_NUL         '\0'
#define CHR_NEWLINE     '\n'
#define CHR_RETURN      '\t'
#define CHR_BELL        '\a'
#define CHR_TAB         '\t'
#define CHR_BLANK       ' '


/*
 #################################################################################
 #                                                                               #
 #                          MANIPULACAO DE STRINGS                               #
 #                                                                               #
 #################################################################################
*/

#define str_put(s)      puts(s)

PCHAR     str_dup       (PCHAR str);
PCHAR     str_rep       (PCHAR str, CHAR ch, CARDINAL n);
CARDINAL  str_puttext   (PCHAR str, CARDINAL col_first, CARDINAL col_all, CARDINAL col_width);


/*
 #################################################################################
 #                                                                               #
 #                          MANIPULACAO DE CARACTERES                            #
 #                                                                               #
 #################################################################################
*/

#define put_char(c)     putchar(c)
#define put_return()    putchar(CHR_RETURN)

CHAR  put_nchar   (CHAR  ch, UINT n, UINT line_feed = 0);
CHAR  put_newline (UINT  n = 1);

int   show_vsequence (CARDINAL start, CARDINAL finish,
                      UINT gap = 0, PCHAR prompt = NULL);

/*
 #################################################################################
 #                                                                               #
 #                             FUNCOES MATEMATICAS                               #
 #                                                                               #
 #################################################################################
*/

int    ndigits     (LONG n);
int    percent     (LONG val, LONG ref);
ULONG  fatorial    (ULONG n);
ULONG  combination (ULONG n, ULONG p);

/*
 #################################################################################
 #                                                                               #
 #                             FUNCOES DE ARQUIVOS                               #
 #                                                                               #
 #################################################################################
*/

#ifdef __MSDOS__

#define PATH_SEPARATOR  '\\'
#define PATH_EXTENTION  '.'

#else

#define PATH_SEPARATOR  '/'
#define PATH_EXTENTION  '.'

#endif

PCHAR extract_filename   (PCHAR path, PCHAR buffer = NULL);
PCHAR extract_path       (PCHAR path, PCHAR buffer);
PCHAR exchange_extension (PCHAR filename, PCHAR extension, PCHAR buffer);

/*
 #################################################################################
 #                                                                               #
 #                             PESQUISA E ORDENACAO                              #
 #                                                                               #
 #################################################################################
*/

void unsort (void *base, size_t nelem, size_t width);

#endif
