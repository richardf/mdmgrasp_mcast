#include "arvore.h"
#include <algorithm>
#include <math.h>

#define MINIMO 0.0001

double fatorial (int num)
{
 double result=1.0;
 for (int i=1; i<=num; ++i)
    result*=i;
 return result;
}

double combinacao (int n, int k) {
  double result = 1.0;
  int maior = k;
    
  if(n-k > k)
    maior = n-k;
  
  for(int i = n; i > maior; i--)
     result *= i;
     
  return result / fatorial(n-maior);
}

// Calcula custo da árvore (e de todos os nós) guardando informacoes relevantes nos nós
void ARVORE::AvaliaCusto(PNO no) {
  
  double incremento; 
  int k = 1;
  
  double Ek = 1.0;
  
  no->links_abaixo = 0; // Qtd de links na sub-árvore de raiz nó
  no->links_abaixo_orig = 0;
  
  no->soma_custos_subarvores_replica = 0.0;
  no->F->push_back(0.0); // probabilidade de 0 pacotes serem suficientes  
  no->F_orig->push_back(0.0);
    
  // printf("%d\n",no->id);
  
  if(no->filhos == NULL) { 
     incremento = no->prob_perda/100.0; 
     // printf("1 - P[%d] = %f\n", k,incremento);
     while(incremento > MINIMO) {       
       no->F->push_back(1.0 - incremento);
       no->F_orig->push_back(1.0 - incremento);
       k++;  
       incremento = pow(no->prob_perda/100.0, k); 
       // printf("1 - P[%d] = %f\n", k,incremento);
     }
     no->F->push_back(1.0);
     no->F_orig->push_back(1.0);
     
     no->custo = no->custo_orig = 0.0;     
  }   
  else {
    vector<double> F_filhos;    
    for(PFILHOS p = no->filhos; p != NULL; p = p->prox) {
      AvaliaCusto(p->no);
      if(F_filhos.size() == 0)
        F_filhos = *(p->no->F);
      else {
        if(p->no->F->size() > F_filhos.size())
          F_filhos.resize(p->no->F->size(), 1.0);
        for(int i = 0; i < p->no->F->size(); i++)
           F_filhos[i] *= p->no->F->at(i);
      }      
        
      no->links_abaixo += p->no->links_abaixo + 1;      
      
    }
    
    double f;
    // Cálculo de E[K]
    if(no->pai == NULL)
      f = F_filhos[1];    
    else
      f = (1.0 - (no->prob_perda/100.0)) * F_filhos[1];
    incremento = 1.0 - f;
    // printf("1 - P[%d] = %f (f = %f - filho = %f)\n", k,incremento, f, F_filhos[1]);
    double aux;
    while(incremento > MINIMO) {
      Ek += incremento;      
      no->F->push_back(f);
      no->F_orig->push_back(f);
      
      if(++k >= F_filhos.size())
        aux = 1.0;
      else
        aux = F_filhos[k];    
        
      if(no->pai == NULL)
        f = aux;
      else {
        int l;	
        for(l = 0, f = 0; l < k; l++) {
	  if ((k-l) >= F_filhos.size())
	     aux = 1.0;
	  else
	     aux = F_filhos[k-l];
	  
          f += combinacao(k,l) * pow(no->prob_perda/100.0,l) * pow(1-no->prob_perda/100.0,k-l) * aux; 
	}
      }
      incremento = 1.0 - f;
      // printf("1 - P[%d] = %f (f = %f - filho = %f)\n", k,incremento, f, aux);
    }
    
    no->F->push_back(1.0);
    no->F_orig->push_back(1.0);
    
    F_filhos.clear();
    no->links_abaixo_orig = no->links_abaixo;
    
    no->custo = no->custo_orig = no->links_abaixo * Ek;
    
  }  
}

