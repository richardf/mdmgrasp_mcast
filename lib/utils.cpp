
/**************************************************************************************
 * Arquivo     : utils.cpp                                                            *
 * Dependencias: utils.h, types.h                                                     *
 * Autor       : Eyder Rios (erios@ic.uff.br)                                         *
 * Data        : 01/06/2002                                                           *
 * Descricao   : Implementa varias funcoes para manipulacao de strings, conversao,    *
 *               entrada/saida de dados, etc.                                         *
 **************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "random.h"
#include "utils.h"
#include <limits.h>
#include <float.h>



/*
  #######################################################################################
  #                                                                                     #
  #                             VARIAVEIS E CONSTANTES GLOBAIS                          #
  #                                                                                     #
  #######################################################################################
*/

PUBLIC __oo   oo;     // constante infinito


/*
  #######################################################################################
  #                                                                                     #
  #                                 MANIPULACAO DE STRINGS                              #
  #                                                                                     #
  #######################################################################################
*/

/***************************************************************************************
 * Funcao   : PCHAR str_dup (PCHAR str)                                                *
 * Criacao  : 25/06/2002                                                               *
 * Alteracao: 25/06/2002                                                               *
 * Descricao: Duplica 'str' em uma area de memoria alocada dinamicamente.              *
 * Parametros: str   - string a ser duplicada                                          *
 * Retorno  : Retorna um ponteiro para a area de memoria alocada.                      *
 ***************************************************************************************/

PCHAR str_dup (PCHAR str)
{
  PCHAR  buffer;

  buffer = new char[strlen (str) + 1];
  if (assigned (buffer))
    strcpy (buffer,str);

  return buffer;
}

/***************************************************************************************
 * Funcao   : PCHAR str_rep (PCHAR str, CHAR ch, CARDINAL n)                           *
 * Criacao  : 01/06/2002                                                               *
 * Alteracao: 01/06/2002                                                               *
 * Descricao: Gera em 'str' uma string composta por 'n' caracteres 'ch'.               *
 * Parametros: str   - string que recebera' o resultado gerado                         *
 *             ch    - caractere a ser repetido                                        *
 *             n     - no. de repeticoes do caractere 'ch'                             *
 * Retorno  : Retorna um ponteiro para 'str'.                                          *
 ***************************************************************************************/

PUBLIC PCHAR str_rep (PCHAR str, CHAR ch, CARDINAL n)
{
  CARDINAL i;

  for (i=0;i < n;i++)
    str[i] = ch;
  str[i] = CHR_NUL;

  return str;
}

/***************************************************************************************
 * Funcao   : CARDINAL  str_puttext (PCHAR str, CARDINAL col_first, CARDINAL col_all,  *
 *                                   CARDINAL col_width)                               *
 * Criacao  : 01/06/2002                                                               *
 * Alteracao: 01/06/2002                                                               *
 * Descricao: Mostra o texto em 'str' na tela com a  1a.  linha  iniciando  na  coluna *
 *            'col_first' e as demais linhas na coluna 'col_all'.O texto em cada linha *
 *            tem, no maximo, 'col_width' caracteres. Se 'col_width' for  0,  nao  ha' *
 *            restricoes quanto 'a largura do texto. Uma nova linha e' impressa quando *
 *            um '\n' e' encontrado ou o numero de caracteres na linha e'  'col_width' *
 *            (se 'col_width' > 0).                                                    *
 * Parametros: str       - string a ser mostrada na tela                               *
 *             col_first - coluna onde inicia a primeira linha do texto                *
 *             col_all   - coluna onde inicia as demais linhas do texto                *
 *             col_width - largura maxima do texto em cada linha (se = 0, e' ignorado) *
 * Retorno  : Retorna o numero de linhas impressas.                                    *
 ***************************************************************************************/

PUBLIC CARDINAL  str_puttext (PCHAR str, CARDINAL col_first, CARDINAL col_all, CARDINAL col_width)
{
  CARDINAL  i,len,lins;

  if (col_width == 0)
    col_width = MAXCARDINAL;

  put_nchar (CHR_BLANK,col_first);

  len  = 0;
  lins = 1;
  for (i=0;str[i];i++) {
    if ((len == col_width) && (str[i + 1])) {
      put_newline ();
      put_nchar (CHR_BLANK,col_all);
      len = 0;
    }

    if (str[i] != CHR_NEWLINE) {
      put_char (str[i]);
      len++;
    }
    else {
      len = col_width;
      lins++;
    }
  }

  return lins;
}

/***************************************************************************************
 * Funcao   : CHAR  put_nchar (CHAR  ch, UINT n, UINT line_feed)                       *
 * Criacao  : 01/06/2002                                                               *
 * Alteracao: 01/06/2002                                                               *
 * Descricao: Imprime na tela um sequencia de 'n' caracteres 'ch'.                     *
 * Parametros: ch        - caracetere a ser repetido nao tela                          *
 *             n         - no. de repeticoes do caractere 'ch'                         *
 *             line_feed - no. de linhas apos a impressao                               *
 * Retorno  : Retorna o caracetere impresso.                                           *
 ***************************************************************************************/

PUBLIC CHAR  put_nchar (CHAR  ch, UINT n, UINT line_feed)
{
  while (n--)
    put_char (ch);

  while (line_feed--)
    put_newline ();

  return ch;
}

/***************************************************************************************
 * Funcao    : CHAR  put_newline (UINT n)                                              *
 * Criacao   : 01/06/2002                                                              *
 * Alteracao : 01/06/2002                                                              *
 * Descricao : Imprime uma ou mais linhas em branco na tela                            *
 * Parametros: n         - no. de linhas em branco                                     *
 * Retorno   : Retorna o caracetere impresso.                                          *
 ***************************************************************************************/

