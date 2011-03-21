#ifndef __grasp_h__
#define __grasp_h__

#include "params.h"
#include "utils.h"
#include "clock.h"
#include "solucao.h"
#include "pool.h"
#include "main.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <sys/times.h>
#include <set>



class GRASP
{
private:
  PSOLUCAO         melhor_sol; /** A melhor solucao encontrada*/
  int              iter_cor, /** iteracao corrente do grasp*/
                   tam_elite, /** Tamanho do conjunto elite*/
                   semente,
                   min_sup, /** Suporte minimo*/
                   npadroes,
				   iter_pool_igual; /** numero de iteracoes que o pool deve ficar igual p/ ocorrer uma md */;
  set<ParHash>     construidas; /**set que armazena o hash das solu��es iniciais j� constru�das */
  ESTRATEGIA_MD    est;

protected:

  int              iter_max; /** */
  int              iter_max_total; /** */


  virtual PSOLUCAO Construcao (int, const vector<int>* sol_parcial = NULL) = 0;
  virtual PSOLUCAO Construcao (const vector<int>* sol_parcial, vector<int>* elemens_removidos) = 0;

  virtual PSOLUCAO BuscaLocal (PSOLUCAO sol, int& qtd_bl) = 0;
  virtual PSOLUCAO BuscaLocalQueRemoveElementos   (PSOLUCAO sol, int& qtd_bl, int numero_elementos_remover) = 0;

  virtual void Inicializacao    () { };

  virtual void AntesIteracao    (int) { };
  virtual void DepoisIteracao   (int, PSOLUCAO) { };

  virtual void AntesConstrucao  (int) { };
  virtual void DepoisConstrucao (int, PSOLUCAO) { };

  virtual void AntesBuscaLocal  (int, PSOLUCAO) { };
  virtual void DepoisBuscaLocal (int, PSOLUCAO) { };

  virtual void Finalizacao      (PSOLUCAO)  { };

  virtual PSOLUCAO Executa  ();


public:
           GRASP  (int max_iter, int sem, int pct, int te, ESTRATEGIA_MD estrat, int sup, int n);
  virtual ~GRASP  ();

  virtual int       Iter    ();
  virtual int       IterMax ();

  virtual PSOLUCAO Exec    (PCOMPTIME tempo = NULL);
  virtual PSOLUCAO Solucao ();
};

typedef GRASP    *PGRASP;

#endif
