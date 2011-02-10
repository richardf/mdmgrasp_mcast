#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "grasprm.h"
#include "clock.h"
#include "utils.h"
#include "grasp.h"
#include "main.h"

#include <limits>
#include <cmath>

using namespace std;

/**
* Verifica se o double A é menor que o double B, considerando um erro epsilon.
* Knuth, Art of computer programming
*/
bool defLessThan(double a, double b)
{
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * 
std::numeric_limits<double>::epsilon()
);
}


void insereOrdenado(vector< pair<int,float> >& vetor, int roteador, double custo) {

   vetor.push_back(make_pair(0,0.0));
   int i;

   i = vetor.size()-2;

   for(; i >= 0 ;){

     if(vetor[i].second > custo) break;
     vetor[i+1] = vetor[i];
     i--;
  }

   vetor[i+1].first = roteador;
   vetor[i+1].second = custo;

/*   vetor.push_back(make_pair(0,0.0));
   int i;

   for(i = vetor.size()-2; (i >= 0) && (vetor[i].second < custo) ; vetor[i+1] = vetor[i--]) ;

   vetor[i+1].first = roteador;
   vetor[i+1].second = custo;
*/
}

void GRASPRM::ReconheceVizinhos(int roteador, vector<int>& vizinhos) {

   vector<int> viz_aux;
   vector<int>::iterator result;

   for(vector<ARVORE*>::iterator it = arvores.begin(); it < arvores.end(); it++) {
     (*it)->Vizinhos(roteador, viz_aux);
     for(unsigned int j = 0; j < viz_aux.size(); j++) {
       result = find(vizinhos.begin(), vizinhos.end(), viz_aux[j]);
       if(result == vizinhos.end())
         vizinhos.push_back(viz_aux[j]);
     }
     viz_aux.clear();
   }

   //cout << "ROTEADOR: " << roteador << " - VIZINHOS: ";
   //for(int b = 0; b < vizinhos.size(); b++)
   //  cout << vizinhos[b] << " ";
   //cout << "\n";
}

// Monta as árvores multicast
void GRASPRM::ConstroiArvores (char* arq_instancia)
{
  vector<ARESTA> arestas;
  ARESTA aresta;
  int qtd_fontes, fonte, qtd_arestas;

  ifstream instancia(arq_instancia);

  if( ! instancia.good() )
	app_error("Arquivo da instancia nao encontrado",7);


  instancia >> qtd_fontes;

  for(int i = 0; i < qtd_fontes; i++) {
    instancia >> fonte >> qtd_arestas;
    for(int j = 0; j < qtd_arestas; j++) {
       instancia >> aresta.origem >> aresta.destino >> aresta.custo;
       arestas.push_back(aresta);
    }

    arvores.push_back(new ARVORE(fonte, arestas));
    arestas.clear();
  }

  instancia.close();
}

void GRASPRM::IdentificaCandidatos() {
  vector<ARVORE*>::iterator i;
  for(i = arvores.begin(); i != arvores.end(); i++)
    (*i)->Candidatos(candidatos);
}


GRASPRM::GRASPRM (char* arq_instancia, int m, int n_iter, int sem, int alf, int pct, int te,
                  ESTRATEGIA_MD estrat, int sup, int n)
       : GRASP(n_iter, sem, pct, te, estrat, sup, n), semente(sem), numserv(m), alfa(alf/100.0)
{
   ConstroiArvores(arq_instancia);
   IdentificaCandidatos();
}

GRASPRM::~GRASPRM() {
  // Destroi as árvores
  vector<ARVORE*>::iterator i;
  for(i = arvores.begin(); i != arvores.end(); i++)
     delete *i;
}

void GRASPRM::Inicializacao ()
{
  // Imprime dados iniciais da execução
  cout << "semente: " << semente << "\n";

  //inicializa a semente aleatoria
  xrandomize(semente);
}

void GRASPRM::Finalizacao (PSOLUCAO sol)
{
  // Imprime dados finais da execução
}

void GRASPRM::DepoisConstrucao (int iter_cor, PSOLUCAO sol)
{
  // Retorna as árvores aos estados iniciais (A solução faz isso qdo eh criada)
  /* vector<ARVORE*>::iterator i;

  for(i = arvores.begin(); i != arvores.end(); i++)
     (*i)->Reset(); */
}

