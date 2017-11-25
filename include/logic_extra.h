//
// Created by filipe on 11/25/17.
//

#ifndef CTHREAD_LOGIC_EXTRA_H
#define CTHREAD_LOGIC_EXTRA_H

#include "t2fs.h"
/*
 * FUNCOES APENAS PARA SEREM USADAS NOS TESTES
 */


t2fs_superbloco_s* getSuperbloco();
fat_s* getFat();
void SuperBlockRead();
void InitSuperBlock();
void SuperBlockPrint();
void InitFat();
void FatRead(int n_sectors);
void FatPrint(int n);
void FatWrite();

#endif //CTHREAD_LOGIC_EXTRA_H
