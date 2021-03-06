#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <string>
#include <sstream>
#include "bibrand.c"

using namespace std;

#define ARQ_TMP "./md/ar_tmp"

int compare_int(const void *a, const void *b)
{
	const int *da = (const int *) a;
	const int *db = (const int *) b;
    
	return (*da > *db) - (*da < *db);
}

struct largest_pattern
{
	int *pattern;
	int size;
	int support;
	struct largest_pattern *next;
};
struct head_pattern
{
	int count;
	struct largest_pattern *first;
};
struct largest_pattern *new_pattern,*current_pattern,*head_pattern;
struct head_pattern *head;
struct largest_pattern *previous_pattern;
int nlargest_patterns;

void choose_largest(int size_candidate, int *candidate, int support_candidate, int seed) {
	int i;
	// int choice;
        
	previous_pattern = NULL;
	qsort (candidate, size_candidate, sizeof (int), compare_int);                                             
        
	// Check if it's the first pattern in list
	if (head->count == 0)
	{
		free(current_pattern->pattern);
		if (!(current_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
		{
			printf("\nError - malloc current_pattern->pattern (choose)\n");
			exit(1);
		}
		for (i = 0; i < size_candidate; i++)
		{
			current_pattern->pattern[i] = candidate[i];
		}
		current_pattern->size = size_candidate;
		current_pattern->support = support_candidate;
		head->count++;
	}
	else
	{
		current_pattern = head->first;
		// Check if its size is large enough to be in the list
		if (size_candidate >= current_pattern->size)
		{
			while ((size_candidate > current_pattern->size) && (current_pattern->next != NULL))
			{
				previous_pattern = current_pattern;
				current_pattern = current_pattern->next;
			}
			// Last position at the list
			if (size_candidate > current_pattern->size)
			{
				if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
				{
					printf("\nError - malloc new_pattern (choose)\n");
					exit(1);
				}
				if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
				{
					printf("\nError - malloc new_pattern->pattern (choose)\n");
					exit(1);
				}
				for (i = 0; i < size_candidate; i++)
				{
					new_pattern->pattern[i] = candidate[i];
				}
				new_pattern->size = size_candidate;
				new_pattern->support = support_candidate;
				new_pattern->next = NULL;
				current_pattern->next = new_pattern;
				head->count++;
				// check if the list is full
				if (head->count > nlargest_patterns)
				{
					current_pattern = head->first;
					head->first = head->first->next;
					free(current_pattern);
					current_pattern = NULL;
					head->count--;
				}
			}
			// A position in the middle of the list
			else 
			{
				// Before current position
				if (size_candidate < current_pattern->size)
				{
					if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
					{
						printf("\nError - malloc new_pattern (choose)\n");
						exit(1);
					}
					if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
					{
						printf("\nError - malloc new_pattern->pattern (choose)\n");
						exit(1);
					}
					for (i = 0; i < size_candidate; i++)
					{
						new_pattern->pattern[i] = candidate[i];
					}
					new_pattern->size = size_candidate;
					new_pattern->support = support_candidate;
					new_pattern->next = current_pattern;
					if (previous_pattern == NULL)
					{
						head->first = new_pattern;
					}
					else {
						previous_pattern->next = new_pattern;
					}
					head->count++;
					// check if the list is full
					if (head->count > nlargest_patterns)
					{
						current_pattern = head->first;
						head->first = head->first->next;
						free(current_pattern);
						current_pattern = NULL;
						head->count--;
					}
				}
				// A tie (concerning to size) has occured
				else
				{
					// Check the supports
					while ((support_candidate > current_pattern->support) && (size_candidate == current_pattern->size))
					{
						previous_pattern = current_pattern;
						current_pattern = current_pattern->next;
                                                if(current_pattern == NULL) break;
					}
					// Last position at the list
					if (current_pattern == NULL)
					{
						if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
						{
							printf("\nError - malloc new_pattern (choose)\n");
							exit(1);
						}
						if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
						{
							printf("\nError - malloc new_pattern->pattern (choose)\n");
							exit(1);
						}
						for (i = 0; i < size_candidate; i++)
						{
							new_pattern->pattern[i] = candidate[i];
						}
						new_pattern->size = size_candidate;
						new_pattern->support = support_candidate;
						new_pattern->next = NULL;
						previous_pattern->next = new_pattern;
						head->count++;
						// check if the list is full
						if (head->count > nlargest_patterns)
						{
							current_pattern = head->first;
							head->first = head->first->next;
							free(current_pattern);
							current_pattern = NULL;
							head->count--;
						}
					}
                                        else if ((support_candidate < current_pattern->support) || (size_candidate != current_pattern->size)) {
						// Before current position
						if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
						{
							printf("\nError - malloc new_pattern (choose)\n");
							exit(1);
						}
						if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
						{
							printf("\nError - malloc new_pattern->pattern (choose)\n");
							exit(1);
						}
						for (i = 0; i < size_candidate; i++)
						{
							new_pattern->pattern[i] = candidate[i];
						}
						new_pattern->size = size_candidate;
						new_pattern->support = support_candidate;
						new_pattern->next = current_pattern;
						if(previous_pattern)
                                                    previous_pattern->next = new_pattern;
                                                else
                                                    head->first = new_pattern;
						head->count++;
						// check if the list is full
						if (head->count > nlargest_patterns)
						{
							current_pattern = head->first;
							head->first = head->first->next;
							free(current_pattern);
							current_pattern = NULL;
							head->count--;
						}
					}
					else {
						// check if the pattern was already inserted
						while ((support_candidate == current_pattern->support && size_candidate == current_pattern->size))
						{
                                                        for (i = 0; i < size_candidate; i++)
								if (current_pattern->pattern[i] != candidate[i])
									break;
							
							// return if the pattern already exists
							if (i == size_candidate)
								return;
							
							previous_pattern = current_pattern;
							current_pattern = current_pattern->next;
                                                        if (current_pattern == NULL) break;
						}
						
						// insert before current position (the pattern was not already inserted)
						if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
						{
							printf("\nError - malloc new_pattern (choose)\n");
							exit(1);
						}
						if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
						{
							printf("\nError - malloc new_pattern->pattern (choose)\n");
							exit(1);
						}
						for (i = 0; i < size_candidate; i++)
						{
							new_pattern->pattern[i] = candidate[i];
						}
						new_pattern->size = size_candidate;
						new_pattern->support = support_candidate;
						new_pattern->next = current_pattern;
						                                                
						if (previous_pattern == NULL)
						{
							head->first = new_pattern;
						}
						else {
							previous_pattern->next = new_pattern;
						}
						head->count++;
						// check if the list is full
						if (head->count > nlargest_patterns)
						{
							current_pattern = head->first;
							head->first = head->first->next;
							free(current_pattern);
							current_pattern = NULL;
							head->count--;
						}
					}
				}
			}
		}
		// Otherwise, check if the list is not complete yet (it means that the candidate will be the element in the first position at the list)
		else if (head->count < nlargest_patterns)
		{
			if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
			{
				printf("\nError - malloc new_pattern (choose)\n");
				exit(1);
			}
			if (!(new_pattern->pattern = (int *) malloc(size_candidate*sizeof(int))))
			{
				printf("\nError - malloc new_pattern->pattern (choose)\n");
				exit(1);
			}
			for (i = 0; i < size_candidate; i++)
			{
				new_pattern->pattern[i] = candidate[i];
			}
			new_pattern->size = size_candidate;
			new_pattern->support = support_candidate;
			new_pattern->next = current_pattern;
			head->first = new_pattern;
			head->count++;
		}
	}
}

int main(int argc, char **argv)
{
	FILE *fpin, *fpout;
	// char *line = NULL;
	int size,supp,i;
	int *simple_pattern;
	
	if(argc != 8) {
		printf("Uso incorreto. Uso correto:\n");
		printf("fpmax_hnmp <semente> <id_arq_tmp> <banco de dados> <tam. do banco> <suporte minimo> <qtd de padroes> <arq. saida>\n");
		exit(1);
	}
	
	int seed = atoi(argv[1]);
	int min_sup = atoi(argv[5]);
	int tam_banco = atoi(argv[4]);
	
	nlargest_patterns = atoi(argv[6]);

	if ((fpout = fopen(argv[7], "w")) == NULL) {
		fprintf(stderr,"could not open %s\n",argv[7]);
		exit(1);
	}

	if (!(new_pattern = (struct largest_pattern *) malloc(sizeof(struct largest_pattern))))
	{
		printf("\nError - malloc new_pattern (main)\n");
		exit(1);
	}
	if (!(new_pattern->pattern = (int *) malloc(sizeof(int))))
	{
		printf("\nError - malloc new_pattern->pattern (main)\n");
		exit(1);
	}
	new_pattern->size = 0;
	new_pattern->support = 0;
	new_pattern->next = NULL;
	current_pattern = new_pattern;
	if (!(head = (struct head_pattern *) malloc(sizeof(struct head_pattern))))
	{
		printf("\nError - malloc head (main)\n");
		exit(1);
	}
	head->count = 0;
	head->first = new_pattern;
		 
        ostringstream buffer;
	
	for(int s = min_sup; s <= tam_banco; s++) {		
		
		 buffer.str("");
                 buffer << "./md/fim_maximal " << argv[3] << " " << s << " " << ARQ_TMP << argv[2] << ".txt";
                 //buffer << "./md/fim_maximal " << argv[3] << " " << min_sup << " " << ARQ_TMP << argv[2] << ".txt";		

		// printf("%s", buffer.str().c_str());
		
		system(buffer.str().c_str());
                
                buffer.str("");
                buffer << ARQ_TMP << argv[2] << ".txt";

		if ((fpin = fopen(buffer.str().c_str(), "r")) == NULL) {
			fprintf(stderr,"could not open %s\n", buffer.str().c_str());
			exit(1);
		}

		while (!feof(fpin)) {
                	size = -1;
			fscanf(fpin,"%d;",&size);
                        if(size == -1) break;
			fscanf(fpin,"%d;",&supp);
			printf("p\t%d\t",supp);
			if (!(simple_pattern = (int *) malloc(size*sizeof(int))))
			{
				printf("\nError - malloc simple_pattern (main)\n");
				exit(1);
			}
			for (i = 0; i < size; i++)
			{
				fscanf(fpin,"%d ",&simple_pattern[i]);
				printf("%d ",simple_pattern[i]);                                
			}
                        printf("\n");
                        choose_largest(size, simple_pattern, supp, seed);                        
			free(simple_pattern);
		}
		
		fclose(fpin);
		
	}
	
	// printf("\nPatterns found:\n");
	previous_pattern = head->first;
	do {
		current_pattern = previous_pattern->next;
		printf("pu\t%d\t",previous_pattern->support);
		for (i = 0; i < previous_pattern->size; i++)
		{
			printf("%d ",previous_pattern->pattern[i]);
			fprintf(fpout,"%d ",previous_pattern->pattern[i]);
		}
		fseek(fpout, -1, 1);		
		printf("\n");
		fprintf(fpout,"\n");
		free(previous_pattern);
		previous_pattern = current_pattern;
	} while (current_pattern != NULL);

	fclose(fpout);
	
	remove(buffer.str().c_str());
	
	return 0;
}
