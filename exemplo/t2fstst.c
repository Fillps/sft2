
/**

    T2 shell, para teste do T2FS - Sistema de arquivos do trabalho 2 de Sistemas Operacionais I

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/t2fs.h"

void cmdMan(void);

void cmdWho(void);
void cmdLs(void);
void cmdMkdir(void);
void cmdRmdir(void);

void cmdOpen(void);
void cmdRead(void);
void cmdClose(void);

void cmdWrite(void);
void cmdCreate(void);
void cmdDelete(void);
void cmdSeek(void);
void cmdTrunc(void);

void cmdChDir(void);
void cmdGetCwd(void);

void cmdCp(void);
void cmdFscp(void);

void cmdExit(void);

static void dump(char *buffer, int size) {
    int base, i;
    char c;
    for (base=0; base<size; base+=16) {
        printf ("%04d ", base);
        for (i=0; i<16; ++i) {
            if (base+i<size) printf ("%02X ", buffer[base+i]);
            else printf ("   ");
        }

        printf (" *");

        for (i=0; i<16; ++i) {
            if (base+i<size) c = buffer[base+i];
            else c = ' ';

            if (c<' ' || c>'z' ) c = ' ';
            printf ("%c", c );
        }
        printf ("*\n");
    }
}


char	helpExit[] = "finish this shell";
char	helpMan[] = "command help";
char	helpWho[] = "shows T2FS authors";
char	helpLs[] = "list files in dir [1]";
char	helpMkdir[] = "create dir [1] in T2FS";
char	helpRmdir[] = "remove dir [1] from T2FS";
char	helpChDir[] = "change current working dir to [1]";
char	helpGetCwd[] = "get current working dir";
char	helpOpen[] = "open file [1] from T2FS";
char	helpRead[] = "read [2] bytes from file-handle [1]";
char	helpClose[] = "close file-handle [1]";
char	helpWrite[] = "write [2] bytes in file-handle [1]";
char	helpCreate[] = "create file [1] in T2FS";
char	helpDelete[] = "delete file [1] from T2FS";
char	helpSeek[] = "set CP of file-handle [1] on position [2]";
char	helpTrunc[] = "truncate file-handle [1] to [2] bytes";
char	helpCp[] = "copy file [1] to [2]";
char	helpFscp[] = "fscp -t [1] [2] -> copy HostFS[1] to T2FS[2]\nfscp -f [1] [2] -> copy T2FS[1]   to HostFS[2]";

struct	{
	char	name[20];
	void	(*f)(void);
	char	*help;
	
} cmdList[] = {
	
	{ "exit",	cmdExit,	helpExit 	},
	{ "x",		cmdExit,	helpExit 	},
	{ "man",	cmdMan,		helpMan 	},
	{ "who",	cmdWho,		helpWho 	},
	{ "id",		cmdWho,		helpWho 	},
	{ "dir",	cmdLs,		helpLs 		},
	{ "ls",		cmdLs,		helpLs 		},
	{ "mkdir",	cmdMkdir,	helpMkdir 	},
	{ "md",		cmdMkdir,	helpMkdir 	},
	{ "rmdir",	cmdRmdir,	helpRmdir 	},
	{ "rm",		cmdRmdir,	helpRmdir 	},
	{ "chdir",	cmdChDir,	helpChDir 	},
	{ "chdir",	cmdChDir,	helpChDir 	},
	{ "cd",		cmdChDir,	helpChDir 	},
	{ "getcwd",	cmdGetCwd,	helpGetCwd 	},
	{ "cwd",	cmdGetCwd,	helpGetCwd 	},
	
	{ "open",	cmdOpen,	helpOpen 	},
	{ "read",	cmdRead,	helpRead	},
	{ "rd",		cmdRead,	helpRead	},
	{ "close",	cmdClose,	helpClose	},
	{ "cl",		cmdClose,	helpClose	},
	{ "write",	cmdWrite,	helpWrite	},
	{ "wr",		cmdWrite,	helpWrite	},
	{ "create",	cmdCreate,	helpCreate	},
	{ "cr",		cmdCreate,	helpCreate	},
	{ "delete",	cmdDelete,	helpDelete	},
	{ "del",	cmdDelete,	helpDelete	},
	{ "seek",	cmdSeek,	helpSeek	},
	{ "sk",		cmdSeek,	helpSeek	},
	{ "truncate",	cmdTrunc,	helpTrunc	},
	{ "trunc",	cmdTrunc,	helpTrunc	},
	{ "tk",		cmdTrunc,	helpTrunc	},
	
	{ "cp",		cmdCp,		helpCp		},
	{ "fscp",	cmdFscp,	helpFscp	},
	{ "fim",	NULL,		NULL		}

	
};




int main()
{
    char cmd[256];
    char *token;
    int i;
    int flagAchou, flagEncerrar;

    //token = strtok("who"," \t");
    strcpy(cmd, "man");
    token = strtok(cmd," \t");
    cmdMan();

    flagEncerrar = 0;
    while (1) {
        printf ("T2FS> ");
        gets(cmd);
        if( (token = strtok(cmd," \t")) != NULL ) {	
		flagAchou = 0;
		for (i=0; cmdList[i].f!=NULL; i++) {
			if (strcmp(cmdList[i].name, token)==0) {
				flagAchou = 1;
				cmdList[i].f();
				if (cmdList[i].f == cmdExit) {
					flagEncerrar = 1;
				}
				break;
			}
		}
		if (!flagAchou) printf ("???\n");
        }
	if (flagEncerrar) break;
    }
    return 0;
}

/**
Encerra a operação do teste
*/
void cmdExit(void) {
    printf ("bye, bye!\n");
}

