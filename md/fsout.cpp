#include "fsout.h"
#include "common.h"

FSout::FSout(char *filename)
{
  out = fopen(filename,"wt");
}

FSout::~FSout()
{
  if(out) fclose(out);
}

int FSout::isOpen()
{
  if(out) return 1;
  else return 0;
}

void FSout::printSet(int length, int *iset, int support)
{
//#ifdef shown 
  // MARCOS
  fprintf(out, "%d;%d;",length,support);
  // FIM MARCOS
  for(int i=0; i<length; i++) 
  {
	fprintf(out, "%d ", order_item[iset[i]]);
//	printf("%d ", order_item[iset[i]]);
  }
  // MARCOS
  //fprintf(out, "(%d)\n", support);
  fprintf(out,"\n");
  // FIM MARCOS
//  printf("(%d)\n", support);
//#endif
}

void FSout::printset(int length, int *iset)
{
//#ifdef shown 
  for(int i=0; i<length; i++) 
    fprintf(out, "%d ", order_item[iset[i]]);
//#endif
}

void FSout::close()
{
	fclose(out);
}

