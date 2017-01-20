/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "H264HeisenbergApi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL/SDL.h"

//Memory Allocation arithmetic: Calls to H264HeisenMalloc

//Debug prints
#define DEBUG(argv) //printf argv

/*CVS tag name for identification*/

const char tagName[256] = "$Name: FIRST_ANDROID_COPYRIGHT $";


//Define packets
u32 NextPacket (u8 **pStrm);
//crop picture and partition into u8 blocks
u32 CropPicture(u8 *pOutImage, u8 *pInImage,
		u32 picWidth, u32 picHeight, CropParams *pCropParams);

/*Global variables for stream handling*/

u8 *streamStop = NULL;
u32 packetsize = 0;
u32 nalUnitStream = 0;
FILE *foutput = NULL;

#define RENDER 1
void DrawOutput(u8 *data, u32 picWidth, u32 picHeight);
SDL_Surface* screen = NULL;

//Main loop handling

enum mainLoopStatus {

  MLS_STOP = 0,
  MLS_CONTINUE = 1,
  MLS_FRAMERENDERED = 2

};

//Native main loop differs from JavaScript asynchronous parsing.
void runMainLoop();
int mainLoopIteration();


#ifdef SOC_DESIGNER

//Initialisation function defined in InitCache.s
extern void cache_init(void);

//Push the initialization to the native end of C.
int $Sub$$main(char argc, char * argv[]) {

  cache_init(); //set up extra cache
 
  return $Super$$main(argc, argv) //call original function
    
}
#endif

/*---------------------------------------------------------------------------------------------------------
 
 Function name: main
 
 Purpose: 
 
    main function of decoder testbench. Provides command line interface with File I/O for H.264 decoder.
    Prints out the usage information when executed without arguments.
 
 
 
 -----------------------------------------------------------------------------------------------------------*/
 


u32 i, tmp;
u32 maxNumPics =0;
u8 *imageData;
u8 *tmpImage = NULL;
u32 strmLen;
u32 picSize;
H264
