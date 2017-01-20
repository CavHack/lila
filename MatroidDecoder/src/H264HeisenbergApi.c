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
#include "h264BSDContainer"
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

void H264HeisenbergTrace(char *string)
