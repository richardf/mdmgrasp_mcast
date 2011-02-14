#ifndef __solucao_h__
#define __solucao_h__
#include "grasp.h"
#include <fstream>
#include <vector>
#include <iostream>

#define MULTIPLICADOR_HASH 37 //n�mero primo para c�lculo do hash

using namespace std;

class GRASPRM;

/** Par com custo da solu��o e seu valor hash, usado para filtrar repeti��es */
class ParHash
{
    public:
        double valor;
        long int hash;
        bool operator < (const ParHash&) const;
};

class SOLUCAO
{
private:
  GRASPRM       *grasp;     // GRASP
  vector<int>    N_M,
                 M;
  double         ValSol;

public:
  SOLUCAO (GRASPRM *grasprm);

  int        Quant    ()         { return M.size ();   }
  int        Comp     (int idx)  { return M[idx];      }
  int        Diff     (int idx)  { return N_M[idx];    }
  int        DiffQuant()         { return N_M.size (); }

  void       Troca         (int m_idx, int roteador);

  void       Exibe         (char *texto = NULL);
  void       ExibeN_M      (char *texto = NULL);
  double      Valor         ();

  bool       Adic  (int elem);
  bool       Remove(int elem);

  ParHash&   Hash();	        //retorna o valor hash da solu��o

  std::vector<int> tovector();

  friend class GRASPRM;
};

typedef SOLUCAO *PSOLUCAO;

#endif
