#include <iostream>
#include <fstream>
#include "grasp.h"



GRASP::GRASP (int max_iter, int sem, int pct, int te, ESTRATEGIA_MD estrat, int sup, int n) :
  melhor_sol(NULL), iter_cor(0), tam_elite(te), semente(sem), min_sup(sup), npadroes(n),
  est(estrat), iter_max_total(max_iter)
{
  iter_pool_igual = (pct * max_iter) / 100;
  iter_max = max_iter;
}

GRASP::~GRASP ()
{
}

int GRASP::Iter ()
{
  return iter_cor;
}

int GRASP::IterMax ()
{
  return iter_max_total;
}

PSOLUCAO GRASP::Solucao ()
{
  return melhor_sol;
}

PSOLUCAO GRASP::Executa ()
{
  PSOLUCAO    sol;	/** Armazena temporariamente cada solucao gerada*/
  int         qtd_bl;	/** quantidade de passos executados na busca local p/ cada solucao gerada*/
  struct tms  ti, tf;	/** Utilizadas para medir a duracao de etapas do processo*/
  double       t; 	/** Tempo que determinado processo levou para executar */
  bool pool_variou; 	/** indica se o pool recebeu um novo elemento em cada iteracao*/
  bool pool_variou_apos_MD = false;	/** indica se o pool sofreu alguma alteracao apos a ultima mineracao */
  Pool* pool = NULL; 	/** Armazena as tam_elite melhores solucoes encontradas*/
  vector< vector<int> > padroes; /** padroes minerados*/
  int padrao_atual = 0;	/** o numero do padrao utilizado na construcao em cada iteracao */
  int qtd_padroes = 0;	/** quantidade de padroes encontrados na mineracao */
  int pool_igual = 0;	/** numero de iteracoes sem que o pool recebesse novas solucoes */


  pool = new Pool(tam_elite);
  if (pool == NULL)  app_error("Impossivel instanciar o pool", 8);
  melhor_sol = NULL;

  Inicializacao ();


  for (iter_cor = 1; iter_cor <= iter_max; iter_cor++) {


     if ( (pool_variou_apos_MD) && (pool_igual >= iter_pool_igual) ) {  //entao minera

	// Extra��o de padr�es
	padrao_atual = 0;
  	qtd_padroes = 0;
	padroes.clear();


//    	times(&ti);
	pool->ReconhecePadroes(padroes, est, min_sup, npadroes, semente);
//	times(&tf); 
//	t =  ((float)(tf.tms_utime - ti.tms_utime) /(float)(CLK_TCK));

//	cout << "tempo_md\t" << t << endl;

	qtd_padroes = padroes.size();


	for(;padrao_atual < qtd_padroes; padrao_atual++) {
	     cout << "\npa\t" << padrao_atual << "\t";
   	     for(unsigned int item = 0; item < padroes[padrao_atual].size(); item++)
	    	cout << padroes[padrao_atual][item] << " ";
  	}

	padrao_atual = 0;
	cout << endl;

	//zera contador de iteracoes sem atualizacao no pool
	pool_igual = 0;
	pool_variou_apos_MD = false;
     }


    cout << iter_cor << "\t";

//    AntesIteracao    (iter_cor);
//    AntesConstrucao  (iter_cor);


    times(&ti);
    if (! padroes.empty())
	    sol = Construcao (iter_cor, &(padroes[padrao_atual]));
    else
	    sol = Construcao (iter_cor, NULL);

    times(&tf);
    t =  ((double)((tf.tms_utime - ti.tms_utime)) / sysconf(_SC_CLK_TCK));

  //  DepoisConstrucao (iter_cor,sol);


    /**
      * Verifica se a solu��o j� foi constru�da. Caso positivo, parte para a cria��o
      * de outra. Caso negativo, adiciona ao conjunto de solu��es criadas
      */
 //   ParHash *solHash = &(sol->Hash());
 //   set<ParHash>::iterator it;
 //   it = construidas.find(*solHash);
 //   if (it == construidas.end() ) //caso nao conste no conjunto de criadas, insere no mesmo
 //   {	
 //       construidas.insert(*solHash);
 //   }
 //   else
 //   {	//se for repetida
 //   	cout << "FILTRO: Solucao repetida" << endl;
 //       pool_igual++;

//	if (!padroes.empty()) { // passa para o proximo padrao
//		padrao_atual++;
//		padrao_atual = padrao_atual % qtd_padroes;
//	}	
//        delete solHash;
//        delete sol;
//        continue;
//    }
 //   delete solHash;


    if (! padroes.empty())
	    cout << padrao_atual << "\t" << sol->Valor() << "\t" << t << "\t";
    else
    	    cout << sol->Valor() << "\t" << t << "\t";


//    AntesBuscaLocal  (iter_cor,sol);

    qtd_bl = 0;
    times(&ti);
//    sol = BuscaLocal (sol,qtd_bl);
    sol = BuscaLocalQueRemoveElementos (sol,qtd_bl,1);
    times(&tf);
    t =  ((double)((tf.tms_utime - ti.tms_utime)) / sysconf(_SC_CLK_TCK));

//    DepoisBuscaLocal (iter_cor,sol);

    if (! padroes.empty())
    	cout << qtd_bl << "\t" << sol->Valor() << "\t" << t << "\t";
    else
    	cout << sol->Valor() << "\t" << qtd_bl << "\t" << t << "\t";


    //atualiza pool
    pool_variou = pool->Inserir(sol->tovector(), sol->Valor());

    //atualiza o numero de iteracoes sem atualizacoes no pool
    //e registra se houve alteracao apos a ultima mineracao
    if (!pool_variou) {
	pool_igual++;
    }
    else {
	pool_igual = 0;
	pool_variou_apos_MD = true;
    }


    if (melhor_sol != NULL) {
      if (sol->Valor () < melhor_sol->Valor ()) {
        delete melhor_sol;
        melhor_sol = sol;
      }
      else
        delete sol;
    }
    else
      melhor_sol = sol;


    if (!padroes.empty()) {
	padrao_atual++;
	padrao_atual = padrao_atual % qtd_padroes;
    }


//    DepoisIteracao  (iter_cor,melhor_sol);

    cout << endl;

  }


//  Finalizacao (melhor_sol);

  return melhor_sol;
}


PSOLUCAO GRASP::Exec (PCOMPTIME tempo)
{
  PSOLUCAO   sol;
  struct tms ti,tf;
  double t;

  sol = NULL;

  if (tempo != NULL)
    comptime_start (tempo);

  times(&ti);
  sol = Executa ();
  times(&tf);
  t =  (((double) (tf.tms_utime - ti.tms_utime)) / sysconf(_SC_CLK_TCK)) + (((double) (tf.tms_cutime - ti.tms_cutime)) / sysconf(_SC_CLK_TCK));

  cout << "TEMPO\t" << t << "\n SOLUCAO: ";
  sol->Exibe();
  if (tempo != NULL)
      comptime_update (tempo);

  return sol;
}
