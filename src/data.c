#include <stdlib.h>
#include <string.h>
#include <data.h>
#include <stdio.h>

#include "apidisk.h"
#include "data.h"
#include "t2fs.h"
#include "logic.h"

dir_s current_dir;

void InitData(){
    ReadRoot();
}

void ReadRoot(){
    current_dir.records = malloc(getEntrySize(getSuperbloco()->RootDirCluster));
    ReadEntry(current_dir.records, getSuperbloco()->RootDirCluster);
    //current_dir.record.name = "root";
    current_dir.record.firstCluster = getSuperbloco()->RootDirCluster;
    //current_dir.record.bytesFileSize = strlen((char*)current_dir);
}

void ReadCluster(char* cluster_data, DWORD cluster){
    char* buffer = malloc(SECTOR_SIZE);
    DWORD sector = cluster*getSuperbloco()->SectorsPerCluster;
    read_sector(buffer, sector);
    strcpy(cluster_data, buffer);
    for (int i = 1; i < getSuperbloco()->SectorsPerCluster; i++){
        read_sector(buffer, sector + i);
        strcat(cluster_data, buffer);
    }
    free(buffer);
}

void ReadEntry(char* entry_data, DWORD cluster){
    char* buffer = malloc(SECTOR_SIZE*getSuperbloco()->SectorsPerCluster);
    ReadCluster(buffer, cluster);
    strcpy(entry_data, buffer);

    for(int i = 1; i < getEntryNClusters(cluster); i++){
        cluster = getNextCluster(cluster);
        ReadCluster(buffer, cluster);
        strcat(entry_data, buffer);
    }
    free(buffer);
}

void OpenDir(){

}

int getEntrySize(DWORD cluster){
    return getSuperbloco()->SectorsPerCluster*SECTOR_SIZE*getEntryNClusters(cluster);
}

void freeDir(){

}


/**
 * FUNCOES PARA TESTE
 */

dir_s* getDir(){
    return &current_dir;
}