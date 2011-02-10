
#ifndef __main_h__
#define __main_h__

#include <stdio.h>
#include <string.h>

#ifndef CLK_TCK
#define CLK_TCK 100
#endif

void app_error (char *msg, int halt);

#endif
