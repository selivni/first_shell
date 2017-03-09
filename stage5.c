#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
enum {
	space = 0,
	slashN = 1,
	badslashN = 2,
	endofile = 3,
	badEOF = 4,
	ampers = 5,
	argsafterpath = 6,
	doublepath = 7,
	nofilename = 8,
	convo = 9
};

const char greet []="CMON SHOW ME WHATCHA GOT!\n>";
const char end []="\nReached end of file.\n";
const char bedend []="\nReached bad end of file.\n";
const char badslashn []=
	"Unbalanced QUOTES CAN YOU EVEN TYPE LIKE A NORMAL PERSON HUH?!?!?!"
	"\n\nOK, NOW ";
const char wrongcd []="Cant execute: too many arguments after \"cd\"\n";
const char firstamp []="Syntax error near unexpected token '&'\n";
const char argafterpath []="No arguments after filepathes allowed\n";
const char noname []="Error: filepath expected\n";
const char twopaths []="No double input/output allowed\n";
const char noconvo []="That's not how you convo, bruh\n";
const char emptyconvo []="Empty start or end of convo\n";

struct list 
{
	char *Arr;
	struct list *next;
};

char* filepaths[3];
int fileflague;
int fd[3];

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

char* getpath (int *c)
{
	char *A;
	int k=1,n=1;
	A = malloc (8*k);
	do
	{
		k = addlet (&A,*c,n++,k);
	} while ((*c = getchar()) != '\n' &&
				 *c != EOF &&
				 *c != '&' &&
				 *c != ' ');
	return A;
}

void skripperino (int *c)
{
	while (((*c) = getchar()) == ' ')
		{}
}

void readln()
{
	int c;
	while ((c = getchar()) != '&' && c != '\n' && c != EOF)
		{}
	/*return nothing or stolyar will rek ur ass*/
}

int readpath (int c)
{
	char* A;
	int k;
	if (c == '>')
	{
		if (filepaths[1] != NULL || filepaths[2] != NULL)
		{
			readln();
			return doublepath;
		}
		if ((c = getchar()) == '>')
		{
			k = 2;
			skripperino(&c);
		} else
		{
			k = 1;
			if (c == ' ')
				skripperino(&c);
		}
	} else
	{
		if (filepaths[0] != NULL)
		{
			readln();
			return doublepath;
		}
		k = 0;
		skripperino (&c);
	}
	if (c != '\n' && c != EOF && c != '&' && c != '|')
	{
		A = getpath(&c);
		filepaths[k] = A;
	} else
	{
		readln();
		return nofilename;
	}
	if (c == '\n')
		return slashN;
	if (c == EOF)
		return endofile;
	if (c == '&')
		return ampers;
	if (c == '|')
		return convo;
	return space;
}

int readword (char** A)
{
	int c,n=1,k=1,err;
	*A = malloc(8*k);
	(*A)[0] = '\0';
	while ((c = getchar()) == '\n' || 
			  c == ' ' || 
			  c == EOF || 
			  c == '&' || 
			  c == '|')
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
			if (c == '|')
				return convo;
		}
	}
	if (c == '>' || c == '<')
	{
		fileflague = 1;
		err = readpath(c);
		free (*A);
		*A = NULL;
		return err;
	}
	if (fileflague == 1)
	{
		free(*A);
		*A = NULL;
		readln();
		return argsafterpath;
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
					 c != '&' &&
					 c != '|');
	}
	if (c == ' ' || c == '"')
		return space;
	if (c == EOF)
		return endofile;
	if (c == '&')
		return ampers;
	if (c == '|')
		return convo;
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

void clearpaths()
{
	int i;
	for (i = 0; i<=2; i++)
		if (filepaths[i] != NULL)
		{
			free(filepaths[i]);
			filepaths[i] = NULL;
		}
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
	clearpaths();
}

