
#ifndef T2FS_LOGIC_H
#define T2FS_LOGIC_H

#define SB_READ_ERROR -10
#define SB_WRITE_ERROR -11
#define FAT_READ_ERROR -12
#define FAT_WRITE_ERROR -13

#define SB_SECTOR 0

#define FREE_CLUSTER 0x0
static unsigned int BAD_CLUSTER = 0xFFFFFFFD;
static unsigned int EOF_CLUSTER = 0xFFFFFFFF;

#define SUCCESS 0
#define CLUSTER_NOT_AVAILABLE 1
#define CLUSTER_ERROR 1

/** Fat */
typedef struct{
    char* data;
    int size;
    unsigned int first_free;
}fat_s;

typedef struct t2fs_superbloco t2fs_superbloco_s;

void Init();
unsigned int DeleteFile(unsigned int cluster);
unsigned int CreateFile(int n_clusters);
unsigned int AppendFile(unsigned int cluster, int n_clusters);
unsigned int getNextCluster(unsigned int cluster);


#endif