void ARVORE::AtualizaCusto(PNO no, bool trocado) {
  double incremento, f, Ek = 1.0;
  int k = 1;
  vector<double> F_filhos;    

  no->F->clear();
  no->F->push_back(0);    

  for(PFILHOS p = no->filhos; p != NULL; p = p->prox) {      
    if(F_filhos.size() == 0) {
      F_filhos = *(p->no->F);
      
    }
    else {
      if(p->no->F->size() > F_filhos.size())
        F_filhos.resize(p->no->F->size(), 1.0);
      for(int i = 0; i < p->no->F->size(); i++)
        F_filhos[i] *= p->no->F->at(i);           
    }
          
  }  

  if(no->eh_replica || no->pai == NULL) {
    if(no->eh_replica) {
      
      incremento = no->prob_perda/100.0; 
      while(incremento > MINIMO) {
        no->F->push_back(1.0 - incremento);
        k++;
	incremento = pow(no->prob_perda/100.0, k); 
      }
      no->F->push_back(1.0);
    }
    
    f = (double)F_filhos[k = 1];
    incremento = 1.0 - f;

    while(incremento > MINIMO) {
      Ek += incremento;
      if (++k >= F_filhos.size()) 
         f = 1.0;
      else
         f = F_filhos[k];
      incremento = 1.0 - f;
   }
    
    no->custo = no->links_abaixo * Ek;
  }
  else {
    f = (1 - no->prob_perda/100.0) * F_filhos[1];
    incremento = 1 - f;
    double aux;
    while(incremento > MINIMO) {
      Ek += incremento;
      no->F->push_back(f);
      int l;
      k++;
      for(l = 0, f = 0; l < k; l++) {
        if((k-l) >= F_filhos.size())
	  aux = 1.0;
	else
	  aux = F_filhos[k-l];  
      
        f += combinacao(k,l) * pow(no->prob_perda/100.0,l) * pow(1-no->prob_perda/100.0,k-l) * aux;
      }
      incremento = 1 - f;
    }
    no->F->push_back(1.0);
  }  

  if(trocado) {
    
    PNO pai; double replica_antes, replica_depois;
    
    if(no->eh_replica) {
      
      for(pai = no->pai; pai != NULL; pai = pai->pai) {
	if(pai->eh_replica) {
	  replica_antes = pai->custo + pai->soma_custos_subarvores_replica;
 	  pai->links_abaixo -= no->links_abaixo;
          pai->soma_custos_subarvores_replica += no->custo;   
	  AtualizaCusto(pai, false);
	  break;
	}        
	pai->links_abaixo -= no->links_abaixo;
        pai->soma_custos_subarvores_replica += no->custo; // O pai já considera os custos das subarvores do no	
	AtualizaCusto(pai, false);
      }
    }  
    else {
   
      for(pai = no->pai; pai != NULL; pai = pai->pai) {
	if(pai->eh_replica) {
	  replica_antes = pai->custo + pai->soma_custos_subarvores_replica;
 	  pai->soma_custos_subarvores_replica -= no->custo; 
          pai->links_abaixo += no->links_abaixo;        
          AtualizaCusto(pai, false);
	  break;
        }
	pai->links_abaixo += no->links_abaixo;
        pai->soma_custos_subarvores_replica -= no->custo; // O pai já considera os custos das subarvores do no	
	AtualizaCusto(pai, false);
      }
    }  
    
    // Atualiza o custo das subarvores dos nós acima
    if(pai != NULL) {
      replica_depois = pai->custo + pai->soma_custos_subarvores_replica;
      for(pai = pai->pai; pai != NULL; pai = pai->pai)
	pai->soma_custos_subarvores_replica -= replica_antes-replica_depois;
    }
  }
  
  F_filhos.clear();
  if(!(no->eh_replica)) no->custo = no->links_abaixo * Ek;
}

