/**
 *@file params.cpp
 * Implementa funcoes para manipulacao de parametros de linha de comando.
 *
 * Dependencias: params.h, utils.h, types.h
 *
 * CONSIDERACOES
 *
 * Considere a seguinte linha de comando:
 * 
 *   $ programa -x -o 10 -tx texto --tag-longa
 *
 *  tag        identificador passado na linha de comando que indica qual parametro do
 *             programa sera configurado. No ex, '-x' '-o', '-tx' e '--tag-longa' sao
 *             tags. As tags em sistemas UNIX normalmente iniciam com '-' ou '--'
 *
 *  argumento  um argumento e' o valor que configura a tag. No ex, '10' e 'texto' sao
 *             argumentos. As tags, '-x' e '--tag-longa' nao possuem argumentos
 * @author Eyder Rios (erios@ic.uff.br)
 * @date 27/02/2002
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "params.h"
#include "utils.h"


// mensagens de erro

#define PAR_INV         "parametro '%s' invalido"
#define PAR_ARG         "parametro '%s' requer argumento"
#define PAR_ARG_INT     "parametro '%s' requer argumento inteiro"
#define PAR_ARG_CHAR    "parametro '%s' requer argumento com um unico caractere"
#define PAR_RANGE_INT   "parametro '%s' com argumento %ld fora da faixa [%ld-%lu]"
#define PAR_RANGE_CHAR  "parametro '%s' com argumento %c fora da faixa [%c-%c]"
#define PAR_RANGE_STR   "parametro '%s' com argumento '%s'fora da faixa ['%s'-'%s']"

// strings de uso interno
#define PAR_HELP_MASK   "uso: %s   "
#define PAR_HELP_PARAMS "[parametros]"
#define PAR_HELP_VAL    "val"
#define PAR_HELP_CHR    "chr"
#define PAR_HELP_STR    "str"
#define PAR_HELP_EMPTY  ""

#define PAR_HELP_LEFT   6


/** estrutura de informacoes sobre os parametros e linha de comando (uso interno) */
typedef struct tagPARAMS {
          PPARAMINFO   p_info;
          UINT         p_count;
          PCHAR       *p_argv;
          UINT         p_argc;
          UINT         p_index;
        } PARAMS, PPARAMS;


/** armazena as informacoes sobre os parametro e linha de comando da ultima chamada
 de param_init () */
PRIVATE PARAMS  params;


/***************************************************************************************
 *                                                                                     *
 *                                FUNCOES DE USO INTERNO                               *
 *                                                                                     *
 ***************************************************************************************/


/**
 * Obtem as informacoes sobre o  parametro  especificado  por 'tag'.
 * @date 22/05/2002
 * @param tag	identificador da tag procurada
 * @return informacoes sobre o parametro 'tag'. NULL se a 'tag' nao estiver definida.
*/
PRIVATE PPARAMINFO param_info (PCHAR tag)
{
  UINT  i;

  // verifica se 'tag' nao e' NULL
  if (assigned (tag)) {
    // procura 'tag' na lista de configuracao de parametros
    for (i=0;i < params.p_count;i++) {
      if (strcmp (tag,params.p_info[i].p_tag) == 0)
        return &params.p_info[i];
    }
  }

  return NULL;
}


/**
 * Obtem o indice da ultima ocorrencia de 'tag' na linha de comando.
 * @date 22/05/2002
 * @param tag	identificador da tag procurada
 * @return o indice (0-based) de 'tag'. Caso tag nao esteja na linha de comando retorna -1
 */
PRIVATE INT param_tag_index (PCHAR tag)
{
  INT  i,idx;

  idx = -1;

  // procura 'tag' na linha de comando
  for (i=1;i < params.p_argc;i++) {
    if (strcmp (params.p_argv[i],tag) == 0)
      idx = i;
  }

  return idx;
}


/**
 * Obtem o tipo do argumento 'arg' presente na linha de comando. Esta funcao e' usada
 * para verificar se um argumento e' do tipo requerido por 'tag'.
 * @param tag	identificador da tag procurada
 * @param arg	argumento a ser obter o tipo
 * @return o tipo de argumento 'arg'.
 * @date 22/05/2002
 */
PRIVATE PARAMTYPE param_arg_type (PCHAR tag, PCHAR arg)
{
  PPARAMINFO info;      // informacoes sobre o tag 'tag'
  PARAMTYPE  type;      // tipo do argumento 'arg'
  UINT       i;         // auxiliar

  // verifica se o argumento 'arg' e' um numero. se nao for e' string
  type = pt_int;
  for (i=0;arg[i];i++) {
    if (!isdigit (arg[i]))
      type = pt_str;
  }

  // como uma string tambem pode ser um caractere, verifica se o tag 'tag'
  // requer uma string ou um caractere e definido para 'arg' o tipo adequado
  if (assigned (tag)) {
    info = param_info (tag);
    if (assigned (info)) {
      if ((info->p_type == pt_char) && (strlen (arg) == 1))
        type = pt_char;
    }
  }

  return type;
}


