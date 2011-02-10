#ifndef __arvore_h__
#define __arvore_h__

#include <map>
#include <vector>
#include <iostream>


using namespace std;

struct aresta {
  int origem,
      destino,
      custo;
};
 
typedef struct aresta ARESTA;

typedef struct filhos FILHOS, *PFILHOS;

struct no {
  double         custo,
                custo_orig,
		soma_custos_subarvores_replica;
  int           id,
                prob_perda,
                links_abaixo,
		links_abaixo_orig;
  bool          eh_replica;
  vector<double> *F_orig,
                *F;  
  struct no*    pai;
  PFILHOS       filhos;
};
 
typedef struct no NO, *PNO;

struct filhos {
  PNO     no;
  PFILHOS prox;
};

typedef pair<int, PNO>* Iter;

class ARVORE {
 private:
 
   int           source;
   vector< pair<int, PNO> > nos;
   
   Iter  Acha(int n);
   void  AvaliaCusto(PNO no);
   void  AtualizaCusto(PNO no, bool trocado = true);
   void  Libera(PNO no);
   
 public:
 
   ARVORE(int fonte, vector<ARESTA>& arestas);
  ~ARVORE();
  
   double CustoNo(int n); 
   double Custo();
   void Reset();
   void setaReplica(int n);
   void tiraReplica(int n);
   void Vizinhos(int roteador, vector<int>& vizinhos);
   void Configura(vector<int>& nos_replica);
   void Candidatos(vector<int>& cand);
   
};

typedef ARVORE *PARVORE;

#endif
