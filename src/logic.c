#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logic.h>

#include "t2fs.h"
#include "apidisk.h"
#include "logic.h"
#include "util.h"


t2fs_superbloco_s* superbloco;
fat_s* fat;

BOOL isInit = FALSE;

void Init();
unsigned int DeleteFileWithoutSaving(unsigned int cluster);
unsigned int DeleteFile(unsigned int cluster);
unsigned int CreateFile(int n_clusters);
unsigned int AppendOneCluster(unsigned int cluster);
unsigned int AppendFile(unsigned int cluster, int n_clusters);
unsigned int getNextCluster(unsigned int cluster);
void SuperBlockRead();
void InitSuperBlock();
void SuperBlockPrint();
void InitFat();
void FatRead(int n_sectors);
void FatPrint(int n);
void FatWrite();
unsigned int getFreeCluster();

/**
 * Inicia o Superbloco e a Fat, indicando que já foi inicializado.
 */
void Init(){
    InitSuperBlock();
    InitFat();
    isInit = TRUE;
}
/**
 * Inicia o superbloco, reutilizando a memória se ele já foi inicializado.
 */
void InitSuperBlock() {
    if (isInit==FALSE)
        superbloco = malloc(SECTOR_SIZE);
    SuperBlockRead();
}
/**
 * Lê o superbloco. Se não conseguir, acaba o programa.
 */
void SuperBlockRead() {
    if (read_sector(SB_SECTOR, (unsigned char*)superbloco)!=0){
        perror("Erro ao ler o superbloco!");
        exit(SB_READ_ERROR);
    }
}
/**
 * Salva o superbloco.
 */
void SuperBlockWrite(){
    if (write_sector(SB_SECTOR, (unsigned char*) superbloco)!=0){
        perror("Erro ao salvar o superbloco!");
        exit(SB_WRITE_ERROR);
    }
}

/**
 * Print nas informações do superbloco.
 */
void SuperBlockPrint() {
    printf("Identificação do sistema de arquivo: %s\n", superbloco->id);
    printf("Versão atual desse sistema de arquivos: %d\n", superbloco->version);
    printf("Quantidade de setores lógicos que formam o superbloco: %d\n", superbloco->SuperBlockSize);
    printf("Tamanho total, em bytes: %d\n", superbloco->DiskSize);
    printf("Quantidade total de setores lógicos: %d\n", superbloco->NofSectors);
    printf("Número de setores lógicos que formam um cluster: %d\n", superbloco->SectorsPerCluster);
    printf("Número do setor lógico onde a FAT inicia: %d\n", superbloco->pFATSectorStart);
    printf("Cluster onde inicia o arquivo correspon- dente ao diretório raiz: %d\n", superbloco->RootDirCluster);
    printf("Primeiro setor lógico da área de blocos de dados (cluster 0): %d\n", superbloco->DataSectorStart);
}

/**
 * Lê um setor específico verificando se foi lido.
 * @param buffer
 * @param sector
 */
void FatReadSector(char* buffer, int sector){
    if (read_sector(sector, (unsigned char*)buffer)!=0){
        perror("Erro ao ler a FAT!");
        exit(FAT_READ_ERROR);
    }
}

/**
 * Lê toda a Fat.
 * @param n_sectors: número de setores que a Fat ocupa.
 */
void FatRead(int n_sectors){
    char* buffer = malloc(SECTOR_SIZE);
    FatReadSector(buffer, superbloco->pFATSectorStart);
    strcpy(fat->data, buffer);
    for (int i = 1; i < n_sectors; i++){
        FatReadSector(buffer, superbloco->pFATSectorStart + i);
        strcat(fat->data, buffer);
    }
    free(buffer);
}
/**
 * Procura o próximo cluster livre após o cluster inicado.
 * @param after: ponto inicial da procura
 * @return TRUE se foi possível achar um cluster livre
 *         FALSE se nao foi possivel
 */
