#ifndef __grasprm_h__
#define __grasprm_h__

#include "grasp.h"
#include "random.h"
#include "arvore.h"
#include "solucao.h"

class GRASPRM : public GRASP {
private:
  vector<ARVORE*> arvores;
  int semente;
  unsigned int numserv;
  vector<int> candidatos;
  float alfa;
  
  void      IdentificaCandidatos();
  void      ReconheceVizinhos(int roteador, vector<int>& vizinhos);
  void      DepoisConstrucao (int iter_cor, PSOLUCAO sol);
  void      ConstroiArvores(char* arq_instancia);
  
protected:

  void      Inicializacao();
  void      Finalizacao  (PSOLUCAO sol);
  
  
  PSOLUCAO  Construcao   (int iter_cor, const vector<int>* sol_parcial = NULL);
  PSOLUCAO  BuscaLocal   (PSOLUCAO sol, int& qtd_bl);

public:
  GRASPRM (char* arq_instancia, int m, int n_iter, int sem, int alf, int pct, int te, 
           ESTRATEGIA_MD estrat, int sup, int n);
 ~GRASPRM ();
  
  friend class SOLUCAO;
};

typedef GRASPRM  *PGRASPRM;

PSOLUCAO ExecGRASPMD (char* arq_instancia, int m, int n_iter, int sem, int alfa, int pct, int te, 
                       ESTRATEGIA_MD estrat, int sup, int n, COMPTIME &tempo);
#endif
