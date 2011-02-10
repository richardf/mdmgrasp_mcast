
#ifndef __pool_h__
#define __pool_h__

#include <vector>

using namespace std;

typedef enum { EMD_HLP = 0, EMD_HLS, EMD_HnLP, EMD_HnMP } ESTRATEGIA_MD;

class Pool {
    int tamanho,			// Quantidade corrente de solu��es no pool
        capacidade;			// Quantidade m�xima de solu��es no pool
    vector< vector<int>* > pool;	// Vetor onde ficam armazenadas as solu��es
    					// Uma solu��o � representada por um vetor de int
    vector<double> valores;		// Armazena os valores das solu��es no pool
    void imprime();			// Gera o arquivo a ser minerado
public:
    Pool(int cap);
    bool Inserir(const vector<int>& sol, double valor);
    inline bool Cheio() { return tamanho == capacidade; };
    inline int Tamanho() { return tamanho; };
    void ReconhecePadroes(vector< vector<int> >& padroes, ESTRATEGIA_MD est, 
                          int min_sup, int n, int semente = 1);
    ~Pool() { while(tamanho > 0) delete pool[--tamanho]; };
};

#endif
