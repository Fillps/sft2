/* apidisk.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include "../include/error_codes.h"
#include "../include/apidisk.h"

static int is_initialized = 0;
static char *disk_filename;
static FILE *handle;

static void initialize (void)
{
	int status;

	disk_filename = getenv("T2FS_DISK_FILE");

	if (disk_filename == NULL) {
		fprintf(stderr, "Erro fatal\n");
		fprintf(stderr, "apidisk: Arquivo com a partição do disco não fornecido.\n");
		fprintf(stderr, "apidisk: Forneça via variável de ambiente `T2FS_DISK_FILE'\n");
		exit(APIDISK_NO_DISK_NAME);
	}

	if (access(disk_filename, F_OK) != 0) {
		fprintf(stderr, "Erro fatal\n");
		fprintf(stderr, "apidisk: Arquivo de disco \"%s\" não existe.\n", disk_filename);
		exit(APIDISK_DISK_DOESNT_EXIST);
	}

	if (access(disk_filename, R_OK | W_OK) != 0) {
		fprintf(stderr, "Erro fatal\n");
		fprintf(stderr, "apidisk: Sem permissão de leitura ou escrita no arquivo \"%s\".\n", disk_filename);
		exit(APIDISK_NO_ACCESS_RIGHTS);
	}

	// Arquivo de disco é aberto para leitura e escrita no modo "append"
	handle = fopen(disk_filename, "r+");

	if (handle == NULL) {
		fprintf(stderr, "Erro fatal\n");
		fprintf(stderr, "apidisk: Não foi possível abrir o arquivo \"%s\".\n", disk_filename);
		exit(APIDISK_CANT_OPEN);
	}

	// Reseta file pointer para o ínicio
	status = fseek(handle, 0, SEEK_SET);

	if (status != 0) {
		fprintf(stderr, "Erro fatal\n");
		fprintf(stderr, "apidisk: Não foi possível fazer o seek no disco\n");
		exit(APIDISK_CANT_SEEK);
	}

	is_initialized = 1;
}

int read_sector (unsigned int sector, char *buffer)
{
	int status;
	int bytes_read;

	if (!is_initialized)
		initialize();

	status = fseek(handle, SECTOR_SIZE * sector, SEEK_SET);

	if (status != 0) {
		fprintf(stderr, "apidisk: Não foi possível fazer o seek no disco durente leitura\n");
		return APIDISK_CANT_SEEK;
	}

	/*
	 * Os parâmetros nmemb e size foram "invertidos" aqui para facilitar a obtenção
	 * do número de bytes lidos.
	 */
	bytes_read = fread((void *) buffer, 1, SECTOR_SIZE, handle);

	if (bytes_read < SECTOR_SIZE) {
		fprintf(stderr, "apidisk: Não foi possível ler um setor completo\n");
		fprintf(stderr, "apidisk: lidos %d bytes\n", bytes_read);
		return APIDISK_READ_ERROR;
	}

	return 0;
}

int write_sector (unsigned int sector, char *buffer)
{
	int return_val;
	int bytes_written;

	if (!is_initialized)
		initialize();

	return_val = fseek(handle, SECTOR_SIZE * sector, SEEK_SET);

	if (return_val != 0) {
		fprintf(stderr, "Erro\n");
		fprintf(stderr, "apidisk: Não foi possível fazer o seek no disco durente escrita\n");
		exit(APIDISK_CANT_SEEK);
	}

	/*
	 * Os parâmetros nmemb e size foram "invertidos" aqui para facilitar a obtenção
	 * do número de bytes escritos.
	 */
	bytes_written = fwrite((void *) buffer, 1, SECTOR_SIZE, handle);

	if (bytes_written != SECTOR_SIZE) {
		fprintf(stderr, "Erro\n");
		fprintf(stderr, "apidisk: Não foi possível gravar um setor completo no disco\n");
		fprintf(stderr, "apidisk: escritos %d bytes\nErro retornado pela perror:\n\t", bytes_written);
		perror(NULL);
		return APIDISK_WRITE_ERROR;
	}

	return 0;
}