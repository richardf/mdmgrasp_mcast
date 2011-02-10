// Este codigo esta comentado de forma a ser documentado pelo doxygen.
/**
 * @mainpage Documentacao do GRASPMCAST
 * @section intro_sec Introducao
 * lalalalariii
 *
 * @section comp_sec Compilacao
 * @subsection sub1 Passo 1: Compilando o graspmcast
 * Basta executar make no diretorio principal, que ira gerar o executavel graspmcast.
 * @subsection sub2 Passo 2: Compilando os modulos de mineracao
 * Digite make dentro do diretorio md.
 *
 * @section instancias_sec Estrutura do arquivo de instancias
 * O arquivo de instancias tem o seguinte formato:\n
 * - a 1a linha indica o numero de fontes;\n
 * - Depois temos a indicacao de quais sao as fontes e o seu numero de arestas\n
 * - Cada fonte eh seguida pela indicacao das arestas de origem, destino e o custo da ligacao\n
 *
 * Exemplo:\n
 * \n
 * 001) 50 \n
 * 002) 201 137 \n
 * 003) 206 6 27 \n
 * 004) 6 116 6 \n
 * 005) 116 12 1 \n
 * [...] \n
 * 140) 206 143 \n
 * 141) 201 1 68 \n
 * 142) 1 109 5 \n
 * 143) 109 159 3 \n
 * 144) 159 12 4 \n
 * [...] \n
 * \n
 * Na linha 1) temos o numero de fontes, 50. \n
 * A linha 2) indica o primeiro no usado como fonte (201) e o numero de arestas que ele possui (137). \n
 * Nas linhas 3), 4), 5), ... 139) estao indicados a origem, destino e custo das arestas. Assim, a linha 5) indica uma
 * aresta com origem em 116, destino 12 e custo 1. \n
 * Na linha 140) esta a definicao de uma outra fonte, seguido pela definicao de suas arestas nas linhas seguintes.
 */

#include "main.h"
#include "arvore.h"
#include "params.h"
#include "clock.h"
#include "grasprm.h"
#include "pool.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <sys/times.h>


/*
 ##################################################################################
 #                                                                                #
 #                     CONSTANTES, MACROS E VARIAVEIS GLOBAIS                     #
 #                                                                                #
 ##################################################################################
*/

// strings

#define APP_NAME              "graspmcast"
#define APP_VERSION           "$Revision: 1.5 $"
#define APP_START              2005
#define APP_PARAMS            "[parametros]"

// definicao dos parametros da linha de comando

#define PAR_TAG_INST     "-a"
#define PAR_DEF_INST     pv_str("")
#define PAR_STR_INST     "instancia - arvores multicast"

#define PAR_TAG_NUMSERV  "-m"
#define PAR_DEF_NUMSERV  pv_int(10)
#define PAR_STR_NUMSERV  "numero de servidores de replicacao"

#define PAR_TAG_ITER     "-i"
#define PAR_DEF_ITER      pv_int(500)
#define PAR_STR_ITER     "numero de iteracoes"

#define PAR_TAG_ALFA     "-alfa"
#define PAR_DEF_ALFA      pv_int(70)
#define PAR_STR_ALFA     "Parametro alfa do GRASP"

#define PAR_TAG_PCT      "-pct"
#define PAR_DEF_PCT      pv_int(5)
#define PAR_STR_PCT      "Porcentagem do total de iterações (opção -i)\n" \
                         "que o pool deve ficar sem alteracoes para ocorrer\n" \
						 "uma minera��o de dados"


#define PAR_TAG_TAMELITE "-t"
#define PAR_DEF_TAMELITE pv_int(10)
#define PAR_STR_TAMELITE "tamanho do conjunto elite"

#define PAR_TAG_MINSUP   "-sm"
#define PAR_DEF_MINSUP   pv_int(1)
#define PAR_STR_MINSUP   "suporte minimo"
			  			  
#define PAR_TAG_ESTRAT   "-e"
#define PAR_DEF_ESTRAT   pv_int(3)
#define PAR_STR_ESTRAT   "estratégia de mineração de dados\n" \
                         "  0 - HLP\n"          \
                         "  1 - HLS\n"          \
                         "  2 - HnLP\n"         \
			 "* 3 - HnMP"

#define PAR_TAG_NPADROES "-np"
#define PAR_DEF_NPADROES pv_int(1)
#define PAR_STR_NPADROES "quantidade de padrões a serem usados\n" \
                         "- deve ser informado SSE a opção -e = HnLP ou HnMP"
 
#define PAR_TAG_SEM      "-s"
#define PAR_DEF_SEM      pv_int(1)
#define PAR_STR_SEM      "semente randomica"


