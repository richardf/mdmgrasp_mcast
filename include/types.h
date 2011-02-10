
/**********************************************************************************
 * Arquivo  : types.h                                                             *
 * Criacao  : 27/04/2002                                                          *
 * Autor    : Eyder Rios                                                          *
 * Descricao: Definicao de tipos basicos para manipulacao de dados escalares, lo- *
 *            gicos e ponto-flutuante para utilizacao independete de plataforma.  *
 **********************************************************************************/

#ifndef __types_h__
#define __types_h__

#include <stdlib.h>
#include <limits.h> 
#include <float.h>


#define PUBLIC                              // prefixo p/ objeto public
#define PRIVATE         static              // prefixo p/ objeto private
#define EXTERN          extern              // prefixo p/ objeto externo


// operacoes com ponteiros
#define isnull(p)       ((p) == NULL)       // indica se um ponteiro e' nulo
#define assigned(p)     ((p) != NULL)       // indica se um ponteiro e' valido


// constantes para tipos escalares
#define UNASSIGNED      MAX_CARDINAL
#define INFINITY_POS    MAX_CARDINAL
#define INFINITY_NEG    0

// declaracao de tipos basicos escalares

#ifdef __MSDOS__
typedef enum {FALSE, TRUE}  BOOL;           // logico
#define bool     BOOL
#else
#define BOOL     bool
#define FALSE    false
#define TRUE     true
#endif

typedef          char       CHAR,  *PCHAR;  // caractere
typedef unsigned char       BYTE,  *PBYTE;  // inteiro sem sinal de 8 bits (byte)
typedef signed   short int  INT,   *PINT;   // inteiro com sinal de 16 bits
typedef unsigned short int  UINT,  *PUINT;  // inteiro sem sinal de 16 bits
typedef signed   long  int  LONG,  *PLONG;  // inteiro com sinal de 32 bits
typedef unsigned long  int  ULONG, *PULONG; // inteiro sem sinal de 32 bits

typedef double REAL, *PREAL;                // numero de ponto flutuante

typedef void  *POINTER;                     // ponteiro generico

#define MINSHORT        SHRT_MIN
#define MAXSHORT        SHRT_MAX
#define MAXLONG		LONG_MAX
#define MAXUSHORT       0xffff
#define MINUSHORT       0
#define MINLONG         LONG_MIN
#define MAXULONG        ULONG_MAX
#define MINULONG        0

#ifdef __MSDOS__

#define MAXCARDINAL     UINT_MAX       // numero cardinal maximo (16 bits)
#define MINCARDINAL     0              // numero cardinal maximo (16 bits)

typedef unsigned short int  CARDINAL, *PCARDINAL;

#else

#define MAXCARDINAL     ULONG_MAX       // numero cardinal maximo (32 bits)
#define MINCARDINAL     0                // numero cardinal maximo (32 bits)

typedef unsigned long  int  CARDINAL, *PCARDINAL;

#endif

class VARIANT
{
private:
  union {
    char              *vpchar;
    char               vchar;
    signed   int       vsint;
    unsigned int       vuint;
    signed short int   vsshort;
    unsigned short int vushort;
    signed long int    vslong;
    unsigned long int  vulong;
    float              vfloat;
    double             vdouble;
  } value;

public:
  operator char *             () { return value.vpchar;  };
  operator char               () { return value.vchar;   };
  operator unsigned int       () { return value.vuint;   };
  operator signed   int       () { return value.vsint;   };
  operator unsigned short int () { return value.vushort; };
  operator signed   short int () { return value.vsshort; };
  operator unsigned long int  () { return value.vulong;  };
  operator signed   long int  () { return value.vslong;  };
  operator float              () { return value.vfloat;  };
  operator double             () { return value.vdouble; };
};

#endif
