

#include "../include/superblock_operations.h"
#include "../include/t2fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t2fs_sbloco *superbloco;

bool InitSuperBlock()
{
    char *buffer = (char*) malloc(SECTOR_SIZE);
    SuperBlockRead(buffer);
    superbloco = (t2fs_sbloco*) malloc(sizeof(t2fs_sbloco));
	memcpy(superbloco, buffer, SECTOR_SIZE);
	return true;
}

void SuperBlockRead(char *buffer)
{
    printf("buffer: %s\n", buffer);
    unsigned int superBlocoSector = 0;
    int status = read_sector (superBlocoSector, buffer);
    printf("status: %d\n", status);

    //int write_sector (unsigned int sector, char *buffer);


    printf("buffer: %s\n", buffer);
}


void SuperBlockPrint()
{
    printf("Identificação do sistema de arquivo: %s\n", superbloco->id);
    printf("Versão atual desse sistema de arquivos: %d\n", superbloco->version);
    printf("Quantidade de setores lógicos que formam o superbloco: %d\n", superbloco->SuperBlockSize);
    printf("Tamanho total, em bytes: %d\n", superbloco->DiskSize);
    printf("Quantidade total de setores lógicos: %d\n", superbloco->NofSectors);
    printf("Número de setores lógicos que formam um cluster: %d\n", superbloco->SectorPerCluster);
    printf("Número do setor lógico onde a FAT inicia: %d\n", superbloco->pFATSectorStart);
    printf("Cluster onde inicia o arquivo correspon- dente ao diretório raiz: %d\n", superbloco->RootDirCluster);
    printf("Primeiro setor lógico da área de blocos de dados (cluster 0): %d\n", superbloco->DataSectorStart);

}

char *FatRead(int clusterNumber){return NULL;}
void FatPrint(){}