PARAMINFO par_info[] = {
//    ---- tag ----    ---- ajuda ----   - tipo - - val.default -   ----- faixa -----
    { PAR_TAG_INST,     PAR_STR_INST,     pt_str,  PAR_DEF_INST,     pr_str_any       },
    { PAR_TAG_ITER,     PAR_STR_ITER,     pt_int,  PAR_DEF_ITER,     pr_int(1,10000)  },
    { PAR_TAG_ALFA,     PAR_STR_ALFA,     pt_int,  PAR_DEF_ALFA,     pr_int(1,100)  },
    { PAR_TAG_NUMSERV,  PAR_STR_NUMSERV,  pt_int,  PAR_DEF_NUMSERV,  pr_int(1,1000)   },
    { PAR_TAG_PCT,      PAR_STR_PCT,      pt_int,  PAR_DEF_PCT,      pr_int(0,100)    },    
    { PAR_TAG_TAMELITE, PAR_STR_TAMELITE, pt_int,  PAR_DEF_TAMELITE, pr_int(0,1000)   },
    { PAR_TAG_MINSUP,   PAR_STR_MINSUP,   pt_int,  PAR_DEF_MINSUP,   pr_int(0,1000)   },    
    { PAR_TAG_ESTRAT,   PAR_STR_ESTRAT,   pt_int,  PAR_DEF_ESTRAT,   pr_int(0,3)      },
    { PAR_TAG_NPADROES, PAR_STR_NPADROES, pt_int,  PAR_DEF_NPADROES, pr_int(0,100)    },    
    { PAR_TAG_SEM,      PAR_STR_SEM,      pt_int,  PAR_DEF_SEM,      pr_int(0,10000)  },
};

/*
 ##################################################################################
 #                                                                                #
 #                             FUNCOES DA APLICACAO                               #
 #                                                                                #
 ##################################################################################
*/

/**
 * Mostra a mensagem de erro 'msg' na tela e aborta o programa caso o valor de halt
 * seja diferente de 0 retornando-o como codigo de erro.
 * @param msg	string que contem a mensagem de erro
 * @param halt	codigo de erro a ser retornado pelo programa (se != 0)
 * @date 17/05/2003
 */
void app_error (char *msg, int halt = 0)
{
  printf ("\n%s: %s\n",APP_NAME,msg);
  if (halt) {
    printf ("%s: programa abortado (erro=%d)\n",APP_NAME,halt);
    exit (halt);
  }
}



/**
 * Mostra informacoes sobre o programa, como nome, autor e outros.
 * @date 02/06/2002
 */
void show_credits ()
{
  DATETIME  date;
  char      period[32];

  clock_datetime (&date);
  if (date.dt_year > APP_START)
    sprintf (period,"%d-%d",APP_START,date.dt_year);
  else
    sprintf (period,"%d",date.dt_year);

/*  printf ("\tUniversidade Federal Fluminense - UFF\n");
  printf ("\tMestrado em Computacao\n");
  printf ("\tOrientadores : Luiz Satoru e Simone Lima\n");
  printf ("\tImplementacao: Geiza Silva, Copyright(c) %s\n",period);
  printf ("\tVersão       : %s - %s, %s\n\n",APP_VERSION,__DATE__,__TIME__);*/
}



/**
 * Mostra informacoes sobre como configurar o programa na linha de comando
 * @date 02/06/2002
*/
void show_help ()
{
  param_help (APP_NAME,APP_PARAMS);
}



/**
 * Funcao principal do programa
 * @param argc	numero de parametros digitados
 * @param argv	string formada pelos parametros digitados
 * @date 12/09/2003
 * @return 0, se correr tudo bem
 */
int main (int argc, char **argv)
{
  PSOLUCAO     sol;
  COMPTIME     tempo;
  char         buffer[256];



  // inicializa gerenciador de parametros de linha de comando
  param_init (par_info,tag_count (par_info),argv,argc);

  if (argc < 2) {       // se no. de argumentos insuficiente, mostra ajuda
    show_help ();
    return -1;
  }

  if (!param_check (buffer)) {
    app_error (buffer,-2);
    return -2;
  }

//  show_credits ();

  if ((param_asint (PAR_TAG_TAMELITE) >= (param_asint (PAR_TAG_ITER)/2)))
    app_error ("Valor de 't' deve ser menor que 'i'/2\n", 1);
  
  if (param_asint (PAR_TAG_MINSUP) >= param_asint (PAR_TAG_TAMELITE)) 
    app_error ("O suporte minimo deve ser menor que o tamanho do conjunto elite\n", 1);
  
    
   sol = ExecGRASPMD (param_asstr (PAR_TAG_INST), param_asint (PAR_TAG_NUMSERV), param_asint (PAR_TAG_ITER),
		param_asint (PAR_TAG_SEM), param_asint(PAR_TAG_ALFA), param_asint (PAR_TAG_PCT),
		param_asint (PAR_TAG_TAMELITE), (ESTRATEGIA_MD) param_asint (PAR_TAG_ESTRAT), 
		param_asint (PAR_TAG_MINSUP), param_asint (PAR_TAG_NPADROES), tempo);


  delete sol;
    
  return 0;
}
