# C-Image
Image manipulation scripts and function in C/python.

# Constants and enums
1. The Official PNG signiture to identify valid PNG Image.

```C
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

```
2. chunk types to identify the chunks.
```C
typedef enum CHUNK_TYPES {
	IEND_ = 0x444E4549, 
	IHDR_ = 0x52444849,
	IDAT_ = 0x54414449,
} CHUNK_TYPE;
```
3. Default capacities for reading bytes.
```C
#define TYPE_CAP     4
#define CHUNK_SIZE   1024
#define PNG_SIG_CAP  8
```
4. Enum Format for print functions (print as hex, ascii or integer).
```C
typedef enum Format {
	HEX = 0,
	ASCII,
	INT,
} Format;
 
```

# structures

1. structure to parse an individual chunk from a stream.
```C
typedef struct PNGChunk 
{   
	uint32_t length; 
	uint8_t  type[TYPE_CAP];
	uint32_t data[CHUNK_SIZE];
	uint32_t crc;

} Chunk; 
```
2. structure to decode an IHDR chunk.
```C 
typedef struct IHDR {
	uint32_t Width, Height; 
   	uint8_t BitDepth;
   	uint8_t ColorType;
   	uint8_t CompressionMethod;
   	uint8_t FilterMethod;
   	uint8_t InterlaceMethod;
} IHDR;
```

# Functions
- `print_byte(uint8_t byte, Format f)`: Print a byte with specified format (HEX, ASCII, INT).
- `print_as(const char *Label, uint8_t *buffer, size_t cap, Format f)`: print a buffer with specified format (HEX, ASCII, INT)
- `read_bytes(FILE *Stream, void *buff, size_t buff_cap)`: Load buff_cap bytes from stream into buff, on failure quit.
- `reverse_bytes(void *buf0, size_t buf_cap)`: reverse bytes (because 32bit integers as stored in opposite order)
- `*ReadIHDR(FILE *Stream)`: read then return IHDR structure from png file stream.
- `PrintIHDR(IHDR *ihdr)`: print IHDR chunk.
- `report_chunk(Chunk *C)`: report a given chunk's info.
- `*ReadChunk(FILE *Stream, IHDR *ihdr)`: read a chunk and if it is IHDR, Store its data in IHDR structure.
- `ReadIntoChunkBuffer(FILE *Stream, Chunk *CBuffer)`: Read a chunk into a preallocated buffer.

# Files
 (NOT DOCUMENTED)
# resources
Official Png format: [libpng docs](http://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html)
