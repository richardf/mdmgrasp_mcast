
/************************************************************************************
 * Arquivo     : clock.h                                                            *
 * Dependencias: types.h                                                            *
 * Autor       : Eyder Rios (erios@ic.uff.br)                                       *
 * Data        : 17/04/2002                                                         *
 * Descricao   : Implementa rotinas para contagem de tempo, data e hora.            *
 ************************************************************************************/

#ifndef __clock_h__
#define __clock_h__

#include <time.h>
#include "types.h"

typedef struct tagDATETIME {
          UINT  dt_year;        // ano: com 4 digitos
          UINT  dt_month;       // mes: 1-12
          UINT  dt_day;         // dia: 1-31
          UINT  dt_wday;        // dia da semana: 0-6 (0=domingo - 6=sabado)
          UINT  dt_yday;        // dia do ano: 1-365
          UINT  dt_hour;        // hora: 0-23
          UINT  dt_min;         // minuto: 0-59
          UINT  dt_sec;         // segundo: 0-59
        } DATETIME, *PDATETIME;

// identificador do contador de tempo
typedef ULONG   TIMER, *PTIMER;

typedef struct tagCOMPTIME {
          TIMER   t_timer;
          ULONG   t_min;
          ULONG   t_max;
          UINT    t_upt;
          REAL    t_sum;
          REAL    t_avg;
        } COMPTIME, *PCOMPTIME;

ULONG  timer_tick  ();
ULONG  timer_wait  (ULONG time);

ULONG  timer_start    (PTIMER timer);
ULONG  timer_enlapsed (PTIMER timer);

void   clock_datetime (PDATETIME dt);
void   clock_fdate    (PDATETIME dt, char *str_date);
void   clock_ftime    (PDATETIME dt, char *str_time);
void   clock_fdatetime(PDATETIME dt, char *str);

void   comptime_start  (PCOMPTIME ctime);
void   comptime_update (PCOMPTIME ctime);

#endif
