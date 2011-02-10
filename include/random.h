
/**********************************************************************************
 * Arquivo  : random.h                                                            *
 * Criação  : 17/05/2002                                                          *
 * Autor    : Eyder Rios                                                          *
 * Descricao: Definicao de funcoes para geracao de numeros aleatorios.            *
 **********************************************************************************/

#ifndef __random_h__
#define __random_h__

#define XRND_DECIMALS         4       // no. maximo de casas decimais em xrandom()


void     xrandomize      (int seed = 0);
int      xrandom         (int limit);
int      xrandom_range   (int min, int max);
void     xrandom_couple  (int &rnd1, int &rnd2, int limit);

double   xfrand          ();
double   xfrandom        (double min);
double   xfrandom_range  (double min, double max);

#endif

