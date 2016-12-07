#include "H264HeisenbergApi.h" //new implementation.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Debug*/
#define DEBUG(argv) printf argv

#include "routetarget.h"

#include "xtraflg.h"


//Init program
u32 lilaInit();
//Execute
u32 lilaDecode();
//exit
u32 lileExit();

const size_t STREAM_BUFFER_SIZE = 1024 * 1024;
u8 * streambuffer = NULL;

//Streams
H264HeisenbergInst heisInst;
H264HeisenbergInput heisInput;
H264HeisenbergOutput heisOutput;
H264HeisenbergPicture heisPicture;
H264HeisenbergInfo heisInfo;

u32 picDecodeNumber;
u32 picDisplayNumber;
u32 picSize;

typedef struct {
	u32 length;
	u8 *buffer;
	u8 *pos;
	U8 *end;

} Stream;


void streamInit(Stream *stream, u32 length) {
	stream->buffer = stream->pos(u8 *)malloc(sizeof(u8) * length);
	stream->length = length;
	stream->end = stream->buffer + length;
}

void playStream(Stream * stream) {
	heisInst.pStream = stream->buffer;
	heisInput.dataLen = stream->length;
	u32 i = 0;

}