int set_first_free(unsigned int after) {
    for (unsigned int i = after; i < fat->size; i++){
        if (fat->data[i]==FREE_CLUSTER){
            fat->first_free = i;
            return TRUE;
        }
    }
    fat->first_free = CLUSTER_NOT_AVAILABLE;
    return FALSE;
}

/**
 * Inicia a Fat, reutilizando a memória se ela já foi inicializada.
 */
void InitFat(){
    int n_cluster = (superbloco->NofSectors - superbloco->DataSectorStart)/superbloco->SectorsPerCluster;
    if (isInit==FALSE) {
        fat = malloc(sizeof(fat_s *));
        fat->data = malloc(n_cluster*sizeof(DWORD));
    }
    FatRead(n_cluster/SECTOR_SIZE);
    fat->size = n_cluster;
    set_first_free(2);
}

/**
 * Libera um cluster.
 * @param cluster : cluster a ser liberado.
 */
void FreeCluster(unsigned int cluster){
    fat->data[cluster] = FREE_CLUSTER;
    if (fat->first_free != CLUSTER_NOT_AVAILABLE || fat->first_free > cluster)
        fat->first_free = cluster;
}

/**
 * Deleta um arquivo tendo como parametro seu cluster inicial.
 * Apaga todos os clusters ligados ao seu cluster inicial.
 * @param cluster : cluster inicial do aquivo
 * @return SUCCESS se deletar sem encontrar erros
 *         CLUSTER_ERROR se encontrar um BAD_CLUSTER ou FREE_CLUSTER
 */
unsigned int DeleteFileWithoutSaving(unsigned int cluster){
    unsigned int next_cluster = fat->data[cluster];
    if (next_cluster==BAD_CLUSTER || next_cluster==FREE_CLUSTER)
        return CLUSTER_ERROR;
    FreeCluster(cluster);
    while (next_cluster!=EOF_CLUSTER){
        cluster = next_cluster;
        next_cluster = fat->data[cluster];
        if (next_cluster==BAD_CLUSTER || next_cluster==FREE_CLUSTER)
            return CLUSTER_ERROR;
        FreeCluster(cluster);
    }
    return SUCCESS;
}

/**
 * Deleta um arquivo tendo como parametro seu cluster inicial.
 * Apaga todos os clusters ligados ao seu cluster inicial
 * Salva a fat no disco. Se encontrar erros, reverte todas as mudancas.
 * @param cluster : cluster inicial do aquivo
 * @return SUCCESS se deletar o arquivos sem erros.
 *         CLUSTER_ERROR se encontrar erros.
 */
unsigned int DeleteFile(unsigned int cluster){
    if (DeleteFileWithoutSaving(cluster)==CLUSTER_ERROR){
        InitFat();//reset na fat
        return CLUSTER_ERROR;
    }
    FatWrite();
    return SUCCESS;
}

/**
 * Cria um arquivo com n clusters. Salva a Fat no disco.
 * @return  o cluster inicial do arquivo
 *          CLUSTER_NOT_AVAILABLE se não encontrar cluster suficientes.
 */
unsigned int CreateFile(int n_clusters){
    unsigned int first_cluster = getFreeCluster();
    if (first_cluster == CLUSTER_NOT_AVAILABLE)
        return CLUSTER_NOT_AVAILABLE;
    if (n_clusters > 1){
        if (AppendFile(first_cluster, n_clusters - 1)==CLUSTER_NOT_AVAILABLE)//AppendFile da o reset na Fat
            return CLUSTER_NOT_AVAILABLE;
    }
    else
        FatWrite();
    return first_cluster;
}

/**
 * Lococa o próximo cluster livre como o fim de um arquivo.
 * Procura um pŕoximo cluster livre.
 * @return o cluster a ser usado como fim de arquivo
 *         CLUSTER_NOT_AVAILABLE se não encontrar um cluster livre disponível.
 */