/***************************************************************************************
 * Funcao   : UINT param_tag_args (PCHAR tag)                                          *
 * Criacao  : 22/05/2002                                                               *
 * Alteracao: 22/05/2002                                                               *
 * Descricao: Obtem a quantidade de argumentos passados para 'tag' na linha de comando *
 * Parametros: tag   - identificador da tag procurada                                  *
 * Retorno  : Retorna o numero de argumentos passados para 'tag'                       *
 ***************************************************************************************/

PRIVATE UINT param_tag_args (PCHAR tag)
{
  PPARAMINFO  info;     // informacoes sobre 'tag'
  UINT        count;    // no. de argumentos de 'tag'
  INT         idx;      // indice de 'tag' na linha de comando

  count = 0;

  idx = param_tag_index (tag);
  if (idx >= 0) {
    info = param_info (tag);
    if (assigned (info)) {
      if ((info->p_type != pt_bool) && (++idx < params.p_argc)) {
        if (params.p_argv[idx][0] != '-') {
          if (param_arg_type (tag,params.p_argv[idx]) == info->p_type)
            count++;
        }
      }
    }
  }

  return count;
}

/***************************************************************************************
 * Funcao   : BOOL param_istag (PCHAR tag)                                             *
 * Criacao  : 22/05/2002                                                               *
 * Alteracao: 22/05/2002                                                               *
 * Descricao: Verifica se 'tag' e realmente uma tag definida pelo programa. Esta defi- *
 *            nicao e' feita pela funcao param_init()                                  *
 * Parametros: tag   - identificador da tag procurada                                  *
 * Retorno  : Retorna TRUE se 'tag' em uma tag, senao FALSE.                           *
 ***************************************************************************************/

PRIVATE BOOL param_istag (PCHAR tag)
{
  UINT  i;

  for (i=0;i < params.p_count;i++) {
    if (strcmp (tag,params.p_info[i].p_tag) == 0)
      return TRUE;
  }

  return FALSE;
}

/***************************************************************************************
 *                                                                                     *
 *                                FUNCOES DE USO GLOBAL                                *
 *                                                                                     *
 ***************************************************************************************/

/***************************************************************************************
 * Funcao    : void param_init (PPARAMINFO info, UINT count, PCHAR argv[], int argc)   *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Inicializa a definicao de tags do programa. Toda funcao  da  biblioteca *
 *             so podera' ser utilizada apos a chamada de param_init().                *
 * Parametros: info   - definicao das tags do programa                                 *
 *             count  - numero de tags especificadas em 'info'                         *
 *             argv   - linha de comando - o mesmo valor de argv da funcao main()      *
 *             argc   - numero de parametros na linha de comando - equivale a argc     *
 * Retorno   : Nenhum                                                                  *
 ***************************************************************************************/

PUBLIC void param_init (PPARAMINFO info, UINT count, PCHAR argv[], int argc)
{
  params.p_info  = info;
  params.p_count = count;
  params.p_argv  = argv;
  params.p_argc  = argc;
  params.p_index = 0;
}

/***************************************************************************************
 * Funcao    : BOOL param_check (PCHAR msg)                                            *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Verifica se a linha de comando especificada em param_init() e' valida.  *
 *             A funcao verifica se os tags e argumentos sao valido. No caso de erro a *
 *             funcao retorna FALSE e a 'msg' recebe uma mensagem de erro para o  pri- *
 *             meio erro encontrado na linha de comando                                *
 * Parametros: msg    - recebe uma mensagem de error (caso haja erro)                  *
 * Retorno   : Retorna TRUE se a linha de comando esta' OK, senao FALSE                *
 ***************************************************************************************/

