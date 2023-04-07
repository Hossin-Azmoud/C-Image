/* Made by Moody0101 (Hossin azmoud) */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define TYPE_CAP     4
#define CHUNK_SIZE   1024
#define IEND         0x444E4549
#define PNG_SIG_CAP 8

typedef enum Format {
	HEX = 0,
	ASCII,
} Format;

const uint8_t PNG_HEADER[PNG_SIG_CAP] = {137, 80, 78, 71, 13, 10, 26, 10};

 
typedef struct PNGChunk 
{   
	uint32_t length; 
	uint8_t  type[TYPE_CAP];
	uint32_t data[CHUNK_SIZE];
	uint32_t crc;
} Chunk;     
     
typedef struct IHDR {
	uint32_t Width, Height; 
   	uint8_t BitDepth;
   	uint8_t ColorType;
   	uint8_t CompressionMethod;
   	uint8_t FilterMethod;
   	uint8_t InterlaceMethod;
} IHDR;

void print_byte(uint8_t byte, Format f)
{
	if(f == HEX)
	{
		printf(" %x ", byte);
		return;
	}
	
	if(f == ASCII)
	{
		if(isalnum(byte)) {
			printf(" %c ", byte);
			return;
		}

		print_byte(byte, HEX);
	}
}

void print_as(const char *Label, uint8_t *buffer, size_t cap, Format f)
{
	if(!f) f = HEX;

	printf("%s :", Label);
	
	for(int i = 0; i < cap; i++)
	{
		uint8_t v = buffer[i]; 
		print_byte(v, f);
	}

	printf("\n");
}

void report_chunk(Chunk *C)
{   
	printf("\n");	
	printf("Chunk Type: %i\n", C->length);
	print_as("Chunk Type: ", C->type, sizeof(C->type), ASCII);
	
	printf("Data: SKIPPED (TODO)\n");
	printf("CRC: %i\n", C->crc);
	printf("\n");	
}

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

void PrintIHDR(IHDR *ihdr)
{
	printf("width:             %i\n", ihdr->Width);
	printf("Height:            %i\n", ihdr->Height); 
   	printf("BitDepth:          %i\n", ihdr->BitDepth);
   	printf("ColorType:         %i\n", ihdr->ColorType);
   	printf("CompressionMethod: %i\n", ihdr->CompressionMethod);
   	printf("FilterMethod:      %i\n", ihdr->FilterMethod);
   	printf("InterlaceMethod:   %i\n", ihdr->InterlaceMethod);
}

IHDR *ReadIHDR(FILE *Stream) {
	IHDR *ihdr = malloc(sizeof(IHDR));
		
	read_bytes(Stream, &ihdr->Width,             sizeof(ihdr->Width));
	reverse_bytes(&ihdr->Width, sizeof(ihdr->Width));
	
	read_bytes(Stream, &ihdr->Height,            sizeof(ihdr->Height)); 
	
	reverse_bytes(&ihdr->Height, sizeof(ihdr->Height));

	read_bytes(Stream, &ihdr->BitDepth,          sizeof(ihdr->BitDepth));
   	read_bytes(Stream, &ihdr->ColorType,         sizeof(ihdr->ColorType));
   	read_bytes(Stream, &ihdr->CompressionMethod, sizeof(ihdr->CompressionMethod));
   	read_bytes(Stream, &ihdr->FilterMethod,      sizeof(ihdr->FilterMethod));
   	read_bytes(Stream, &ihdr->InterlaceMethod,   sizeof(ihdr->InterlaceMethod));

	return ihdr;
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
	
	print_as("PNG header -> ", sig, PNG_SIG_CAP, HEX);

	
	Chunk *chunk = malloc(sizeof(Chunk));

	read_bytes(Input, &chunk->length, sizeof(chunk->length));
	reverse_bytes(&chunk->length, sizeof(chunk->length));
	
	read_bytes(Input, &chunk->type, sizeof(chunk->type));
	fseek(Input, chunk->length, SEEK_CUR);
	
	
	read_bytes(Input, &chunk->crc, sizeof(chunk->crc));
	reverse_bytes(&chunk->crc, sizeof(chunk->crc));

	report_chunk(chunk);

	read_bytes(Input, &chunk->length, sizeof(chunk->length));
	reverse_bytes(&chunk->length, sizeof(chunk->length));
	
	read_bytes(Input, &chunk->type, sizeof(chunk->type));
	fseek(Input, chunk->length, SEEK_CUR);
	
	
	read_bytes(Input, &chunk->crc, sizeof(chunk->crc));
	reverse_bytes(&chunk->crc, sizeof(chunk->crc));

	report_chunk(chunk);



	// printf("Type -> 0x%08X\n", *(uint32_t*) chunk->type);

	printf("\n");
	
	/*
		IHDR *ihdr = ReadIHDR(Input);	

		printf("Read IHDR\n");
		PrintIHDR(ihdr);
		free(ihdr);
	*/

	free(chunk);
	fclose(Input);
	return 0;
}
