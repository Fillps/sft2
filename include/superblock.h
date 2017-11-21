#include "data.h"
#include "apidisk.h"

void SuperBlockRead(char *buffer);
bool InitSuperBlock();
void SuperBlockPrint();
char *FatRead(int clusterNumber);//Cluster number é para saber se deve ser lida a Fat Principal ou a Secundária. Retorna o buffer da fat lida
void FatPrint();