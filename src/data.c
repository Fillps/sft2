#include <stdlib.h>
#include <string.h>
#include <data.h>
#include <stdio.h>
#include <t2fs.h>


#include "util.h"
#include "apidisk.h"
#include "data.h"
#include "t2fs.h"
#include "logic.h"

dir_s current_dir;

handler_list_s file_handlers;
handler_list_s dir_handlers;
BOOL isInitData = FALSE;

void InitHandlers();

void InitData(){
    ReadRoot();
    InitHandlers();
    isInitData = TRUE;
}

void InitHandlers() {
    file_handlers.handlers = calloc(MAX_HANDLERS, sizeof(handler_s));
    file_handlers.n_handlers = 0;
    file_handlers.first_free = 0;
    dir_handlers.handlers = calloc(MAX_HANDLERS, sizeof(handler_s));
    dir_handlers.n_handlers = 0;
    dir_handlers.first_free = 0;
}

void ReadRoot(){
    current_dir.records = (record_s*)calloc(getEntrySize(getSuperbloco()->RootDirCluster)/sizeof(record_s), sizeof(record_s));
    strcpy(current_dir.path, "/");
    setCurrentDir(getSuperbloco()->RootDirCluster, &current_dir);
}

void ReadClusterDir(record_s *cluster_dirs, DWORD cluster){
    DWORD sector = cluster*getSuperbloco()->SectorsPerCluster + getSuperbloco()->DataSectorStart;
    read_sector(sector, (unsigned char*)cluster_dirs);
    for (DWORD i = 1; i < getSuperbloco()->SectorsPerCluster; i++){
        read_sector(sector + i, (unsigned char*)cluster_dirs+SECTOR_SIZE*i);
    }
}

BOOL ReadDir(record_s *entry_data, DWORD cluster){
    int cluster_size = getSuperbloco()->SectorsPerCluster*SECTOR_SIZE;
    ReadClusterDir(entry_data, cluster);
    for(int i = 1; i < getEntryNClusters(cluster); i++){
        cluster = getNextCluster(cluster);
        ReadClusterDir(entry_data + cluster_size * i, cluster);
    }
    return TRUE;
}

int setCurrentDir(DWORD cluster, dir_s* dir) {
    int current_cluster = dir->records[0].firstCluster;
    free(dir->records);
    dir->records = (record_s*)calloc(getEntrySize(cluster)/sizeof(record_s), sizeof(record_s));
    if (ReadDir(dir->records, cluster)==FALSE){
        perror("Erro ao abrir o diretorio!\n");
        setCurrentDir(current_cluster, &current_dir);
        return FALSE;
    }
    dir->n_records = 0;
    while (dir->records[dir->n_records].firstCluster!=0)
        dir->n_records++;
    return SUCCESS;
}

DWORD CreateDirHandler(DWORD cluster){
    return CreateHandler(&dir_handlers, cluster);
}

int CreateHandler(handler_list_s *handler_list, DWORD cluster){
    for(int i = 0; i < MAX_HANDLERS; i++){
        if (handler_list->handlers[i].cluster==cluster){
            return -1;
        }
    }
    handler_s* handler = handler_list->handlers+handler_list->first_free;
    handler->cluster = cluster;
    handler->id = handler_list->first_free;
    handler->offset = 0;
    set_next_handler_free(handler_list);
    return handler->id;
}

void set_next_handler_free(handler_list_s* handler_list){
    for (int i = 0; i < MAX_HANDLERS; i++){
        if (handler_list->handlers[i].cluster==0) {
            handler_list->first_free = i;
            return;
        }
    }
}

BOOL delete_handler(handler_list_s* handler_list, int id){
    for (int i = 0; i < MAX_HANDLERS; i++){
        if (handler_list->handlers->id==id) {
            handler_list->first_free = i;
            memset(handler_list->handlers+i,0,sizeof(handler_s));
            if (handler_list->first_free > i)
                handler_list->first_free = i;
            return TRUE;
        }
    }
    return FALSE;
}