/**
Informa os comandos aceitos pelo programa de teste
*/
void cmdMan(void) {
	int i;
	char *token = strtok(NULL," \t");
	
	printf ("Testing program for T2FS - v 2017.2.0\n");
	
	if (token==NULL) {
		for (i=0; strcmp(cmdList[i].name,"fim")!=0; i++) {
			if (cmdList[i].help == cmdList[i+1].help) {
				printf ("%s, ", cmdList[i].name);
				continue;
			}
			printf ("%s => %s\n", cmdList[i].name, cmdList[i].help);
		}
		printf ("\n");
		return;
	}
	
	for (i=0; strcmp(cmdList[i].name,"fim")!=0; i++) {
		if (strcmp(cmdList[i].name,token)==0) {
			printf ("%s: %s\n", cmdList[i].name, cmdList[i].help);
		}
	}
	

}

	
/**
Chama da função identify2 da biblioteca e coloca o string de retorno na tela
*/
void cmdWho(void) {
    char name[256];
    int err = identify2(name, 256);
    if (err) {
        printf ("Erro: %d\n", err);
        return;
    }
    printf ("%s\n", name);
}

/**
Copia arquivo dentro do T2FS
Os parametros são:
    primeiro parametro => arquivo origem
    segundo parametro  => arquivo destino
*/
void cmdCp(void) {

    // Pega os nomes dos arquivos origem e destion
    char *src = strtok(NULL," \t");
    char *dst = strtok(NULL," \t");
    if (src==NULL || dst==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // Abre o arquivo origem, que deve existir
    FILE2 hSrc = open2 (src);
    if (hSrc<0) {
        printf ("Open source file error: %d\n", hSrc);
        return;
    }
    // Cria o arquivo de destino, que será resetado se existir
    FILE2 hDst = create2 (dst);
    if (hDst<0) {
        close2(hSrc);
        printf ("Create destination file error: %d\n", hDst);
        return;
    }
    // Copia os dados de source para destination
    char buffer[2];
    while( read2(hSrc, buffer, 1) == 1 ) {
        write2(hDst, buffer, 1);
    }
    // Fecha os arquicos
    close2(hSrc);
    close2(hDst);

    printf ("Files successfully copied\n");
}

/**
Copia arquivo de um sistema de arquivos para o outro
Os parametros são:
    primeiro parametro => direção da copia
        -t copiar para o T2FS
        -f copiar para o FS do host
    segundo parametro => arquivo origem
    terceiro parametro  => arquivo destino
*/
void cmdFscp(void) {
    // Pega a direção e os nomes dos arquivos origem e destion
    char *direcao = strtok(NULL, " \t");
    char *src = strtok(NULL," \t");
    char *dst = strtok(NULL," \t");
    if (direcao==NULL || src==NULL || dst==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // Valida direção
    if (strncmp(direcao, "-t", 2)==0) {
        // src == host
        // dst == T2FS

        // Abre o arquivo origem, que deve existir
        FILE *hSrc = fopen(src, "r+");
        if (hSrc==NULL) {
            printf ("Open source file error\n");
            return;
        }
        // Cria o arquivo de destino, que será resetado se existir
        FILE2 hDst = create2 (dst);
        if (hDst<0) {
            fclose(hSrc);
            printf ("Create destination file error: %d\n", hDst);
            return;
        }
        // Copia os dados de source para destination
        char buffer[2];
        while( fread((void *)buffer, (size_t)1, (size_t)1, hSrc) == 1 ) {
            write2(hDst, buffer, 1);
        }
        // Fecha os arquicos
        fclose(hSrc);
        close2(hDst);
    }
    else if (strncmp(direcao, "-f", 2)==0) {
        // src == T2FS
        // dst == host

        // Abre o arquivo origem, que deve existir
        FILE2 hSrc = open2 (src);
        if (hSrc<0) {
            printf ("Open source file error: %d\n", hSrc);
            return;
        }
        // Cria o arquivo de destino, que será resetado se existir
        FILE *hDst = fopen(dst, "w+");
        if (hDst==NULL) {
            printf ("Open destination file error\n");
            return;
        }
        // Copia os dados de source para destination
        char buffer[2];
        while ( read2(hSrc, buffer, 1) == 1 ) {
            fwrite((void *)buffer, (size_t)1, (size_t)1, hDst);
        }
        // Fecha os arquicos
        close2(hSrc);
        fclose(hDst);
    }
    else {
        printf ("Invalid copy direction\n");
        return;
    }

    printf ("Files successfully copied\n");
}

/**
Cria o arquivo informado no parametro
Retorna eventual sinalização de erro
Retorna o HANDLE do arquivo criado
*/
void cmdCreate(void) {
    FILE2 hFile;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    hFile = create2 (token);
    if (hFile<0) {
        printf ("Error: %d\n", hFile);
        return;
    }

    printf ("File created with handle %d\n", hFile);
}

/**
Apaga o arquivo informado no parametro
Retorna eventual sinalização de erro
*/
void cmdDelete(void) {

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    int err = delete2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("File %s was deleted\n", token);
}

/**
Abre o arquivo informado no parametro [0]
Retorna sinalização de erro
Retorna HANDLE do arquivo retornado
*/
void cmdOpen(void) {
    FILE2 hFile;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    hFile = open2 (token);
    if (hFile<0) {
        printf ("Error: %d\n", hFile);
        return;
    }

    printf ("File opened with handle %d\n", hFile);
}

/**
Fecha o arquivo cujo handle é o parametro
Retorna sinalização de erro
Retorna mensagem de operação completada
*/
void cmdClose(void) {
    FILE2 handle;

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    int err = close2(handle);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Closed file with handle %d\n", handle);
}

void cmdRead(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // Alloc buffer for reading file
    char *buffer = malloc(size);
    if (buffer==NULL) {
        printf ("Memory full\n");
        return;
    }

    // get file bytes
    int err = read2(handle, buffer, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }
    if (err==0) {
        printf ("Empty file\n");
        return;
    }

    // show bytes read
    dump(buffer, err);
    printf ("%d bytes read from file-handle %d\n", err, handle);
    
    free(buffer);
}

void cmdWrite(void) {
    FILE2 handle;
    int size;
    int err;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => string
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    size = strlen(token);

    // get file bytes
    err = write2(handle, token, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }
    if (err!=size) {
        printf ("Erro: escritos %d bytes, mas apenas %d foram efetivos\n", size, err);
        return;
    }

    printf ("%d bytes writen to file-handle %d\n", err, handle);
}

/**
Cria um novo diretorio
*/
void cmdMkdir(void) {
    // get first parameter => pathname
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // change working dir
    int err = mkdir2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Created new directory\n");
}

/**
Apaga um diretorio
*/
void cmdRmdir(void) {
    // get first parameter => pathname
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    // change working dir
    int err = rmdir2(token);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Directory was erased\n");
}

void cmdLs(void) {

    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }

    // Abre o diretório pedido
    DIR2 d;
    d = opendir2(token);
    if (d<0) {
        printf ("Open dir error: %d\n", d);
        return;
    }

    // Coloca diretorio na tela
    DIRENT2 dentry;
    while(1) {
	    int errCode = readdir2(d, &dentry);
	    
	    if (errCode==-END_OF_DIR)
		    break;
	    
	    if (errCode!=0) {
		    printf ("readdir2 error code: %d\n", errCode);
		    break;
	    }
	    
	    printf ("%c %8u %s\n", (dentry.fileType==0x02?'d':'-'), dentry.fileSize, dentry.name);
    }

    closedir2(d);


}


/**
Chama a função truncate2() da biblioteca e coloca o string de retorno na tela
*/
void cmdTrunc(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }
    
    // posiciona CP na posicao selecionada
    int err = seek2(handle, size);
    if (err<0) {
        printf ("Error seek2: %d\n", err);
        return;
    }
    
    // trunca
    err = truncate2(handle);
    if (err<0) {
        printf ("Error truncate2: %d\n", err);
        return;
    }

    // show bytes read
    printf ("file-handle %d truncated to %d bytes\n", handle, size );
}

void cmdSeek(void) {
    FILE2 handle;
    int size;

    // get first parameter => file handle
    char *token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &handle)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // get second parameter => number of bytes
    token = strtok(NULL," \t");
    if (token==NULL) {
        printf ("Missing parameter\n");
        return;
    }
    if (sscanf(token, "%d", &size)==0) {
        printf ("Invalid parameter\n");
        return;
    }

    // seek
    int err = seek2(handle, size);
    if (err<0) {
        printf ("Error: %d\n", err);
        return;
    }

    printf ("Seek completado para a posicao %d\n", size);
    
}

void cmdChDir(void) {
	
}

void cmdGetCwd(void) {
	
}