PUBLIC BOOL param_check (PCHAR msg)
{
  PPARAMINFO  info;
  PARAMTYPE   type;
  LONG        val_int;
  CHAR        val_char;
  UINT        i;

  for (i=1;i < params.p_argc;i++) {
    if (params.p_argv[i][0] == '-') {
      if (!param_istag (params.p_argv[i])) {
        sprintf (msg,PAR_INV,params.p_argv[i]);
        return FALSE;
      }
      else {
        info = param_info (params.p_argv[i]);
        if (info->p_type != pt_bool) {
          if (i < params.p_argc - 1) {
            i++;
            if (params.p_argv[i][0] != '-') {
              type = param_arg_type (info->p_tag,params.p_argv[i]);

              if (type != info->p_type) {
                switch (info->p_type) {
                  case pt_char:
                    sprintf (msg,PAR_ARG_CHAR,params.p_argv[i - 1]);
                    break;

                  case pt_int:
                    sprintf (msg,PAR_ARG_INT,params.p_argv[i - 1]);
                    break;

                  default:
                    sprintf (msg,PAR_ARG,params.p_argv[i - 1]);
                }
                return FALSE;
              }
              else {
                switch (type) {
                  case pt_int:
                    val_int = atol (params.p_argv[i]);
                    if ((val_int < info->p_range.r_min.u_int) ||
                        (val_int > info->p_range.r_max.u_int)) {
                       sprintf (msg,PAR_RANGE_INT,
                                    params.p_argv[i - 1],
                                    val_int,
                                    info->p_range.r_min.u_int,
                                    info->p_range.r_max.u_int);
                       return FALSE;
                    }
                    break;

                  case pt_char:
                    val_char = params.p_argv[i][0];
                    if ((val_char < info->p_range.r_min.u_char) ||
                        (val_char > info->p_range.r_max.u_char)) {
                       sprintf (msg,PAR_RANGE_CHAR,
                                    params.p_argv[i - 1],
                                    params.p_argv[i][0],
                                    info->p_range.r_min.u_char,
                                    info->p_range.r_max.u_char);
                       return FALSE;
                    }
                    break;

                  case pt_bool:
                  case pt_str:
                    break;
                }
              }
            }
            else {
              sprintf (msg,PAR_ARG,params.p_argv[i - 1]);
              return FALSE;
            }
          }
          else {
            sprintf (msg,PAR_ARG,params.p_argv[i]);
            return FALSE;
          }
        }
      }
    }
  }

  return TRUE;
}

/***************************************************************************************
 * Funcao    : void param_help (PCHAR appname)                                         *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Mostra na tela uma texto explicando quais os parametros definidos para  *
 *             o programa de nome 'appname'.                                           *
 * Parametros: appname  - nome do programa (arquivo executavel)                        *
 *             par_text - texto descritivo de como usar os parametros.                 *
 * Retorno   : Retorna TRUE se a linha de comando esta' OK, senao FALSE                *
 ***************************************************************************************/

PUBLIC void param_help (PCHAR appname, PCHAR par_text)
{
  UINT   i,max,len;
  PCHAR  oper;
  char   buffer[4096];

  max = 0;
  for (i=0;i < params.p_count;i++) {
    len = strlen (params.p_info[i].p_tag);
    if (len > max)
      max = len;
  }

  if (isnull (par_text))
    par_text = PAR_HELP_PARAMS;

  len = printf (PAR_HELP_MASK,appname);
  str_puttext (par_text,0,len,0);
  put_newline ();

  for (i=0;i < params.p_count;i++) {
    switch (params.p_info[i].p_type) {
      case pt_int:
        oper = PAR_HELP_VAL;
        sprintf (buffer,"%s (default %ld)",params.p_info[i].p_help,
                                           (LONG) params.p_info[i].p_default.u_int);
        break;

      case pt_char:
        oper = PAR_HELP_CHR;
        sprintf (buffer,"%s (default '%c')",params.p_info[i].p_help,
                                            params.p_info[i].p_default.u_char);
        break;

      case pt_str:
        oper = PAR_HELP_STR;
        sprintf (buffer,"%s (default '%s')",params.p_info[i].p_help,
                                            params.p_info[i].p_default.u_str);
        break;

      default:
        oper = PAR_HELP_EMPTY;
        sprintf (buffer,params.p_info[i].p_help);
    }

    len = printf ("%*s%-*s %-6s",PAR_HELP_LEFT,PAR_HELP_EMPTY,
                                 max,params.p_info[i].p_tag,oper);
    str_puttext (buffer,0,len,0);

    put_newline ();
 }
 put_newline ();
}

/***************************************************************************************
 * Funcao    : PCHAR param_appfile ()                                                  *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Retorna o caminho para o arquivo do programa executavel. Esta funcao e- *
 *             retorna o valor de argv[0].                                             *
 * Parametros: Nenhum                                                                  *
 * Retorno   : Nenhum                                                                  *
 ***************************************************************************************/

PUBLIC PCHAR param_appfile ()
{
  return params.p_argv[0];
}