int openfiles()
{
	if (filepaths[0] != NULL)
		if ((fd[0] = open(filepaths[0],O_RDONLY)) == -1)
		{
			perror(filepaths[0]);
			return -1;
		}
	if (filepaths[1] != NULL)
		if ((fd[1] = open(filepaths[1],O_WRONLY|O_CREAT|O_TRUNC,0751)) == -1)
		{
			perror(filepaths[1]);
			goto F;
		}
	if (filepaths[2] != NULL)
		if ((fd[2] = open(filepaths[2],O_WRONLY|O_APPEND)) == -1)
		{
			perror(filepaths[2]);
			goto F;
		}
	return 0;
F: if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	return -1;
}

void turndastream()
{
	if (fd[0] != -1)
	{
		dup2(fd[0],0);
		close(fd[0]);
	}
	if (fd[1] != -1)
	{
		dup2(fd[1],1);
		close(fd[1]);
	}
	if (fd[2] != -1)
	{
		dup2(fd[2],1);
		close(fd[2]);
	}
}

void closefd()
{
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	if (fd[2] != -1)
		close(fd[2]);
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
	{
		if (openfiles() == -1)
			return;
		if ((p = fork()) == 0)
		{
			turndastream();
			while (wait4(-1,NULL,WNOHANG,NULL) > 0)
				{}
			execvp(checksum[0],checksum);
			perror(checksum[0]);
			exit(1);
		} else
			if (err != ampers)
				while (wait(NULL) != p)
					{}
	}
/*		usleep(10000);*/
	closefd();
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
	clearpaths();
}

char** listtoarray (struct list *p,int wordcount)
{
	char** checksum;
	struct list *q;
	int i = 0;
	if (wordcount != 0)
	{
		checksum = malloc((wordcount+1) * sizeof(char*));
		while (p->next != NULL)
		{
			checksum[i++] = p->Arr;
			q = p;
			p = p->next;
			free(q);
		}
		checksum[wordcount] = NULL;
	free(p);
	return checksum;
	}
	free(p);
	return NULL;
}

void muralloc(char**** checksums, int i)
{
	char*** newsums;
	newsums = malloc(i*sizeof(char**));
	newsums[--i] = NULL;
	while (--i>=0)
		newsums[i] = (*checksums)[i];
	free(*checksums);
	*checksums = newsums;
}

void nosums(char*** checksums, int n)
{
	int i,j;
	for (i = 0; i < n; i++)
		if (checksums[i] != NULL)
		{
			j=0;
			while (checksums[i][j] != NULL)
				free(checksums[i][j++]);
			free(checksums[i]);
		}
	free(checksums);
}

void waitforeveryone(int* pids, int i)
{
	int pid=0,j,k;
	k = i+1;
	while(k > 0)
	{
		pid = wait(NULL);
		for (j = 0; j <= i; j++)
			if (pids[j] == pid)
			{
				pids[j] = 0;
				k--;
			}
	}
}

void runconvo (char*** checksums, int i, int err)
{
	int j=0, filein = -1, fileout = -1, piped1[2],piped2[2],pids[i];
	i--;
	if (openfiles() == -1)
		return;
	if (fd[0] != -1)
		filein = fd[0];
	if (fd[1] != -1)
		fileout = fd[1];
	if (fd[2] != -1)
		fileout = fd[2];
	pipe(piped1);
	if ((pids[j] = fork()) == 0)
	{
		if (filein != -1)
		{
			dup2(filein,0);
			close(filein);
		}
		dup2(piped1[1],1);
		close(piped1[1]);
		close(piped1[0]);
		while (wait4(-1,NULL,WNOHANG,NULL) > 0)
			{}
		execvp(checksums[0][0],checksums[0]);
		perror(checksums[0][0]);
		exit(1);
	}
	close(piped1[1]);
	if (filein != -1)
		close(filein);
	while (++j < i)
	{
		pipe(piped2);
		if ((pids[j] = fork()) == 0)
		{
			dup2(piped1[0],0);
			close(piped1[0]);
			dup2(piped2[1],1);
			close(piped2[1]);
			close(piped2[0]);
			while (wait4(-1,NULL,WNOHANG,NULL) >0)
				{}
			execvp(checksums[j][0],checksums[j]);
			perror(checksums[j][0]);
			exit(1);
		}
		close(piped1[0]);
		close(piped2[1]);
		piped1[0] = piped2[0];
	}
	if ((pids[j] = fork()) == 0)
	{
		dup2(piped1[0],0);
		close(piped1[0]);
		if (fileout != -1)
		{
			dup2(fileout,1);
			close(fileout);
		}
		while (wait4(-1,NULL,WNOHANG,NULL) >0)
			{}
		execvp(checksums[j][0],checksums[j]);
		perror(checksums[j][0]);
		exit(1);
	}
	close(piped1[0]);
	close(fileout);
	if (err != ampers)
		waitforeveryone(pids,i);
}

