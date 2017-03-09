#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
enum 
{
	space = 0,
	endofile = 1,
	slashN = 2,
	badslashN = 3
};
const char prompt []="CMON SHOW ME WHATCHA GOT!\n";



struct item 
{
	char *Arr;
	struct item *next;
};

int addlet (char** A,int c,int n,int k)
{
	int i;
	n++;
	if (n >= (k * 8)) 
	{
		char *B;
		B = malloc(++k*8*sizeof(char));
		for (i = 0;i < n-2;i++)
			B[i] = (*A)[i];
		B[n-2] = c;
		B[n-1] = '\0';
		free(*A);
		*A = B;
	} else
	{
		(*A)[n-2] = c;
		(*A)[n-1] = '\0';
	}
	return k;
}
int readword (char** A) 
{
	int c,n,k;
	k = n = 1;
	*A = malloc(8*k);
	(*A)[0] = '\0';
	while (((c = getchar()) == '\n') || (c == ' ') || (c == EOF))   
 /*Skipping garbage*/
	{ 
		if (c == '\n') 
		{
			free(*A);
			*A = NULL;
			return slashN;
		} else 
			if (c == EOF)
			{
				free(*A);
				return endofile;
			}
	}
	if (c != '"') /*Code becomes a shit load of copypaste, but CPU is happy*/
	{
		do
		{
			k = addlet(A,c,n,k);
			n++;
		} while (((c = getchar()) != EOF) && (c != '\n') && (c != ' '));
	} else
	{
		while (((c = getchar()) != '"') && (c != EOF) && (c != '\n'))
		{
			k = addlet(A,c,n,k);
			n++;
		}
		if (c == '\n') {
			return badslashN;
		}
	}
	if ((c == ' ')||(c == '"')) 
		return space;
	if (c == EOF) 
		return endofile;
	return slashN;
}

struct item* add (struct item *last, char *A) 
 /*Cuz i forgot how do parameters work here*/
{
	last->next = malloc(sizeof (struct item));
	last->Arr = A;	
	last->next->next = NULL;
	return last->next;
}

int release (struct item *p)
{
	free(p->Arr);
	free(p);
	return 0;
}

int writeln (char *p)
{
	int i;
	for (i=0;p[i] != '\0';i++)
		printf("%c",p[i]);
	printf("\n");
	return 0;
}

void clearsum(char **checksum)
{
	int i = 0;
	while (checksum[i] != NULL)
	{
		free(checksum[i]);
		i++;
	}
}

void grim (char **checksum)
{
	int p;
	if (checksum[0][0] == 'c' && 
		 checksum[0][1] == 'd' && 
		 checksum[0][2] == '\0')
	{
		if (checksum[2] == NULL)
		{
			p = chdir(checksum[1]);
			if (p == -1)
			{
				perror(checksum[1]);
			}
		} else
		{
			printf ("HOWTO: cd <parameterer>\n");
		}
	} else
	{
		p = fork();
		if (p == 0)
		{
			{
				execvp(checksum[0], checksum);
				perror(checksum[0]);
				exit(1);
			}
		} else
		{
		wait(NULL);
		} 
	}		
}

void holocaust2 (struct item *p, int err, int n)
{
	char* checksum [n+1];
	struct item *first;
	int i = 0;
	if (n != 0)
	{
		if (err == badslashN) 
		{
			printf("U R WRONG (not right)\n");
		}
		while (p->next != NULL)
		{
			if (err == slashN)	
			{
				checksum[i] = p->Arr;
				i++;
				first = p;
				p = p->next;
				free(first);
			} else
			{
				first = p;
				p = p->next;
				release(first);
			}
		}
		checksum[n] = NULL;
		if (err == slashN)
		{
			grim(checksum);
			clearsum(checksum);
		}
	}
	free(p);
}

void holocaust (struct item *p, int n)
{
	char* checksum[n+1];
	struct item *first;
	int i = 0;
	if (n != 0)
	{
		while (p->next != NULL)
		{
			checksum[i] = p->Arr;
			i++;
			first = p;
			p = p->next;
			free(first);
		}
		grim(checksum);
		clearsum(checksum);
	}
	free(p);
}

int main ()
{
	struct item *first, *last, *p;
	char *Arr;
	int err;
	int n = 0;
	p = first = last = malloc (sizeof (struct item));
	p->next = NULL;
	printf("%s",prompt);
	do
	{
		err = readword(&Arr);
		if (err == endofile) 
		{
			holocaust(p,n);
			printf("Reached end of file.\n");
			return 0;
		}
		if (Arr != NULL)
		{
			last = add(last,Arr);
			n++;
		}
		if ((err == slashN)||(err == badslashN)) 
		{
			holocaust2(p,err,n);
			n = 0;
			p = first = last = malloc(sizeof (struct item));
			p->next = NULL;
			printf("%s",prompt);
		}
	} while (1);			
	printf("Reached end of file.\n");
	return 0;
}