/***************************************************************************************
 * Funcao    : BOOL param_asbool (PCHAR tag)                                           *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o valor de um argumento associado a 'tag' tag com tipo BOOL.      *
 *             Caso nao haja um valor associado na linha de comando, a funcao retorna  *
 *             o valor default.                                                        *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC BOOL param_asbool (PCHAR tag)
{
  PPARAMINFO  info;

  info = param_info (tag);
  if (assigned (info)) {
    if (param_tag_index (info->p_tag) != -1)
      return TRUE;
    else
      return info->p_default.u_bool;
  }

  return FALSE;
}

/***************************************************************************************
 * Funcao    : INT param_asint (PCHAR tag)                                             *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o valor de um argumento associado a 'tag' tag com tipo INT.       *
 *             Caso nao haja um valor associado na linha de comando, a funcao retorna  *
 *             o valor default.                                                        *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC INT param_asint (PCHAR tag)
{
  return (INT) param_aslong (tag);
}

/***************************************************************************************
 * Funcao    : LONG param_aslong (PCHAR tag)                                           *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o valor de um argumento associado a 'tag' tag com tipo LONG.      *
 *             Caso nao haja um valor associado na linha de comando, a funcao retorna  *
 *             o valor default.                                                        *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC LONG param_aslong (PCHAR tag)
{
  PPARAMINFO info;
  INT        i;

  info = param_info (tag);
  if (isnull (info))
    return 0;

  i = param_tag_index (info->p_tag);
  if (i != -1) {
    i++;
    if (!param_istag (params.p_argv[i]))
      return (INT) atol (params.p_argv[i]);
  }

  return info->p_default.u_int;
}

/***************************************************************************************
 * Funcao    : CHAR param_aschar (PCHAR tag)                                           *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o valor de um argumento associado a 'tag' tag com tipo CHAR.      *
 *             Caso nao haja um valor associado na linha de comando, a funcao retorna  *
 *             o valor default.                                                        *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC CHAR param_aschar (PCHAR tag)
{
  PPARAMINFO info;
  INT        i;

  info = param_info (tag);
  if (isnull (info))
    return 0;

  i = param_tag_index (info->p_tag);
  if (i != -1) {
    i++;
    if (!param_istag (params.p_argv[i]))
      return params.p_argv[i][0];
  }

  return info->p_default.u_char;
}

/***************************************************************************************
 * Funcao    : PCHAR param_asstr (PCHAR tag)                                           *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o valor de um argumento associado a 'tag' tag com tipo PCHAR.     *
 *             Caso nao haja um valor associado na linha de comando, a funcao retorna  *
 *             o valor default. NAO MODIFIQUE O CONTEUDO DA STRING RETORNADA!          *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC PCHAR param_asstr (PCHAR tag)
{
  PPARAMINFO info;
  INT        i;

  info = param_info (tag);
  if (isnull (info))
    return 0;

  i = param_tag_index (info->p_tag);
  if (i != -1) {
    i++;
    if (!param_istag (params.p_argv[i]))
      return params.p_argv[i];
  }

  return info->p_default.u_str;
}

/***************************************************************************************
 * Funcao    : PCHAR param_arg_first ()                                                *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o primeiro argumento da linha de comando que nao esta associado a *
 *             nenhuma tag. Caso nao existam argumentos nestas condicoes, a funcao re- *
 *             torna NULL.                                                             *
 * Parametros: tag   - tag associada ao argumento desejado                             *
 * Retorno   : Retorna o valor do argumento associado 'a tag 'tag'                     *
 ***************************************************************************************/

PUBLIC PCHAR param_arg_first ()
{
  PCHAR  arg;
  UINT   i;

  arg = NULL;
  for (i=1;i < params.p_argc;i++) {
    if (param_istag (params.p_argv[i]))
      i += param_tag_args (params.p_argv[i]);
    else {
      params.p_index = i + 1;
      arg = params.p_argv[i];
      break;
    }
  }

  return arg;
}

/***************************************************************************************
 * Funcao    : PCHAR param_arg_next  ()                                                *
 * Criacao   : 22/05/2002                                                              *
 * Alteracao : 22/05/2002                                                              *
 * Descricao : Obtem o proximo argumento da linha de comando que nao esta associado  a *
 *             nenhuma tag. Caso nao existam argumentos nestas condicoes, a funcao re- *
 *             torna NULL. Esta funcao baseia-se no  primeiro  argumento  obtido  pela *
 *             funcao param_arg_first() e por chadadas anteriores de param_arg_next(). *
 *             Assim, so chame para_arg_next() apos ter chamado para_arg_first() pelo  *
 *             menos uma vez.                                                          *
 * Parametros: Nenhum                                                                  *
 * Retorno   : Retorna o proximo argumento da linha de comando.                        *
 ***************************************************************************************/

PUBLIC PCHAR param_arg_next  ()
{
  PCHAR  arg;
  UINT   i;

  arg = NULL;
  for (i=params.p_index;i < params.p_argc;i++) {
    if (param_istag (params.p_argv[i]))
      i += param_tag_args (params.p_argv[i]);
    else {
      params.p_index = i + 1;
      arg = params.p_argv[i];
      break;
    }
  }

  return arg;
}

