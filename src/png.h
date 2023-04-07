#ifndef _PNG
#define _PNG

#define TYPE_CAP     4
#define CHUNK_SIZE   1024
#define IEND_        0x444E4549
#define IHDR_        0x52444849
#define PNG_SIG_CAP  8

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

static const uint8_t PNG_SIG[PNG_SIG_CAP] = {
	137, 
	80, 
	78, 
	71, 
	13, 
	10, 
	26, 
	10
}; 

typedef enum Format {
	HEX = 0,
	ASCII,
} Format;

 
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

void  print_byte(uint8_t byte, Format f);
void  print_as(const char *Label, uint8_t *buffer, size_t cap, Format f);
void  read_bytes(FILE *Stream, void *buff, size_t buff_cap);
void  reverse_bytes(void *buf0, size_t buf_cap);
IHDR  *ReadIHDR(FILE *Stream);
void  PrintIHDR(IHDR *ihdr);
void  report_chunk(Chunk *C);
Chunk *ReadChunk(FILE *Stream, IHDR *ihdr);
void  ReadIntoChunkBuffer(FILE *Stream, Chunk *CBuffer);

#endif // _PNG