int DeleteDirHandler(int handler){
    return delete_handler(&dir_handlers, handler);
}

int OpenDir(char *path, dir_s* dir){
    if (path[0]=='/') {//checa se path eh absoluto
        strcpy(dir->path, "/");
        setCurrentDir(getSuperbloco()->RootDirCluster, &current_dir);
        if (strlen(path)==1)//checa se é apenas root
            return SUCCESS;
        return OpenDir(path+1, dir);//abre resto apos root
    }
    char* sub = strchr(path, '/');
    if (sub == NULL)//eh o ultimo diretorio
        return OpenDirByName(path, dir);

    char* name = malloc(sub-path+1);
    memcpy(name, path, sub-path);
    name[sub-path] = '\0';
    if (OpenDirByName(name, dir)==OPEN_ERROR)//abre primeiro dir do path
        return OPEN_ERROR;
    free(name);
    return OpenDir(sub+1, dir);//abre abre o resto do path
}

int OpenDirByName(char *name, dir_s* dir){
    for (int i = 0; i < dir->n_records; i++){
        if (strcmp(dir->records[i].name, name)==0 && dir->records[i].TypeVal==TYPEVAL_DIRETORIO){
            if(setCurrentDir(dir->records[i].firstCluster, dir)==SUCCESS){

                strcat(dir->path, name);
                printf("%s\n%s\n", dir->path,name);
                return SUCCESS;
            }
        }
    }
    return OPEN_ERROR;
}

int getEntrySize(DWORD cluster){
    return getSuperbloco()->SectorsPerCluster*SECTOR_SIZE*getEntryNClusters(cluster);
}

BOOL ReadDirInfo(DWORD handler, DIRENT2* dentry){
    dir_s dir;
    dir.records = (record_s*)calloc(getEntrySize(dir_handlers.handlers[handler].cluster)/sizeof(record_s), sizeof(record_s));
    setCurrentDir(dir_handlers.handlers[handler].cluster, &dir);
    DIRENT2* dirent2 = malloc(sizeof(DIRENT2));
    int ret;
    if(dir_handlers.handlers[dir_handlers.handlers[handler].offset].id!=0){
        memcpy(dirent2->name, dir.records[dir_handlers.handlers[handler].offset].name, MAX_FILE_NAME_SIZE);
        dirent2->name[MAX_FILE_NAME_SIZE] = '\0';
        dirent2->fileType = dir.records[dir_handlers.handlers[handler].offset].TypeVal;
        dirent2->fileSize = dir.records[dir_handlers.handlers[handler].offset].bytesFileSize;
        memcpy(dentry, dirent2, sizeof(DIRENT2));
        dir_handlers.handlers[handler].offset++;
        ret = SUCCESS;
    }
    else
        ret = -END_OF_DIR;
    free(dirent2);
    free(dir.records);
    return ret;
}

int cd(char* path){
    return OpenDir(path, &current_dir);
}

int getCurPath(char* path, int size){
    //printf("%s\n\n",current_dir.path,size);
    strncpy(path,current_dir.path,size);
    path[size - 1] = '\0'; //strnpy nao adiciona o null no fim da string
    return strlen(path)-strlen(current_dir.path);
}

void PrintDir(){
    for (int i = 0; i < current_dir.n_records; i++){
        printf("\nnome:%s cluster:%x tipo:%x\n",
               current_dir.records[i].name,
               current_dir.records[i].firstCluster,
               current_dir.records[i].TypeVal);
    }
}

void PrintHandlers(handler_list_s* handler_list){
    for (int i = 0; i < MAX_HANDLERS; i++){
        if (handler_list->handlers[i].cluster!=0){
            printf("id:%u cluster:%x offset:%u\n",
                   handler_list->handlers[i].id,
                   handler_list->handlers[i].cluster,
                   handler_list->handlers[i].offset);
        }
    }
}

void PrintDirHandlers(){
    PrintHandlers(&dir_handlers);
}

/**
 * FUNCOES PARA TESTE
 */

dir_s* getDir(){
    return &current_dir;
}