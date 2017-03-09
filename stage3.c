#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum {
	space = 0,
	slashN = 1,
	badslashN = 2,
	endofile = 3,
	badEOF = 4,
	ampers = 5
};

const char greet []="CMON SHOW ME WHATCHA GOT!\n>";
const char end []="\nReached end of file.\n";
const char bedend []="\nReached bad end of file.\n";
const char badslashn []=
	"Unbalanced QUOTES CAN YOU EVEN TYPE LIKE A NORMAL PERSON HUH?!?!?!"
	"\n\nOK, NOW ";
const char wrongcd []="Cant execute: too many arguments after \"cd\"\n";
const char firstamp []="Syntax error near unexpected token '&'\n";

struct list 
{
	char *Arr;
	struct list *next;
};

int addlet (char **A, int c, int n, int k)
{
	int i;
	n++;
	if (n >= (k*8))
	{
		char *B;
		B = malloc((++k*8*sizeof(char)));
		for (i = 0;i < n-2; i++)
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
	int c,n=1,k=1/*,err*/;
	*A = malloc(8*k);
	(*A)[0] = '\0';
	while ((c = getchar()) == '\n' || c == ' ' || c == EOF || c == '&')
/*Just to skip some garbage*/
	{
		if (c != ' ')
		{
			free(*A);
			*A = NULL;
			if (c == '\n')
				return slashN;
			if (c == EOF)
				return endofile;
			if (c == '&')
				return ampers;
		}
	}
	if (c == '"')
	{
		while ((c = getchar()) != '"' && (c != EOF) && (c != '\n'))
			k = addlet(A,c,n++,k);
		if (c == '\n')
			return badslashN;
		if (c == EOF)
			return badEOF;
	} else
	{
		do
		{
			k = addlet(A,c,n++,k);
		} while ((c = getchar()) != EOF &&
					 c != '\n' &&
					 c != ' ' &&
					 c != '&');
	}
	if (c == ' ' || c == '"')
		return space;
	if (c == EOF)
		return endofile;
	if (c == '&')
		return ampers;
	return slashN;
}

void add (char *A, struct list **last)
{
	(*last)->next = malloc (sizeof(struct list));
	(*last)->Arr = A;
	*last = (*last)->next;
	(*last)->next = NULL;
	/*DA NAHT RETARN ANETHENGHE!*/
}

void holocaust(struct list *p,int n)
{
	struct list *q;
	while (p->next != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	free (p);
}

void grim(char **checksum, int err)
{
	int p;
	if (checksum[0][0] == 'c' &&
		 checksum[0][1] == 'd' &&
		 checksum[0][2] == '\0')
	{
		if (checksum[2] == NULL)
		{
			if ((p = chdir(checksum[1])) == -1)
				perror(checksum[1]);	
		} else
			printf("%s",wrongcd);
	} else
		if ((p = fork()) == 0)
		{
			while (wait4(-1,NULL,WNOHANG,NULL) > 0)
				{}
			execvp(checksum[0],checksum);
			perror(checksum[0]);
			exit(1);
		} else
			if (err != ampers)
				while (wait(NULL) != p)
					{}
/*		usleep(10000);*/
}

void clearsum (char **checksum, int n)
{
	int i;
	for (i = 0; i <= n ; i++)
		free(checksum[i]);
}

void flush (struct list *p, int err, int n)
{
	char* checksum [n+1];
	struct list *q;
	int i = 0;
	if (n != 0)
	{
		while (p->next != NULL)
		{
			checksum[i++] = p->Arr;
			q = p;
			p = p->next;
			free(q);
		}
		checksum[n] = NULL;
		grim(checksum,err);
		clearsum(checksum,n);
	}
	if (err == ampers && n == 0)
		printf("%s",firstamp);
	free(p);
}

int main()
{
	struct list *first, *last, *p;
	char *Arr;
	int err, n = 0;
	p = first = last = malloc (sizeof (struct list));
	p->next = NULL;
	printf ("%s", greet);
	do
	{
		err = readword(&Arr);
		if (err == endofile || err == badEOF)
		{
			holocaust(first,n);
			if (err == endofile)
			{
				printf("%s",end);
			} else
				printf("%s",bedend);
			while (wait4(-1,NULL,WNOHANG,NULL) != -1)
				{}
			return 0;
		}
		if (err == badslashN)
		{
			holocaust(first,n);
			printf("%s",badslashn);
			n = 0;
			p = first = last = malloc(sizeof(struct list));
			p->next = NULL;
			while (wait4(-1,NULL,WNOHANG,NULL) > 0)
				{}
			printf("%s",greet);
		} else
		{
			if (Arr != NULL)
			{
				add(Arr,&last);
				n++;
			}
			if (err != space)
			{
				flush(first, err, n);
				p = first = last = malloc(sizeof(struct list));
				p->next = NULL;
				while (wait4(-1,NULL,WNOHANG,NULL) > 0)
					{}
				n = 0;
				if (err == slashN)
					printf("%s",greet);
			}
		}
	} while(1048576);
}
