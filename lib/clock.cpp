
/************************************************************************************
 * Arquivo     : clock.cpp                                                          *
 * Dependencias: clock.h, types.h                                                   *
 * Autor       : Eyder Rios (erios@ic.uff.br)                                       *
 * Data        : 17/04/2002                                                         *
 * Descricao   : Implementa rotinas para contagem de tempo, data e hora.            *
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "clock.h"


/**************************************************************************************
 *                                   CONSIDERACOES                                    *
 * A contagem de tempo em sistemas multitarefa, pode  dar  a  impressao, a principio, *
 * de nao funcionar corretamente. Quando, por exemplo, mandamos o programa ficar pa-  *
 * rado por 10 segundos, o tempo real de sistema em que ele fica neste  estado  pode  *
 * ser maior. Temos que considerar que o sistema  operacional  executa   centenas  de *
 * outras tarefas. Contudo, a contagem de tempo e' feita corretamente, pois e' consi- *
 * derado somente o tempo em que o sistema  ficou executado a tarefa que solicitou  a *
 * parada.                                                                            *
 *************************************************************************************/

/***********************************************************************************
 *                                                                                 *
 *                          VARIAVEIS GLOBAIS INTERNAS                             *
 *                                                                                 *
 ***********************************************************************************/

// ultimo instante em que timer_start () foi chamado s/ parametro
//PRIVATE ULONG  timer_ref = 0;


/***********************************************************************************
 * Funcao    : ULONG timer_tick ()                                                 *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Obtem o instante atual do relogio interno do sistema.               *
 * Parametros: Nenhum                                                              *
 * Retorno   : Retorna o instante atual do relogio em milisegundos.                *
 ***********************************************************************************/

PUBLIC ULONG timer_tick ()
{
#ifdef __MSDOS__
  // em abiente DOS nao exite a constante CLOCKS_PER_SEC
  return ((ULONG) clock () * 1000.0 / (ULONG) CLK_TCK);
#else
  // converte para double para nao ocorrer overflow na multiplicacao
  return (ULONG) (((double) clock () * 1000.0 / (double) CLOCKS_PER_SEC));
#endif
}

/***********************************************************************************
 * Funcao    : ULONG  timer_wait  (ULONG time)                                     *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Suspende a exewcucao do programa por um intervalo de 'time' mili-   *
 *             segundos.                                                           *
 * Parametros: time  - quantidade de milisegundos a esperar                        *
 * Retorno   : Retorna o tempo esperado.                                           *
 ***********************************************************************************/

PUBLIC ULONG  timer_wait (ULONG time)
{
  ULONG end_tick;
  
  end_tick = timer_tick () + time;

  while (timer_tick () < end_tick);

  return time;
}

/***********************************************************************************
 * Funcao    : ULONG timer_start (PTIMER timer)                                    *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Inicializa a funcao de contagem de tempo para o  contador  descrito *
 *             por 'timer'. Utilizada quando se quer medir o tempo de execucao  de *
 *             alguma parte do programa. Esta funcao  trabalha   em  conjunto  com *
 *             timer_enlapsed ().                                                  *
 *             Observe que a funcao pode trabalhar com varios contadores  de tempo *
 *             diferentes, bastando para isso utilizar descritores  (timer)  dife- *
 *             rentes. Se o valor de 'timer' e' NULL, e' considerado o instante em *
 *             que o programa iniciou sua execucao.                                *
 * Parametros: cinfo   - descritor de contador de tempo.                           *
 * Retorno   : Retorna o instante de tempo em ms em que o contador foi inicializa- *
 *             do.                                                                 *
 ***********************************************************************************/

PUBLIC ULONG timer_start (PTIMER timer)
{
  ULONG tick;

  tick = timer_tick ();
  if (assigned (timer))
    *timer = tick;

  return tick;
}

/***********************************************************************************
 * Funcao    : ULONG timer_enlapsed (PTIMER timer)                                 *
 * Criacao   : 17/04/2002                                                          *
 * Descricao : Obtem o tempo decorrido em segundos desde a ultima chamada da funcao*
 *             timer_start () para o contador de tempo especificado por 'timer'.   *
 *             Se 'timer' for NULL, a funcao retorna o tempo decorrido desde o  i- *
 *             nicio da execucao do programa. Observe que  neste caso  o  contador *
 *            pode 'virar' varias vezes.                                           *
 * Parametros: timer  - manipulador do contador de tempo                           *
 * Retorno   : Retorna o tempo decorrido em milisegundos para o contador 'timer'.  *
 ***********************************************************************************/