PSOLUCAO GRASPRM::Construcao (int iter_cor, const vector<int>* sol_parcial)
{
  unsigned int    ultimo;      // indice do ultimo candidato que pertence a LRC
  float           minimo;      // valor mínimo para um candidato entrar na LRC
  vector< pair<int,float> > CustosOrd;  // Custos dos roteadores candidatos em ordem decrescente
  PSOLUCAO        sol;         // Guarda a solucao construida
  int             i,x;
  unsigned int    j;
  float           custo_aux;   // custo de um roteador

  // Cria uma nova solucao
  sol = new SOLUCAO(this);
  // Inicia a solução com os elementos da solução parcial
  j = 0;
  if(sol_parcial != NULL) {
     j = sol_parcial->size();
     //printf("Padrão usado: ");
     for(unsigned int i = 0; i < j; i++) {
     //    printf("%d ", (*sol_parcial)[i]);
         sol->Adic ((*sol_parcial)[i]);
     }
     //printf("\n");
  }

  for (;j < numserv;j++) {
    // Para cada roteador candidato
    for (i = 0; i < sol->DiffQuant(); i++) {
      custo_aux = 0;
      //cout << "Nó que será avaliado: " << sol->Diff(i) << "\n";

      // Para cada árvore
      for(unsigned int a = 0; a < arvores.size(); a++) {
         custo_aux += (arvores[a])->CustoNo(sol->Diff(i));
	 //cout << "\tCusto na árvore \"" << a << "\": " << (arvores[a])->CustoNo(sol->Diff(i)) << "\n";
      }

      // cout << "\tTOTAL: " << custo_aux << "\n";

      insereOrdenado(CustosOrd, sol->Diff(i), custo_aux);

    }

    // Calcula valor mínimo para entrar na LRC
    minimo = alfa * CustosOrd[0].second;

    //cout << "MINIMO PARA ENTRAR NA RCL: " << minimo;

    // Encontra o ultimo item da LRC
    for(ultimo = 0; CustosOrd[ultimo].second >= minimo; ultimo++);

    if(ultimo > CustosOrd.size()) ultimo = CustosOrd.size();

    //cout << " TAMANHO DA RCL: " << ultimo;

    x = xrandom(ultimo);

    //cout << " ESCOLHIDO: " << x << "\n";

    sol->Adic(CustosOrd[x].first);

    //cout << "Entrou: " << CustosOrd[x].first << "\n";

    CustosOrd.clear();
  }

  return sol;

}

PSOLUCAO GRASPRM::BuscaLocal (PSOLUCAO sol, int& qtd_bl)
{
  bool     Trocou;
  double   val_sol;
  int      i_troca = 0,
           roteador_troca = 0;

  qtd_bl++;

  Trocou = false;

  val_sol = sol->Valor ();

  vector<int> vizinhos;
  int corrente;
  for(unsigned int i = 0; i < numserv; i++) {
     corrente = sol->Comp(i);
     ReconheceVizinhos(corrente, vizinhos);
     for(unsigned int j = 0; j < vizinhos.size(); j++) {
       sol->Troca(i, vizinhos[j]);
//       if(sol->Valor() < val_sol) {
	if ( defLessThan(sol->Valor(), val_sol) ) {
          Trocou = true;
	  i_troca = i;
	  roteador_troca = vizinhos[j];
	  val_sol = sol->Valor();
       }
       sol->Troca(i, corrente);
     }
     vizinhos.clear();
  }

  if (Trocou) {
    sol->Troca (i_troca, roteador_troca);
    return BuscaLocal(sol, qtd_bl);
  }

  return sol;
}


PSOLUCAO ExecGRASPMD (char* arq_instancia, int m, int n_iter, int sem, int alfa, int pct, int te,
                       ESTRATEGIA_MD estrat, int sup, int n, COMPTIME &tempo)
{
  PGRASPRM   grasp;
  PSOLUCAO   sol;


  grasp = new GRASPRM (arq_instancia, m, n_iter, sem, alfa, pct, te, estrat, sup, n);

  if (grasp != NULL)
    sol = (PSOLUCAO) grasp->Exec (&tempo);
  else
    sol = NULL;

  return sol;
}
