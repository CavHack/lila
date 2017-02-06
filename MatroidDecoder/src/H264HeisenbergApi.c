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

/*------------------------------------------------------------------------------
 Table of contents
 1. Include headers
 2. External compiler flags
 3. Module defines
 4. Local function prototypes
 5. Functions
 H264HeisenbergInit
 H264HeisenbergGetInfo
 H264HeisenbergRelease
 H264HeisenbergDecode
 H264HeisenbergGetAPIVersion
 H264HeisenbergNextPicture
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 1. Include headers
 ------------------------------------------------------------------------------*/

#include<stdlib.h>
#include<string.h>
#include "basetype.h"
#include "h264BSDContainer.h"
#include "H264HeisenbergApi.h"
#include "H264BSDDecoder.h"
#include "h264BSDUTIL.h"

#define UNUSED(x) (void)(x)

/*---------------------------------------------------------------------
2. Versioning
----------------------------------------------------------------------*/

#define H264HEISENBERG_MAJOR_VERSION 1
#define H264HEISENBERG_MINOR_VERSION 1

/*------------------------------------------------------------------------
3.MODULARITY
--------------------------------------------------------------------------*/

#ifdef H264HEISENBERG_TRACE
#include <stdio.h>
#define HEISENBERG_API_TRC(str)  H264HeisenbergTrace(str)
#else
#define HEISENBERG_API_TRC(str)
#endif

#ifdef H264HEISENBERG_EVALUATION
#define H264HEISENBERG_EVALUATION_LIMIT 500
#endif

void H264HeisenbergTrace(char *string) {
    UNUSED(string);
}



void* H264HeisenbergMalloc(u32 size) {

    return malloc(size);

}

void H264HeisenbergFree(void *ptr, i32 value, u32 count){
    memset(ptr, value, count);

}

/*------------------------------------------------------------------------------
 Function: H264HeisenbergInit()
 Functional description:
 Initialize decoder software. Function reserves memory for the
 decoder instance and calls h264BSDInit to initialize the
 instance data.
 Inputs:
 noOutputReordering  flag to indicate decoder that it doesn't have
 to try to provide output pictures in display
 order, saves memory
 Outputs:
 heisenbergInst             pointer to initialized instance is stored here
 Returns:
 H264HEISENBERG_OK        successfully initialized the instance
 H264HEISENBERG_INITFAIL  initialization failed
 H264HEISENBERG_PARAM_ERR invalid parameters
 H264HEISENBERG_MEM_FAIL  memory allocation failed
 ------------------------------------------------------------------------------*/

H264HeisenbergRet H264HeisenbergInit(H264HeisenbergInst *decInst, u32 noOutputReordering)
{
    u32 rv = 0;
    
    heisenbergContainer_t *pHeisenbergCont;
    
    HEISENBERG_API_TRC("H264HeisenbergInit#");
    
    /* check that right shift on negative numbers is performed signed */
    /*lint -save -e* following check causes multiple lint messages */
    if ( ((-1)>>1) != (-1) )
    {
        DEC_API_TRC("H264HeisenbergInit# ERROR: Right shift is not signed");
        return(H264SWDEC_INITFAIL);
    }
    /*lint -restore */
    
    if (heisenbergInst == NULL)
    {
        HEISENBERG_API_TRC("H264HeisenbergInit# ERROR: decInst == NULL");
        return(H264HEISENBERG_PARAM_ERR);
    }
    
    pHeisenbergCont = (heisenbergContainer_t *)H264HeisenbergMalloc(sizeof(heisenbergContainer_t));
    
    if (pHeisenbergCont == NULL)
    {
        HEISENBERG_API_TRC("H264HeisenbergInit# ERROR: Memory allocation failed");
        return(H264HEISENBERG_MEMFAIL);
    }
    
#ifdef H264HEISENBERG_TRACE
    sprintf(pHeisenbergCont->str, "H264HeisenbergInit# decInst %p noOutputReordering %d",
            (void*)decInst, noOutputReordering);
    HEISENBERG_API_TRC(pDecCont->str);
#endif
    
    rv = h264BSDInit(&pHeisenbergCont->storage, noOutputReordering);
    if (rv != HANTRO_OK)
    {
        H264HeisenbergRelease(pHeisenbergCont);
        return(H264HEISENBERG_MEMFAIL);
    }
    
    pHeisenbergCont->heisenbergStat  = INITIALIZED;
    pHeisenbergCont->picNumber = 0;
    
#ifdef H264HEISENBERG_TRACE
    sprintf(pHeisenbergCont->str, "H264HeisenbergInit# OK: return %p", (void*)pDHeisenbergCont);
    HEISENBERG_API_TRC(pHeisenbergCont->str);
#endif
    
    *heisenbergInst = (heisenbergContainer_t *)pHeisenbergCont;
    
    return(H264HEISENBERG_OK);
    
}






























