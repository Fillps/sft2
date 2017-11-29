#ifndef T2FS_DATA_H
#define T2FS_DATA_H

#include "t2fs.h"

typedef struct t2fs_record record_s;

typedef struct {
    record_s record;
    record_s* records;
    int n_records;
} dir_s;

void InitData();



#endif //CTHREAD_DATA_H
