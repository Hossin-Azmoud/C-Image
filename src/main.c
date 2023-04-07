/* Made by Moody0101 (Hossin azmoud) */
#include "png.h"
#include <stdbool.h>

int main(int argc, char **argv)
{
	(void) argc;

#if 1
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
	
	if(memcmp(sig, PNG_SIG, PNG_SIG_CAP) != 0)
	{
		fprintf(stderr, "Invalid png file\n");
		fclose(Input);
		exit(1);
	}
	
	print_as("PNG header -> ", sig, PNG_SIG_CAP, HEX);
	
	IHDR *ihdr = malloc(sizeof(IHDR));
	Chunk *C = ReadChunk(Input, ihdr);
	
	report_chunk(C);
	printf("0x%08x\n",  *(uint32_t *)C->type);

	while(true)
	{
				
		C = ReadChunk(Input, ihdr);
		report_chunk(C);
		if(*(uint32_t *)C->type == IHDR_)
		{
			PrintIHDR(ihdr);
		}

		printf("0x%08x\n",  *(uint32_t *)C->type);
		
		if(*(uint32_t *)C->type == IEND_)
		{
			free(C);
			printf("Finished reading.\n");
			break;
		}
		

		free(C);
	}
	
	/*
		IHDR *ihdr = ReadIHDR(Input);	

		printf("Read IHDR\n");
		PrintIHDR(ihdr);
		free(ihdr);
	*/

	fclose(Input);
#endif
	return 0;
}


