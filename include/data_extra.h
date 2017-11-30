#ifndef T2FS_DATA_EXTRA_H
#define T2FS_DATA_EXTRA_H

#include "data.h"

dir_s* getDir();
void ReadDir(char *cluster_data, DWORD cluster);
void PrintDir();
void PrintDirHandlers();
DWORD CreateDirHandler(DWORD cluster);
#endif //CTHREAD_DATA_EXTRA_H
