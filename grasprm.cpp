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
* Verifica se o double A � menor que o double B, considerando um erro epsilon.
* Knuth, Art of computer programming
*/
bool defLessThan(double a, double b)
{
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * std::numeric_limits<double>::epsilon());
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

// Monta as �rvores multicast
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
  // Destroi as �rvores
  vector<ARVORE*>::iterator i;
  for(i = arvores.begin(); i != arvores.end(); i++)
     delete *i;
}

void GRASPRM::Inicializacao ()
{
  // Imprime dados iniciais da execu��o
  cout << "semente: " << semente << "\n";

  //inicializa a semente aleatoria
  xrandomize(semente);
}

void GRASPRM::Finalizacao (PSOLUCAO sol)
{
  // Imprime dados finais da execu��o
}

void GRASPRM::DepoisConstrucao (int iter_cor, PSOLUCAO sol)
{
  // Retorna as �rvores aos estados iniciais (A solu��o faz isso qdo eh criada)
  /* vector<ARVORE*>::iterator i;

  for(i = arvores.begin(); i != arvores.end(); i++)
     (*i)->Reset(); */
}

PSOLUCAO GRASPRM::Construcao (int iter_cor, const vector<int>* sol_parcial) {
    return Construcao(iter_cor, sol_parcial, 0, NULL);
}


PSOLUCAO GRASPRM::Construcao (int iter_cor, const vector<int>* sol_parcial, int qtdRemovidos, int elemens_removidos[])
{ 
  unsigned int    ultimo;      // indice do ultimo candidato que pertence a LRC
  float           minimo;      // valor m�nimo para um candidato entrar na LRC
  vector< pair<int,float> > CustosOrd;  // Custos dos roteadores candidatos em ordem decrescente
  PSOLUCAO        sol;         // Guarda a solucao construida
  int             i,x;
  unsigned int    j;
  float           custo_aux;   // custo de um roteador

  // Cria uma nova solucao
  sol = new SOLUCAO(this);
  // Inicia a solu��o com os elementos da solu��o parcial
  j = 0;
  if(sol_parcial != NULL) {
     j = sol_parcial->size();
     //printf("Padr�o usado: ");
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
      //cout << "N� que ser� avaliado: " << sol->Diff(i) << "\n";

      // Para cada �rvore
      for(unsigned int a = 0; a < arvores.size(); a++) {
         custo_aux += (arvores[a])->CustoNo(sol->Diff(i));
	 //cout << "\tCusto na �rvore \"" << a << "\": " << (arvores[a])->CustoNo(sol->Diff(i)) << "\n";
      }

      // cout << "\tTOTAL: " << custo_aux << "\n";

      insereOrdenado(CustosOrd, sol->Diff(i), custo_aux);

    }

    // Calcula valor m�nimo para entrar na LRC
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
/**
 * Gera a proxima combinacao de N elementos
 *
 * @param comb  A combinacao anterior
 * @param   k   O tamanho dos subconjuntos a gerar
 * @param   n   O tamanho do conjunto original
 * @return      true se uma combinacao valida foi gerada. false caso contrario
 **/
bool GRASPRM::proxima_comb(int comb[], int k, int n)
{
    int i = k - 1;
    ++comb[i];

    while ((i > 0) && (comb[i] >= n - k + 1 + i)) {
        --i;
        ++comb[i];
    }

    if (comb[0] > n - k) /* Combinacao (n-k, n-k+1, ..., n) atingida */
        return false; /* Nao existem mais combinacoes */

    /* comb esta assim (..., x, n, n, n, ..., n).
    Ficara assim: (..., x, x + 1, x + 2, ...) */
    for (i = i + 1; i < k; ++i)
        comb[i] = comb[i - 1] + 1;

    return true;
}

/**
 * Essa busca local remove k elementos da solucao, que sera completada
 * novamente pela fase de construcao.
 **/
PSOLUCAO GRASPRM::BuscaLocalQueRemoveElementos(PSOLUCAO sol, int& qtd_bl, int numero_elementos_remover)
{
    int tamanho_da_sol = sol->Quant(); 
    int comb[numero_elementos_remover]; /* comb[i] eh o indice do i-esimo elemento da combinacao.
                                         * Este vetor indica os elementos que serao removidos da solucao */
    int elems_remover[numero_elementos_remover];
    double val_sol;


    /* Prepara comb para a combinacao inicial */
    for (int i = 0; i < numero_elementos_remover; ++i)
        comb[i] = i;


    bool tem_mais_combinacoes = true;
    bool sol_melhorou = false;

    while(!sol_melhorou && tem_mais_combinacoes) {

        val_sol = sol->Valor();
        
        /* separa os elementos a serem removidos */
        for(int i=0; i < numero_elementos_remover; i++) {
            elems_remover[i] = sol->Comp(comb[i]);
        }

        /* e os remove */
        for(int i=0; i < numero_elementos_remover; i++) {
            sol->Remove(elems_remover[i]);
        }

        //chama a fase de construcao
        

        if (defLessThan(sol->Valor(), val_sol)) {
            sol_melhorou = true;
            return BuscaLocalQueRemoveElementos(sol, qtd_bl, numero_elementos_remover);
        }

        tem_mais_combinacoes = proxima_comb(comb, numero_elementos_remover, tamanho_da_sol);
    }



    
//    bool Trocou;
//
//    int i_troca = 0,
//            roteador_troca = 0;
//
//    qtd_bl++;
//
//    Trocou = false;
//
//
//
//    vector<int> vizinhos;
//    int corrente;
//    for (unsigned int i = 0; i < numserv; i++) {
//        corrente = sol->Comp(i);
//        ReconheceVizinhos(corrente, vizinhos);
//        for (unsigned int j = 0; j < vizinhos.size(); j++) {
//            sol->Troca(i, vizinhos[j]);
//            //       if(sol->Valor() < val_sol) {
//            if (defLessThan(sol->Valor(), val_sol)) {
//                Trocou = true;
//                i_troca = i;
//                roteador_troca = vizinhos[j];
//                val_sol = sol->Valor();
//            }
//            sol->Troca(i, corrente);
//        }
//        vizinhos.clear();
//    }
//
//    if (Trocou) {
//        sol->Troca(i_troca, roteador_troca);
//        return BuscaLocalQueRemoveElementos(sol, qtd_bl, 0);
//    }

    return sol;
}