PUBLIC  CHAR  put_newline (UINT n)
{
  while (n--) {
#ifdef __MSDOS__
    putchar ('\r');
#endif
    putchar ('\n');
  }

  return '\n';
}

/*
  #######################################################################################
  #                                                                                     #
  #                                 MANIPULACAO DE ARQUIVOS                             #
  #                                                                                     #
  #######################################################################################
*/

/*
  #######################################################################################
  #                                                                                     #
  #                              ENTRADA E SAIDA DE DADOS                               #
  #                                                                                     #
  #######################################################################################
*/

/***************************************************************************************
 * Funcao    : int show_vsequence (CARDINAL start, CARDINAL finish, UINT gap,          *
 *                                 PCHAR prompt)                                       *
 * Criacao   : 28/05/2003                                                              *
 * Alteracao : 28/05/2003                                                              *
 * Descricao : Imprime uma sequencia de numeros inteiros onde cada numero e exibido na *
 *             tela verticalmente. Por exemplo, a sequencia de  5 a 15:                *
 *                   1                                                                  *
 *             56789012345                                                             *
 * Parametros: start  - numero inicial                                                 *
 *             finish - ponteiro para area de armazenamento                            *
 *             space  - espaco entre os numeros                                        *
 *             prompt - texto que aparece 'a esqueda em todas as linhas                *
 * Retorno  : Retorna a quantidade de numeros impressos.                               *
 ***************************************************************************************/

int show_vsequence (CARDINAL start, CARDINAL finish, UINT gap, PCHAR prompt)
{
  CARDINAL  n,           // cada numero impresso
            div10;       // potencia de 10
  char      buffer[16];  // auxiliar
  int       i,           // ordem do digito a ser impresso
            step,        // se incrementa ou decrementa
            digs,        // quantidade de digitos do maior numero
            dig;         // um digito de um numero

  // verifica se a sequencia e' crescrente ou decrescente
  if (finish < start) {
    n      = finish;
    finish = start;
    start  = n;

    step   = -1;
    digs = sprintf (buffer,"%lu",(ULONG) start);
  }
  else {
    step = 1;
    digs = sprintf (buffer,"%lu",(ULONG) finish);
  }

  div10 = 1;
  for (i=0;i < digs - 1;i++)
    div10 *= 10;

  for (i=digs - 1;i >= 0;i--) {
    if (assigned (prompt))
      printf ("%s",prompt);

    // imprime a linha do i-esimo digito mais significativo
    n = start - step;
    do {
      n += step;

      dig = (n / div10) % 10;

      if ((i == 0) || (n == finish) || (((n % 10) == 0) && (n != 0)))
        putchar (dig + '0');
      else
        putchar (' ');

      put_nchar (' ',gap);

    } while (n != finish);
    put_newline ();

    div10 = div10 / 10;
  }

  return (finish - start + 1);
}

/*
 #################################################################################
 #                                                                               #
 #                             FUNCOES MATEMATICAS                               #
 #                                                                               #
 #################################################################################
*/

PUBLIC int ndigits (LONG  n)
{
  char buffer[32];

  return sprintf (buffer,"%ld",n);
}

PUBLIC int percent (LONG val, LONG ref)
{
  int result;

  if (ref != 0) 
    result = (int) (val * 100 / ref);
  else
    result = 0;

  return result;
}

PUBLIC ULONG  fatorial (ULONG n)
{
  ULONG fat;

  fat = 1;
  while (n > 1)
    fat *= n--;

  return fat;
}

PUBLIC ULONG  combination (ULONG n, ULONG p)
{
  return fatorial (n) / (fatorial (p) * fatorial (n - p));
}

/*
 #################################################################################
 #                                                                               #
 #                             FUNCOES DE ARQUIVOS                               #
 #                                                                               #
 #################################################################################
*/

PCHAR extract_filename (PCHAR path, PCHAR buffer)
{
  int  i;

  for (i = strlen (path) - 1;i >= 0;i--) {
    if (path[i] == PATH_SEPARATOR) {
      if (assigned (buffer))
        strcpy (buffer,&path[i + 1]);
      return &path[i + 1];
    }
  }

  return path;
}

PCHAR extract_path (PCHAR path, PCHAR buffer)
{
  int  i;

  for (i = strlen (path) - 1;i >= 0;i--) {
    if (path[i] == PATH_SEPARATOR) {
      strncpy (buffer,path,i);
      return path;
    }
  }

  return NULL;
}

PCHAR exchange_extension (PCHAR filename, PCHAR extension, PCHAR buffer)
{
  int  i;

  strcpy (buffer,filename);
  for (i=strlen (buffer) - 1;i >= 0;i--) {
    if (buffer[i] == '.')
      break;
  }

  if (i > 0)
    strcpy (&buffer[i + 1],extension);
  else
    sprintf (buffer,"%s.%s",filename,extension);

  return buffer;
}

void unsort (void *base, size_t nelem, size_t width)
{
  size_t  i,j;
  char   *vbase;
  char   *telem;

  if (nelem <= 0)
    return;

  vbase = (char *) base;
  telem = new char[width];

  if (isnull (telem))
    return;

  for (i=0;i < nelem;i++) {
    j = xrandom (nelem);
    if (i != j) {
      memcpy (telem,&vbase[i * width],width);
      memcpy (&vbase[i * width],&vbase[j * width],width);
      memcpy (&vbase[j * width],telem,width);
    }
  }

  delete telem;
}

