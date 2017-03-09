#include <stdio.h>
#include <stdlib.h>
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
		B = malloc (++k*8*sizeof(char));
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
			free (*A);
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
			k = addlet (A,c,n,k);
			n++;
		} while (((c = getchar()) != EOF) && (c != '\n') && (c != ' '));
	} else
	{
		while (((c = getchar()) != '"') && (c != EOF) && (c != '\n'))
		{
			k = addlet (A,c,n,k);
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
	last->next = malloc (sizeof (struct item));
	last->Arr = A;	
	last->next->next = NULL;
	return last->next;
}

int release (struct item *p)
{
	free (p->Arr);
	free (p);
	return 0;
}

int writeln (char *p)
{
	int i;
	for (i=0;p[i] != '\0';i++)
		printf ("%c",p[i]);
	printf ("\n");
	return 0;
}

void holocaust2 (struct item *p, int err)
{
	struct item *first;
	if (err == badslashN) 
	{
		printf ("U R WRONG (not right)\n");
	}
	while (p->next != NULL)
	{
		if (err == slashN)	
		{
			writeln (p->Arr);
		}
		first = p;
		p = p->next;
		release (first);
	}
	free (p);
}

void holocaust (struct item *p)
{
	struct item *first;
	while (p->next != NULL)
	{
		writeln (p->Arr);
		first = p;
		p = p->next;
		release (first);
	}
	free (p);
}

int main ()
{
	struct item *first, *last, *p;
	char *Arr;
	int err;
	p = first = last = malloc (sizeof (struct item));
	p->next = NULL;
	printf ("%s",prompt);
	do
	{
		err = readword (&Arr);
		if (err == endofile) 
		{
			holocaust (p);
			printf ("Reached end of file.\n");
			return space;
		}
		if (Arr != NULL)
		{
			last = add(last,Arr);
		}
		if ((err == slashN)||(err == badslashN)) 
		{
			holocaust2 (p,err);
			p = first = last = malloc (sizeof (struct item));
			p->next = NULL;
			printf ("%s",prompt);
		}
	} while (1);			
	printf ("Reached end of file.\n");
	return 0;
}
