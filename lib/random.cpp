
/**********************************************************************************
 * Arquivo  : random.cpp                                                          *
 * Criação  : 17/05/2002                                                          *
 * Autor    : Eyder Rios                                                          *
 * Descricao: Definicao de funcoes para geracao de numeros aleatorios.            *
 **********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "random.h"


/***********************************************************************************
 * Funcao    : void xrandomize (int seed)                                          *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Inicializa a semente para geracao de numeros  aleatorios. A  funcao *
 *             chama a funcao da biblioteca C srand(). Se 'seed' for zero passando *
 *             uma  semente  diferente a cada chamada.                             *
 * Parametros: seed  - semente para inicializacao do gerador de no. aleatorios     *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

void xrandomize (int seed)
{
  if (seed == 0)
    seed = (int) time (NULL);

  srand (seed);                     // a semente e' o no. de segundos desde 1/1/1970
}

/***********************************************************************************
 * Funcao    : int xrandom (int limit)                                             *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Gera um numero aleatorio entre 0 e limit - 1, inclusive.            *
 * Parametros: limit   - limite superior do numero aleatorio gerado, exclusive.    *
 * Retorno   : Retorna o numero aleatorio gerado.                                  *
 ***********************************************************************************/

int xrandom (int limit)
{
  return (int) (limit * (rand ()/(RAND_MAX + 1.0)));
}

/***********************************************************************************
 * Funcao    : int xrandom_range (int min, int max)                                *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Gera um numero aleatorio entre min e max,inclusive. Se  min <= max, *
 *             a funcao retorna min.                                               *
 * Parametros: min     - limite inferior do numero aleatorio gerado, inclusive.    *
 *             max     - limite superior do numero aleatorio gerado, inclusive.    *
 * Retorno   : Retorna o numero aleatorio gerado.                                  *
 ***********************************************************************************/

int xrandom_range (int min, int max)
{
  long delta;

  delta = max - min;
  if (delta > 0)
    return  min + xrandom (delta + 1);

  return min;
}

/***********************************************************************************
 * Funcao    : void xrandom_couple (int &rnd1,int &rnd2,int limit)                 *
 * Criacao   : 17/07/2003                                                          *
 * Descricao : Gera dois numeros aleatorios diferentes entre 0 e 'limit' - 1.      *
 * Parametros: rnd1    - primeiro no. aleatorio gerado                             *
 *             rnd2    - segundo  no. aleatorio gerado                             *
 *             limit   - limite superior do numero aleatorio gerado, exclusive.    *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

void xrandom_couple  (int &rnd1, int &rnd2, int limit)
{
  // gera primeiro no. aleatorio
  rnd1 = xrandom (limit);

  // se nao possibilidade de gerar dois nos. diferentes, retorna
  if (limit < 2) {
    rnd2 = rnd1;
    return;
  }

  do {
    rnd2 = xrandom (limit);
  } while (rnd2 == rnd1);
}

/***********************************************************************************
 * Funcao    : double xfrand ()                                                    *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Gera um numero aleatorio entre 0 e 1, inclusive                     *
 * Parametros: Nenhum                                                              *
 * Retorno   : Retorna o numero aleatorio gerado.                                  *
 ***********************************************************************************/

double xfrand ()
{
  return ((double) rand () / (double) RAND_MAX);
}

/***********************************************************************************
 * Funcao    : double xfrandom (double limit)                                      *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Gera um numero aleatorio entre 0 e limit - 1, inclusive.            *
 * Parametros: limit    - limite superior do numero aleatorio gerado, exclusive.   *
 * Retorno   : Retorna o numero aleatorio gerado.                                  *
 ***********************************************************************************/

double xfrandom (double limit)
{
  return limit * ((double) rand () / (RAND_MAX + 1.0));
}

/***********************************************************************************
 * Funcao    : double xfrandom_range (double min, double max)                      *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Gera um numero double aleatorio entre min e max,inclusive.          *
 *             Se  max <= min, a funcao retorna min.                               *
 * Parametros: min     - limite inferior do numero aleatorio gerado, inclusive.    *
 *             max     - limite superior do numero aleatorio gerado, inclusive.    *
 * Retorno   : Retorna o numero aleatorio gerado.                                  *
 ***********************************************************************************/

double xfrandom_range (double min, double max)
{
  if (max < min)
    return min;

  return min + (max - min) * xfrand ();
}

