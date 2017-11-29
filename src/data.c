#include <math.h>

createNewDir(char *name, DWORD firstCluster)
{
	struct t2fs_record_s *newDir = malloc(sizeof(struct t2fs_record_s));
		newDir->TypeVal = TYPEVAL_DIRETORIO;
		newDir->name = name;
		newDir->bytesFileSize = 0;
		newDir->firstCluster = firstCluster;
	return newDir;
}
	

//Retorna o tamanho do cluster
 int getClusterSize()
{
	SuperBlockRead();
	return SECTOR_SIZE * (superbloco->SectorsPerCluster)
}

//Retorna o numero de clusters necessarios para o arquivo
int getNumClusters(t2fs_record_s dir )
{
	float numClusters;
	numClusters = (dir->bytesFileSize) / SECTOR_SIZE
	ceil(numClusters) // Arredonda para cima
	return numClusters
}

int isRoot (t2fs_record_s dir)
{
	if(dir->name[0] == '/')
		memcpy(& (t2fs_superbloco_s->rootDirCluster), dir, sizeof(struct t2fs_record_s) );
}
