/*------------------------------------------------------------------------------
    Table of contents
    1. Include Headers
    2. Enumerations used as a return value or a parameter.
        2.1. API's return value enumerations.
    3. User Structures
        3.1. Structures for H264SwDecDecode() parameters.
        3.2. Structures for information interchange with
             DEC API and user application.
    4. Prototypes of Decoder API functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Table of contents
    1. Include Headers
    2. Enumerations used as a return value or a parameter.
        2.1. API's return value enumerations.
    3. User Structures
        3.1. Structures for H264SwDecDecode() parameters.
        3.2. Structures for information interchange with
             DEC API and user application.
    4. Prototypes of Decoder API functions
------------------------------------------------------------------------------*/

#ifndef H264HEISENBERGAPI_H
#define H264HEISENBERGAPI_H

#ifdef __cplusplus
extern "C" {
#endif

//Headers

	#include "basetype.h"

//Return values for Heisenberg

	typedef enum{

	H264HEISENBERG_OK = 0,
	H264HEISENBERG_STRM_PROCESSED = 1, 
	H264HEISENBERG_PIC_RDY,
	H264HEISENBERG_PIC_RDY_BUFF_NOT_EMPTY,
	H264HEISENBERG_HDRS_RDY_BUFF_NOT_EMPTY,
	H264HEISENBERG_PARAM_ERR = -1,
	H264HEISENBERG_STRM_ERR = -2,
	H264HEISENBERG_NOT_INITIALIZED = -3,
	H264HEISENBERG_MEMFAIL = -4,
	H264HEISENBERG_INITFAIL = -5,
	H264HEISENBERG_HDRS_NOT_RDY = -6,
	H264HEISENBERG_EVALUATION_LIMIT_EXCEEDED = -7

} H264HeisenbergRet;

/*------------------------------------------------------------------------------
    3.1. Structures for H264HeisenbergDecode() parameters.
------------------------------------------------------------------------------*/

/*typedef of the decoder instance*/
typedef void *H264HeisenbergInst;

//input structure
typedef struct {
	u8 *pStream; //Pointer to stream to be decoded
	u32 dataLen; //Number of bytes to be decoded
	u32 picId; // Identifier for the picture to be decoded
	u32 intraConcealmentMethod; /* 0 = gray concealment for intra, 1 = ref concealment for intra*/


} H264HeisenbergInput;

/* Output structure */
    typedef struct
    {
        u8  *pStrmCurrPos;      /* Pointer to stream position where decoder
                                   ended up */
    } H264HeisenbergOutput;

    /* Output structure for H264SwDecNextPicture */
    typedef struct
    {
        u32 *pOutputPicture;    /* Pointer to the picture, YUV format       */
        u32 picId;              /* Identifier of the picture to be displayed*/
        u32 isIdrPicture;       /* Flag to indicate if the picture is an
                                   IDR picture */
        u32 nbrOfErrMBs;        /* Number of concealed MB's in the picture  */
    } H264HeisenbergPicture;

typedef struct
    {
        u32 profile;
        u32 picWidth;
        u32 picHeight;
        u32 videoRange;
        u32 matrixCoefficients;
        u32 parWidth;
        u32 parHeight;
        u32 croppingFlag;
        CropParams cropParams;
    } H264HeisenbergInfo;

    /* Version information */
    typedef struct
    {
        u32 major;    /* Decoder API major version */
        u32 minor;    /* Dncoder API minor version */
    } H264HeisenbergApiVersion;

/*------------------------------------------------------------------------------
    4. Prototypes of Decoder API functions
------------------------------------------------------------------------------*/

    H264HeisenbergRet H264HeisenbergDecode(H264HeisenbergInst      hInst,
                                 H264HeisenbergInput     *pInput,
                                 H264HeisenbergOutput    *pOutput);

    H264HeisenbergRet H264HeisenbergInit(H264HeisenbergInst *hInst,
                               u32            noOutputReordering);

    H264HeisenbergRet H264HeisenbergNextPicture(H264HeisenbergInst     hInst,
                                      H264HeisenbergNextPicture *pOutput,
                                      u32               endOfStream);

    H264HeisenbergRet H264HeisenbergGetInfo(H264HeisenbergInst decInst,
                                  H264HeisenbergInfo *pDecInfo);

    void  H264HeisenbergRelease(H264HeisenbergInst hInst);

    H264HeisenbergApiVersion H264HeisenbergGetAPIVersion(void);

    /* function prototype for API trace */
    void H264HeisenbergTrace(char *);

    /* function prototype for memory allocation */
    void* H264HeisenbergMalloc(u32 size);

    /* function prototype for memory free */
    void H264HeisenbergFree(void *ptr);

    /* function prototype for memory copy */
    void H264HeisenbergMemcpy(void *dest, void *src, u32 count);

    /* function prototype for memset */
    void H264HeisenbergMemset(void *ptr, i32 value, u32 count);


#ifdef __cplusplus
}
#endif

#endif /* H264HEISENBERGAPI_H */






