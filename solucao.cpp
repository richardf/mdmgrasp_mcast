#include <algorithm>
#include <vector>

#include "grasprm.h"
#include "solucao.h"

using namespace std;

bool ParHash::operator < (const ParHash& dir) const
{
    /** Compara inicialmente o custo da solu��o */
    if (valor < dir.valor)
        return true;
    if (valor > dir.valor)
        return false;

    /** Para custos iguais, compara o hash */
    return hash < dir.hash;
}

SOLUCAO::SOLUCAO (GRASPRM *grasprm)
{
  grasp  = grasprm;
  ValSol = -1;

  N_M = grasp->candidatos;

  //grasp->N = new CONJUNTO_INT(&N_M);

  for(vector<ARVORE*>::iterator it = grasp->arvores.begin(); it < grasp->arvores.end(); it++)
    (*it)->Reset();
}

double SOLUCAO::Valor ()
{
  if (ValSol <= 0) {
    ValSol = 0;
    for(vector<ARVORE*>::iterator it = grasp->arvores.begin(); it != grasp->arvores.end(); it++)
      ValSol += (*it)->Custo();
  }

  return ValSol;
}

bool SOLUCAO::Adic (int comp)
{
  if (M.size() < grasp->numserv) {           // adiciona comp a solucao
    M.push_back(comp);
    ValSol = -1;
    vector<int>::iterator it = find(N_M.begin(), N_M.end(), comp);
    N_M.erase(it);
    for(vector<ARVORE*>::iterator it = grasp->arvores.begin(); it != grasp->arvores.end(); it++)
      (*it)->setaReplica(comp);

    return true;   // remove componente do conj N - M
  }

  return false;
}

bool SOLUCAO::Remove (int comp)
{
    if (M.size() > 0) {
        N_M.push_back(comp);
        ValSol = -1;
        vector<int>::iterator it = find(M.begin(), M.end(), comp);
        M.erase(it);
        for (vector<ARVORE*>::iterator it = grasp->arvores.begin(); it != grasp->arvores.end(); it++)
            (*it)->tiraReplica(comp);

        return true; 
    }
    return false;
}

void SOLUCAO::Troca (int m_idx, int roteador)
{
  int aux;

  aux         = M[m_idx];
  // M[m_idx]    = N_M[nm_idx];
  M[m_idx]    = roteador;
  // N_M[nm_idx] = aux;

  for(vector<ARVORE*>::iterator it = grasp->arvores.begin(); it < grasp->arvores.end(); it++) {
    (*it)->tiraReplica(aux);
    (*it)->setaReplica(roteador);
  }

  ValSol = -1;
}

void SOLUCAO::Exibe (char *texto)
{

	for(unsigned int i = 0; i < M.size(); i++)
	cout << M[i] << " ";
	cout << "\n";
  /*char buffer[128];

  if (texto != NULL)
    sprintf (buffer,"%s|%d| = ",texto,(int) Valor ());
  else
    sprintf (buffer,"|%d| = ",(int) Valor ());

  M.Exibe (buffer); */
}

void SOLUCAO::ExibeN_M (char *texto)
{ /*
  char buffer[128];

  if (texto != NULL)
    sprintf (buffer,"%s ",texto);

  N_M.Exibe (buffer); */
}

ParHash& SOLUCAO::Hash()
{
    ParHash *ptr = new ParHash();
    long int total = 0;
    for (unsigned int i = 0; i < M.size(); i++)
    {
        total += M[i]*MULTIPLICADOR_HASH;
    }

    (*ptr).hash = total;
    (*ptr).valor = Valor();

    return *ptr;
}

void SOLUCAO::setvector(vector<int> novoM)
{
    
    while(M.size()>0){
        
        Remove(M[0]);
    }

    for(int j=0;j<novoM.size();j++){
        
        Adic(novoM[j]);
    }
}

std::vector<int> SOLUCAO::tovector() {
  return M;
}