void conv(struct list* first, int wordcount)
{
	char***checksums;
	int i,err;
	char* Arr;
	struct list *last;
	checksums = malloc(2*sizeof(char**));
	i = 1;
	if ((checksums[i-1] = listtoarray(first,wordcount)) != NULL)
	{
		i++;
		first = last = malloc(sizeof(struct list));
		first->next = NULL;
		wordcount = 0;
		while ((err = readword(&Arr)) == space || err == convo)
		{
			if (Arr != NULL)
			{
				add(Arr,&last);
				wordcount++;
			}
			if (err == convo)
			{
				if ((checksums[i-1] = listtoarray(first,wordcount)) != NULL)
					muralloc(&checksums,++i);
				first = last = malloc(sizeof(struct list));
				first->next = NULL;
				clearpaths();
				wordcount = 0;
			}
		
		}
		if (err == endofile || err == badEOF)
		{
			holocaust(first, wordcount);
			nosums(checksums,i-1);
			if (err == endofile)
			{
				printf("%s",end);
			}
			else
				printf("%s",bedend);
			while (wait4(-1,NULL,WNOHANG,NULL) != -1)
				{}
			exit(0);
		}
		if (err != slashN && err != ampers)
		{
			if (err == badslashN)
			{
				printf("%s",badslashn);
			}
			else
				printf("%s",noconvo);
			holocaust(first,wordcount);
			nosums(checksums,i-1);
			while (wait4(-1,NULL,WNOHANG,NULL) > 0)
				{}
			printf("%s",greet);
			return;
		}
		if (Arr != NULL)
		{
			add(Arr,&last);
			wordcount++;
		}
		if ((checksums[i-1] = listtoarray(first,wordcount)) != NULL)
		{
			runconvo(checksums,i,err);
			nosums(checksums,i);
			clearpaths();
			printf("%s",greet);
			return;
		}
	}
	nosums(checksums,i);
	printf("%s",emptyconvo);
}

int main()
{
	struct list *first, *last, *p;
	char *Arr;
	int err, n = 0;
	fileflague = 0;
	filepaths[0] = filepaths[1] = filepaths[2] = NULL;
	fd[0] = fd[1] = fd[2] = -1;
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
		if (err == convo)
		{
			if (Arr != NULL)
			{
				add(Arr,&last);
				n++;
			}
			conv(first,n);
			p = first = last = malloc(sizeof(struct list));
			fileflague = 0;
			p->next = NULL;
			while (wait4(-1,NULL,WNOHANG,NULL) >0)
				{}
			n = 0;
			fd[0] = fd[1] = fd[2] = -1;
		} else
		{
			if (err == badslashN || 
				 err == argsafterpath || 
				 err == nofilename || 
				 err == doublepath)
			{
				holocaust(first,n);
				fileflague = 0;
				switch(err)
				{
				case badslashN:
					printf("%s",badslashn);
					break;
				case argsafterpath:
					printf("%s",argafterpath);
					break;
				case nofilename:
					printf("%s",noname);
					break;
				default:
					printf("%s",twopaths);
				}
				n = 0;
				fd[0] = fd[1] = fd[2] = -1;
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
					fileflague = 0;
					p->next = NULL;
					while (wait4(-1,NULL,WNOHANG,NULL) > 0)
						{}
					n = 0;
					fd[0] = fd[1] = fd[2] = -1;
					if (err == slashN)
						printf("%s",greet);
				}
			}
		}
	} while(1048576);
}
