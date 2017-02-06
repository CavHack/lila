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
 2. compiler flags
 3. Modularity
 4. Local function prototypes
 5. Functions
 h264BSDInterPrediction
 MvPrediction16x16
 MvPrediction16x8
 MvPrediction8x16
 MvPrediction8x8
 MvPrediction
 MedianFilter
 GetInterNeighbour
 GetPredictionMv
 ------------------------------------------------------------------------------*/

#include "opttarget.h"

/*------------------------------------------------------------------------------
 1. Include headers
 ------------------------------------------------------------------------------*/

#include "h264BSD_inter_prediction.h"
#include "h264BSD_neighbour.h"
#include "h264BSD_util.h"
#include "h264BSD_reconstruct.h"
#include "h264BSD_dpb.h"

/*------------------------------------------------------------------------------
 2.flags
 --------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 3. Modularity
 ------------------------------------------------------------------------------*/

typedef struct
{
    u32 available;
    u32 refIndex;
    mv_t mv;
} interNeighbour_t;

/*------------------------------------------------------------------------------
 4. Local function prototypes
 ------------------------------------------------------------------------------*/

static u32 MvPrediction16x16(mbStorage_t *pMb, mbPred_t *mbPred,
                             dpbStorage_t *dpb);
static u32 MvPrediction16x8(mbStorage_t *pMb, mbPred_t *mbPred,
                            dpbStorage_t *dpb);
static u32 MvPrediction8x16(mbStorage_t *pMb, mbPred_t *mbPred,
                            dpbStorage_t *dpb);
static u32 MvPrediction8x8(mbStorage_t *pMb, subMbPred_t *subMbPred,
                           dpbStorage_t *dpb);
static u32 MvPrediction(mbStorage_t *pMb, subMbPred_t *subMbPred,
                        u32 mbPartIdx, u32 subMbPartIdx);
static i32 MedianFilter(i32 a, i32 b, i32 c);

static void GetInterNeighbour(u32 sliceId, mbStorage_t *nMb,
                              interNeighbour_t *n, u32 index);
static void GetPredictionMv(mv_t *mv, interNeighbour_t *a, u32 refIndex);

static const neighbour_t N_A_SUB_PART[4][4][4] = {
    { { {MB_A,5}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,5}, {MB_A,7}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,5}, {MB_CURR,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,5}, {MB_CURR,0}, {MB_A,7}, {MB_CURR,2} } },
    
    { { {MB_CURR,1}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,1}, {MB_CURR,3}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,1}, {MB_CURR,4}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,1}, {MB_CURR,4}, {MB_CURR,3}, {MB_CURR,6} } },
    
    { { {MB_A,13}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,13}, {MB_A,15}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,13}, {MB_CURR,8}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,13}, {MB_CURR,8}, {MB_A,15}, {MB_CURR,10} } },
    
    { { {MB_CURR,9}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,9}, {MB_CURR,11}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,9}, {MB_CURR,12}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,9}, {MB_CURR,12}, {MB_CURR,11}, {MB_CURR,14} } } };

static const neighbour_t N_B_SUB_PART[4][4][4] = {
    { { {MB_B,10}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,10}, {MB_CURR,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,10}, {MB_B,11}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,10}, {MB_B,11}, {MB_CURR,0}, {MB_CURR,1} } },
    
    { { {MB_B,14}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,14}, {MB_CURR,4}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,14}, {MB_B,15}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,14}, {MB_B,15}, {MB_CURR,4}, {MB_CURR,5} } },
    
    { { {MB_CURR,2}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,2}, {MB_CURR,8}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,2}, {MB_CURR,3}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,2}, {MB_CURR,3}, {MB_CURR,8}, {MB_CURR,9} } },
    
    { { {MB_CURR,6}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,6}, {MB_CURR,12}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,6}, {MB_CURR,7}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,6}, {MB_CURR,7}, {MB_CURR,12}, {MB_CURR,13} } } };

static const neighbour_t N_C_SUB_PART[4][4][4] = {
    { { {MB_B,14}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,14}, {MB_NA,4}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,11}, {MB_B,14}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,11}, {MB_B,14}, {MB_CURR,1}, {MB_NA,4} } },
    
    { { {MB_C,10}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_C,10}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,15}, {MB_C,10}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,15}, {MB_C,10}, {MB_CURR,5}, {MB_NA,0} } },
    
    { { {MB_CURR,6}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,6}, {MB_NA,12}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,3}, {MB_CURR,6}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,3}, {MB_CURR,6}, {MB_CURR,9}, {MB_NA,12} } },
    
    { { {MB_NA,2}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_NA,2}, {MB_NA,8}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,7}, {MB_NA,2}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,7}, {MB_NA,2}, {MB_CURR,13}, {MB_NA,8} } } };

static const neighbour_t N_D_SUB_PART[4][4][4] = {
    { { {MB_D,15}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_D,15}, {MB_A,5}, {MB_NA,0}, {MB_NA,0} },
        { {MB_D,15}, {MB_B,10}, {MB_NA,0}, {MB_NA,0} },
        { {MB_D,15}, {MB_B,10}, {MB_A,5}, {MB_CURR,0} } },
    
    { { {MB_B,11}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,11}, {MB_CURR,1}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,11}, {MB_B,14}, {MB_NA,0}, {MB_NA,0} },
        { {MB_B,11}, {MB_B,14}, {MB_CURR,1}, {MB_CURR,4} } },
    
    { { {MB_A,7}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,7}, {MB_A,13}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,7}, {MB_CURR,2}, {MB_NA,0}, {MB_NA,0} },
        { {MB_A,7}, {MB_CURR,2}, {MB_A,13}, {MB_CURR,8} } },
    
    { { {MB_CURR,3}, {MB_NA,0}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,3}, {MB_CURR,9}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,3}, {MB_CURR,6}, {MB_NA,0}, {MB_NA,0} },
        { {MB_CURR,3}, {MB_CURR,6}, {MB_CURR,9}, {MB_CURR,12} } } };
