#ifndef T2FS_DATA_H
#define T2FS_DATA_H

#include "t2fs.h"
#include "util.h"

#define MAX_HANDLERS 256
#define OPEN_ERROR -1

#define CUR_PATH_SIZE 500
typedef struct t2fs_record record_s;

typedef struct {
    char path[CUR_PATH_SIZE];
    record_s* records;
    int n_records;
} dir_s;

typedef struct {
    DWORD id;
    DWORD cluster;
    DWORD offset;
} handler_s;

typedef struct {
    handler_s* handlers;
    int n_handlers;
    int first_free;
} handler_list_s;

void InitData();

BOOL ReadDirInfo(DWORD handler, DIRENT2* dentry);
int DeleteDirHandler(int handler);
int cd(char* path);
int getCurPath(char* path, int size);

#endif //CTHREAD_DATA_H