// Constrói a árvore e calcula seu custo original (sem réplicas)
ARVORE::ARVORE(int fonte, vector<ARESTA>& arestas) : source(fonte) {
   PNO no_dest, no_orig, raiz;
   
   // Cria a raiz (fonte)
   raiz = (PNO) malloc (sizeof(NO));
   raiz->prob_perda = -1;
   raiz->eh_replica;
   raiz->filhos = NULL;
   raiz->pai = NULL;
   raiz->id = fonte;
   raiz->F = new vector<double>(0);
   raiz->F_orig = new vector<double>(0);
   
   nos.push_back(make_pair(fonte, raiz));

   // Insere os outros nós
   for(int i = 0; i < arestas.size(); i++) {
      Iter itNo = Acha(arestas[i].origem);
      if(itNo == NULL) {
          no_orig = (PNO) malloc (sizeof(NO));	  
	  no_orig->id = arestas[i].origem;
          no_orig->prob_perda = arestas[i].custo;
          no_orig->eh_replica = false;
          no_orig->filhos = NULL;
	  no_orig->F = new vector<double>(0);
          no_orig->F_orig = new vector<double>(0);
	        
          nos.push_back(make_pair(arestas[i].origem, no_orig));

      }
      else {
          
	  itNo->second->prob_perda = arestas[i].custo;
	  no_orig = itNo->second;
      }  
      
      itNo = Acha(arestas[i].destino);

      if(itNo == NULL) {
          no_dest = (PNO) malloc (sizeof(NO));
	  no_dest->id = arestas[i].destino;
	  no_dest->eh_replica = false;
	  no_dest->filhos = (PFILHOS) malloc (sizeof(FILHOS));
	  no_dest->filhos->no = no_orig;
	  no_dest->filhos->prox = NULL;
	  no_dest->F = new vector<double>(0);
          no_dest->F_orig = new vector<double>(0);
   
	  nos.push_back(make_pair(arestas[i].destino, no_dest));
	  
      }
      else {
          PFILHOS filho = (PFILHOS) malloc (sizeof(FILHOS));
	  filho->no = no_orig;
	  filho->prox = itNo->second->filhos;
          itNo->second->filhos = filho;
	  no_dest = itNo->second;
      }

      no_orig->pai = no_dest;

   }
   
   AvaliaCusto(raiz);   
   
}

// Libera um nó da árvore e todos os seus filhos
void ARVORE::Libera(PNO no) {
  PFILHOS perc_aux, perc = no->filhos;
  
  while(perc != NULL) {
    Libera(perc->no);    
    perc_aux = perc;
    perc = perc->prox;
    free(perc_aux);
  }
  
  delete no->F;
  delete no->F_orig;
  free(no);
  no = NULL;
}

// Destrói todos os nós da árvore
ARVORE::~ARVORE() {
  Libera((Acha(source))->second);
}

// Retorna a árvore ao seu estado original
void ARVORE::Reset() {
  
  for(int i = 0; i < nos.size(); i++) {
    nos[i].second->custo = nos[i].second->custo_orig;
    nos[i].second->eh_replica = false;
    nos[i].second->links_abaixo = nos[i].second->links_abaixo_orig;
    *(nos[i].second->F) = *(nos[i].second->F_orig);
    nos[i].second->soma_custos_subarvores_replica = 0.0;    
  }  
}
  
// Seta um servidor como servidor de replicação e recalcula o custo da árvore
void ARVORE::setaReplica(int n) {
  Iter it = Acha(n);
  if (it != NULL) {
	it->second->eh_replica = true;
	AtualizaCusto(it->second);
  }
}

// Seta um servidor como servidor de replicação e recalcula o custo da árvore
void ARVORE::tiraReplica(int n) {
  Iter it = Acha(n);
  if (it != NULL) {
	it->second->eh_replica = false;
	AtualizaCusto(it->second);
  }
}

void ARVORE::Candidatos(vector<int>& cand) {
    
  for(int i = 0; i < nos.size(); i++)
    if(nos[i].first != source && nos[i].second->filhos != NULL)
       if(find(cand.begin(), cand.end(), nos[i].first) == cand.end())
         cand.push_back(nos[i].first);
}

void ARVORE::Configura(vector<int>& nos_replica) {
  Reset();
  for(vector<int>::iterator it = nos_replica.begin(); it != nos_replica.end(); it++)
    setaReplica(*it);
}

void ARVORE::Vizinhos(int roteador, vector<int>& vizinhos) {
  Iter it = Acha(roteador);
  
  if(it != NULL) {
    if(it->second->pai != NULL)
      if(it->second->pai->pai != NULL)
        if(!(it->second->pai->eh_replica))
          vizinhos.push_back(it->second->pai->id);
    for(PFILHOS p = it->second->filhos; p != NULL; p = p->prox)
      if(p->no->filhos != NULL)
        if(!(p->no->eh_replica))
          vizinhos.push_back(p->no->id);
  }    
}

Iter ARVORE::Acha(int n) {
   
   for(int i = 0; i < nos.size(); i++) {
      if(nos[i].first==n) return &(nos[i]);
   }   
   return NULL;
}
   
double ARVORE::CustoNo(int n) { 

  Iter it = Acha(n); 

  if(it == NULL)
    return 0.0;
  else
    return (it->second->custo + it->second->soma_custos_subarvores_replica); 
}
     
double ARVORE::Custo() { 
  Iter it = Acha(source); 
  return (it->second->custo + it->second->soma_custos_subarvores_replica); 
}