unsigned int getFreeCluster(){
    if (fat->data[fat->first_free] == CLUSTER_NOT_AVAILABLE)
        return CLUSTER_NOT_AVAILABLE;
    fat->data[fat->first_free] = EOF_CLUSTER;
    unsigned int cluster = fat->first_free;
    set_first_free(cluster);
    return cluster;
}

/**
 * Adiciona um cluster no fim de um arquivo.
 * @param cluster: cluster do arquivo a ser adicionado
 * @return o cluster que foi adicionado.
 *         CLUSTER_ERROR se encontrar algum BAD_CLUSTER ou FREE_CLUSTER no arquivo.
 *         CLUSTER_NOT_AVAILABLE se não encontrar um cluster livre disponível.
 */
unsigned int AppendOneCluster(unsigned int cluster){
    unsigned int next_cluster = fat->data[cluster];
    if (next_cluster==BAD_CLUSTER || next_cluster==FREE_CLUSTER)
        return CLUSTER_ERROR;
    while (next_cluster!=EOF_CLUSTER){
        cluster = next_cluster;
        next_cluster = fat->data[cluster];
        if (next_cluster==BAD_CLUSTER || next_cluster==FREE_CLUSTER)
            return CLUSTER_ERROR;
    }
    unsigned int cluster_to_append = getFreeCluster();
    if (cluster_to_append == CLUSTER_NOT_AVAILABLE)
        return CLUSTER_NOT_AVAILABLE;
    fat->data[cluster] = cluster_to_append;
    return cluster_to_append;
}

/**
 * Adiciona n clusters apartir do fim do arquivo. Salva a Fat no disco.
 * Reverte as mudancas caso nao tenha espaco disponivel.
 * @param cluster
 * @param n_clusters
 * @return o primeiro cluster adicionado.
 *         CLUSTER_NOT_AVAILABLE caso nao tenha clusters disponiveis.
 */
unsigned int AppendFile(unsigned int cluster, int n_clusters){
    unsigned int first_cluster = AppendOneCluster(cluster);
    if (first_cluster == CLUSTER_NOT_AVAILABLE)
        return CLUSTER_NOT_AVAILABLE;
    cluster = first_cluster;
    for (unsigned int i = 1; i < n_clusters; i++){
        cluster = AppendOneCluster(cluster);
        if (cluster == CLUSTER_NOT_AVAILABLE){
            InitFat();//regarrega a fat novamente, dando um reset nas mudancas feitas.
            return CLUSTER_NOT_AVAILABLE;
        }
    }
    FatWrite();
    return first_cluster;
}

/**
 * Salva a Fat no disco.
 */
void FatWrite(){
    if (write_sector(superbloco->pFATSectorStart, (unsigned char*)fat->data)!=0){
        perror("Nao foi possivel salvar a FAT!. Saindo.");
        exit(FAT_WRITE_ERROR);
    }
}

/**
 * Imprime os primeiros n clusters da fat.
 */
void FatPrint(int n){
    printf("%i - %x\n", fat->size, fat->first_free);
    for (int i = 0; i < n; i++)
        printf("%x = %x\n", i, fat->data[i]);
}

/**
 * Pega o pŕoximo cluster.
 * @param cluster
 * @return retorna o próximo cluster.
 *         -1 se 2 > cluster >= BAD_CLUSTER
 */
unsigned int getNextCluster(unsigned int cluster){
    if (cluster<2 && cluster>=BAD_CLUSTER)
        return fat->data[cluster];
    return CLUSTER_ERROR;
}

/*
 * FUNÇÔES PARA TESTE
 */
/**
 * Método get para testar esse módulo
 * @return o superbloco
 */
t2fs_superbloco_s* getSuperbloco(){
    return superbloco;
}
/**
 * Método get para testar esse módulo
 * @return a Fat
 */
fat_s* getFat(){
    return fat;
}