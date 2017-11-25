
#ifndef T2FS_LOGIC_SECTORS
#define T2FS_LOGIC_SECTORS

#include "util.h"


#define SB_READ_ERROR -10
#define SB_WRITE_ERROR -11
#define FAT_READ_ERROR -12
#define FAT_WRITE_ERROR -13

#define SB_SECTOR 0

#define EOF_CLUSTER 0xFFFFFFFF
#define BAD_CLUSTER 0xFFFFFFFE
#define FREE_CLUSTER 0x00000000

/** Fat */
typedef struct{
    char* data;
    int size;
    int first_free;
}fat_s;

typedef struct t2fs_superbloco t2fs_superbloco_s;

void Init();
void DeleteFile(int cluster);
int CreateFile();
int AppendCluster(int cluster);


#endif