#include "pool.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include<stdio.h>

// Arquivos tempor�rios
#define ARQ_ELITE "./md/arq_elite.txt"
#define ARQ_PADROES "./md/arq_padroes.txt"
#define OUTPUT_MD "./md/output_md.txt"

Pool::Pool(int cap) : pool(cap), valores(cap), capacidade(cap), tamanho(0) { 
    for(int i = 0; i < cap; i++) { 
        valores.push_back(0.0);
	pool.push_back(NULL); 
    } 
}

//retorno alterado para boolean p/ medir variacao do pool
//retorna 1 caso o pool seja alterado.
bool Pool::Inserir(const vector<int>& sol, double valor) { 
    // As melhores solu��es s�o as de menor valor
    if(valores.front() > valor || tamanho < capacidade) {
	int i = 0;

	// Evita solu��es repetidas
	vector<int> sol_ord(sol);        
	sort(sol_ord.begin(), sol_ord.end());
	for(int t = capacidade-1; pool[t] != NULL && t >= 0; t--)
	   if(*(pool[t]) == sol_ord) return false;
	   
	// Encontra posi��o para inser��o
	while((valores[i] > valor || valores[i] == 0.0) && i < capacidade) i++;

	--i;

	int t = 0;		
	
	// Remove a pior solu��o
	if(tamanho == capacidade) { 
	    delete pool.front();
	    t = 1;
	}
	else    
	   for(; t < capacidade && !pool[t]; t++);
	
	// Move solu��es e valores para esquerda, liberando a posi��o p/ nova solu��o
	while(t <= i) {
	    valores[t-1] = valores[t];
	    pool[t-1] = pool[t++];
	}
        
	// Insere solu��o
	valores[i] = valor;
	pool[i] = new vector<int>(sol_ord);
        if(tamanho < capacidade) tamanho++;
	
	return true;
    }     
    return false;
}

void Pool::imprime() {
    FILE* arq_elite;
    
    arq_elite = fopen(ARQ_ELITE, "w");
    
    int j, t;
    for (int i = capacidade - 1; pool[i] != NULL && i >= 0; i--) {
        for (j = 0, t = pool[i]->size(); j < t; j++)
            fprintf(arq_elite, "%d ", (*pool[i])[j]);
        fprintf(arq_elite, "\n");
    }
    
    fclose(arq_elite);
}

void Pool::ReconhecePadroes(vector< vector<int> >& padroes, ESTRATEGIA_MD est, 
                            int min_sup, int n, int semente) {
    
    imprime();
    
    // Vari�vel usada p/ a montagem do comando de chamada ao prog. de recon. de padr�es
    ostringstream comando;

    switch(est) {
        case EMD_HLP: 
            // Aciona o algoritmo para extrair o maior padr�o de todos
            comando << "./md/dci3_hlp " << semente << " " << ARQ_ELITE \
	            << " 2 " << min_sup << " " << ARQ_PADROES << " > " << OUTPUT_MD;
  
            break;       
        case EMD_HLS:
            // Aciona o algoritmo para extrair o maior padr�o de cada suporte
            comando << "./md/dci3_hls " << semente << " " << ARQ_ELITE \
	            << " 2 " << min_sup << " " << ARQ_PADROES << " > " << OUTPUT_MD;
  
            break;       
        case EMD_HnLP:
            // Aciona o algoritmo para extrair os n maiores padr�es de todos
            comando << "./md/dci3_hnlp " << semente << " " << ARQ_ELITE \
	            << " 2 " << min_sup << " " << ARQ_PADROES << " " << n << " > " << OUTPUT_MD;
  
            break;
        case EMD_HnMP:
            // Aciona o algoritmo para extrair os n maiores padr�es maximais
            comando << "./md/fpmax_hnmp " << semente << " 0 " << ARQ_ELITE << " " \
                    << tamanho << " " << min_sup << " " << n << " " << ARQ_PADROES << " > " << OUTPUT_MD;
  
            break;
    }

    system(comando.str().c_str());    
    
    char c;    
    
    FILE* arq_log_md = fopen(OUTPUT_MD, "r");
    do {
       c = getc(arq_log_md);
       printf("%c",c);
    } while(!feof(arq_log_md));
    printf("\b");
    
    fclose(arq_log_md);
      
    FILE* arq_padroes = fopen(ARQ_PADROES, "r"); // Abre arquivo onde foram escritos os padr�es
    vector<int> umPadrao;
    
    // Preenche a estrutura que guarda os padr�es
    do { 
        int item=0, pos=0;
        c = getc(arq_padroes);
        while((c >= '0') && (c <= '9')) {
            item *=10;
            item += int(c)-int('0');
            c = getc(arq_padroes);
            pos++;
        } 
        if(pos) umPadrao.push_back(item);
        if(c == '\n')  { 
	    sort(umPadrao.begin(), umPadrao.end());
	    padroes.push_back(umPadrao); 
            umPadrao.erase(umPadrao.begin(), umPadrao.end());
        }
    } while(!feof(arq_padroes));
    printf("\b");
    
    fclose(arq_padroes);

    // Imprime padr�es encontrados
    //printf("PADR�ES ENCONTRADOS: \n");
    //for(int p = 0; p < padroes.size(); p++) {
    //    for(int j = 0; j < padroes[p].size(); j++)
    //        printf("%d ", padroes[p][j]);
    //    printf("\n");
    //}
    //printf("\n");
    
    //printf("TOTAL DE PADR�ES ENCONTRADOS: %d\n\n", padroes.size());

    // Remove arquivo tempor�rio com o conjunto elite de solu��es
    //remove(ARQ_ELITE);
    
    // Remove arquivo tempor�rio com o padr�es
    remove(ARQ_PADROES);
    
    // Remove arquivo tempor�rio com output do programa de MD
    remove(OUTPUT_MD);
}
