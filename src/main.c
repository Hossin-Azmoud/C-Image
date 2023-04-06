#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define TYPE_CAP     4
#define CHUNK_SIZE   1024
#define IEND         0x444E4549

typedef struct PNGChunk 
{
	uint32_t length; 
	uint8_t  type[TYPE_CAP];
	uint32_t data[CHUNK_SIZE];
	uint32_t crc;
} Chunk;


#define PNG_SIG_CAP 8
const uint8_t PNG_HEADER[PNG_SIG_CAP] = {137, 80, 78, 71, 13, 10, 26, 10};

void read_bytes(FILE *Stream, void *buff, size_t buff_cap)
{
	size_t n = fread(buff, buff_cap, 1, Stream);
	if (n != 1) {
        if (ferror(Stream)) {
            fprintf(stderr, "ERROR: could not read %zu bytes from the file.\n", buff_cap);
            
			fclose(Stream);
			exit(1);
        } else if (feof(Stream)) {
            fprintf(stderr, "ERROR: could not read %zu bytes from the file. EOF Error!\n", buff_cap);
			fclose(Stream);
			exit(1);
        } else {
			fclose(Stream);
            assert(0 && "unreachable");
			exit(1);
        }

    }

}

void reverse_bytes(void *buf0, size_t buf_cap)
{
    uint8_t *buf = buf0;
    for (size_t i = 0; i < buf_cap/2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[buf_cap - i - 1];
        buf[buf_cap - i - 1] = t;
    }
}

int main(int argc, char **argv)
{
	const char *PROGRAM_NAME = *argv++;	
	if(*argv == NULL)
	{
		fprintf(stderr, "The png File Path was not provided.\n");
		fprintf(stdout, "USAGE: %s <filePath> .\n", PROGRAM_NAME);
		exit(0);
	}
	
	const char *FilePath = *argv;
	uint8_t    sig[PNG_SIG_CAP];
	FILE       *Input = fopen(FilePath, "rb");
	
	if(Input == NULL)
	{
		fprintf(stderr, "Could not open the file %s\n", FilePath);
		fprintf(stderr, "ERROR: %m\n");
		exit(1);
	}


	read_bytes(Input, sig, PNG_SIG_CAP);
	
	if (memcmp(sig, PNG_HEADER, PNG_SIG_CAP) != 0)
	{
		fprintf(stderr, "Invalid png file\n");
		fclose(Input);
		exit(1);
	}
	
	printf("SIG: ");

	for(size_t i = 0; i < PNG_SIG_CAP; i++)
	{
		printf(" %i ", *(sig + i));
	}
	
	printf("\n");
	
	Chunk *chunk = malloc(sizeof(Chunk));
	

	read_bytes(Input, &chunk->length, sizeof(chunk->length));
	reverse_bytes(&chunk->length, sizeof(chunk->length));
	printf("Length -> %i\n", chunk->length);
	
	printf("sizeof(chunk->type) = %zu\n", sizeof(chunk->type));
	read_bytes(Input, &chunk->type, sizeof(chunk->type));
	printf("Type -> ", chunk->type);

	for(int i = 0; i < TYPE_CAP; i++)
	{
		printf(" %c ", chunk->type[i]);
	}

	printf("\n");
	fclose(Input);

	return 0;
}

#if 0



#endif
