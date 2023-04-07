#include "png.h"

void  print_byte(uint8_t byte, Format f)
{
	if(f == HEX)
	{
		printf(" %x ", byte);
		return;
	}
	
	if(f == ASCII)
	{
		if(isalnum(byte)) {
			printf("%c", byte);
			return;
		}

		print_byte(byte, HEX);
	}
}

void  print_as(const char *Label, uint8_t *buffer, size_t cap, Format f)
{
	if(!f) f = HEX;

	printf("%s : ", Label);
	
	for(size_t i = 0; i < cap; i++)
	{
		uint8_t v = buffer[i]; 
		print_byte(v, f);
	}

	printf("\n");

}

void  read_bytes(FILE *Stream, void *buff, size_t buff_cap)
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

void  reverse_bytes(void *buf0, size_t buf_cap)
{
    uint8_t *buf = buf0;
    for (size_t i = 0; i < buf_cap/2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[buf_cap - i - 1];
        buf[buf_cap - i - 1] = t;
    }
}

IHDR  *ReadIHDR(FILE *Stream)
{
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

void  PrintIHDR(IHDR *ihdr)
{
	printf("width:             %i\n", ihdr->Width);
	printf("Height:            %i\n", ihdr->Height); 
   	printf("BitDepth:          %i\n", ihdr->BitDepth);
   	printf("ColorType:         %i\n", ihdr->ColorType);
   	printf("CompressionMethod: %i\n", ihdr->CompressionMethod);
   	printf("FilterMethod:      %i\n", ihdr->FilterMethod);
   	printf("InterlaceMethod:   %i\n", ihdr->InterlaceMethod);
}



void  report_chunk(Chunk *C)
{   
	printf("\n");	
	printf("Chunk size : %i\n", C->length);
 	print_as("Chunk Type", C->type, sizeof(C->type), ASCII);	
	printf("Data       : SKIPPED (TODO)\n");
	printf("CRC        : %i\n", C->crc);
}

Chunk *ReadChunk(FILE *Stream, IHDR *ihdr)
{
	Chunk *chunk = malloc(sizeof(Chunk));
	read_bytes(Stream, &chunk->length, sizeof(chunk->length));
	reverse_bytes(&chunk->length, sizeof(chunk->length));
	
	read_bytes(Stream, &chunk->type, sizeof(chunk->type));
//	reverse_bytes(&chunk->length, sizeof(chunk->length));
	
	if(*(uint32_t *)chunk->type == IHDR_)
	{
		*ihdr = *ReadIHDR(Stream);
	} else {
		fseek(Stream, chunk->length, SEEK_CUR);
	}
	
	read_bytes(Stream, &chunk->crc, sizeof(chunk->crc));
   //reverse_bytes(&chunk->crc, sizeof(chunk->crc));

	return chunk;
}

void  ReadIntoChunkBuffer(FILE *Stream, Chunk *CBuffer)
{
	read_bytes(Stream, &CBuffer->length, sizeof(CBuffer->length));
	reverse_bytes(&CBuffer->length, sizeof(CBuffer->length));
	
	read_bytes(Stream, &CBuffer->type, sizeof(CBuffer->type));
	
	fseek(Stream, CBuffer->length, SEEK_CUR);
	
	read_bytes(Stream, &CBuffer->crc, sizeof(CBuffer->crc));
	reverse_bytes(&CBuffer->crc, sizeof(CBuffer->crc));
}