PUBLIC ULONG timer_enlapsed (PTIMER timer)
{
  ULONG  time;

  if (assigned (timer))
    time = timer_tick () - *timer;
  else
    time = timer_tick ();

  return time;
}

/***********************************************************************************
 * Funcao    : void clock_datetime (PDATETIME dt)                                  *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Obtem a data e hora local do sistema e armazena em 'dt'             *
 * Parametros: dt   - estrutura que recebera' a data e hora atuais.                *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

PUBLIC void clock_datetime (PDATETIME dt)
{
  struct tm *tm;
  time_t     timer;

  timer = time (NULL);
  tm = localtime (&timer);

  dt->dt_year  = tm->tm_year + 1900;
  dt->dt_month = tm->tm_mon + 1;
  dt->dt_day   = tm->tm_mday;
  dt->dt_wday  = tm->tm_wday;
  dt->dt_yday  = tm->tm_yday;
  dt->dt_hour  = tm->tm_hour;
  dt->dt_min   = tm->tm_min;
  dt->dt_sec   = tm->tm_sec;
}

/***********************************************************************************
 * Funcao    : void clock_fdate (PDATETIME dt, char *str_date)                     *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Formata a data especificada por 'dt' para o formato 'dd/mm/yyyy'.   *
 *             Caso o valor de 'dt' seja NULL, sera utilizada a data atual.        *
 * Parametros: dt       - estrutura que informa a data a ser formatada             *
 *             str_date - recebe a string formatada                                *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

PUBLIC void clock_fdate (PDATETIME dt, char *str_date)
{
  DATETIME temp;

  if (isnull (dt)) {
    clock_datetime (&temp);
    dt = &temp;
  }

  sprintf (str_date,"%02d/%02d/%04d",(int) dt->dt_day,(int) dt->dt_month,(int) dt->dt_year);
}

/***********************************************************************************
 * Funcao    : void clock_ftime (PDATETIME dt, char *str_time)                     *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Formata a hora especificada por 'dt' para o formato 'hh:mm:ss'.     *
 *             Caso o valor de 'dt' seja NULL, sera utilizada a hora atual.        *
 * Parametros: dt       - estrutura que informa a hora a ser formatada             *
 *             str_date - recebe a string formatada                                *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

PUBLIC void clock_ftime (PDATETIME dt, char *str_time)
{
  DATETIME temp;

  if (isnull (dt)) {
    clock_datetime (&temp);
    dt = &temp;
  }

  sprintf (str_time,"%02d:%02d:%02d",(int) dt->dt_hour,(int) dt->dt_min,(int) dt->dt_sec);
}

/***********************************************************************************
 * Funcao    : void clock_fdatetime (PDATETIME dt, char *str)                      *
 * Criacao   : 17/04/2002                                                          *
 * Alteracao : 17/04/2002                                                          *
 * Descricao : Formata a data e hora especificada por 'dt' para o formato          *
 *             'dd/mm/yyyy hh:mm:ss'. Caso o valor de 'dt' seja NULL, sera utili-  *
 *              zada a date e hora atual.                                          *
 * Parametros: dt   - estrutura que informa a data/hora a ser formatada            *
 *             str  - recebe a string formatada                                    *
 * Retorno   : Nenhum                                                              *
 ***********************************************************************************/

void clock_fdatetime (PDATETIME dt, char *str)
{
  char str_date[16],str_time[16];

  clock_fdate (dt,str_date);
  clock_ftime (dt,str_time);

  strcpy (str,str_date);
  strcat (str," ");
  strcat (str,str_time);
}

void comptime_start (PCOMPTIME ctime)
{
  ctime->t_min = MAXULONG;
  ctime->t_max = MINULONG;
  ctime->t_upt = 0;
  ctime->t_sum = 0.0;
  ctime->t_avg = 0.0;

  timer_start (&ctime->t_timer);
}

void comptime_update (PCOMPTIME ctime)
{
  ULONG  time;

  time = timer_enlapsed (&ctime->t_timer);

  if (time > ctime->t_max)
    ctime->t_max = time;

  if (time < ctime->t_min)
    ctime->t_min = time;

  ctime->t_upt++;

  ctime->t_sum += time;

  ctime->t_avg = ctime->t_sum / ctime->t_upt;

  timer_start (&ctime->t_timer);
}


