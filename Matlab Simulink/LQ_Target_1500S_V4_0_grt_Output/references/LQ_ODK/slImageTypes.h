/* Copyright 2020 The MathWorks, Inc. */

/**
 * @file: slImageTypes.h
 *  
 * @brief SimStruct types for Simulink Image operations
 *
 */

#ifndef __SIMSTRUC__IMAGETYPES
#define __SIMSTRUC__IMAGETYPES

#include "sl_types_def.h" /* DTypeId */

/* Enum to describe the ColorFormat attribute of an image */
typedef enum {
    SS_IMAGECOLORFORMAT_BGRA = 0,
    SS_IMAGECOLORFORMAT_RGB = 1,
    SS_IMAGECOLORFORMAT_RGBA = 2,
    SS_IMAGECOLORFORMAT_Grayscale = 3,
    SS_IMAGECOLORFORMAT_CMYK = 4,
    SS_IMAGECOLORFORMAT_Spectral = 5,
    SS_IMAGECOLORFORMAT_Lab = 6,
    SS_IMAGECOLORFORMAT_HSV = 7,
    SS_IMAGECOLORFORMAT_YUV444 = 8,
    SS_IMAGECOLORFORMAT_CFA = 9
} SSImageColorFormat;
                                                
/* Enum to indicate data layout of an image */
typedef enum {
    SS_IMAGEDATALAYOUT_COLMAJOR_PLANAR = 0,
    SS_IMAGEDATALAYOUT_ROWMAJOR_INTERLEAVED = 1,
} SSImageDataLayout;

/* Data type properties for image types */
typedef struct {
    size_t numChannels;
    SSImageColorFormat colorFormat;
    SSImageDataLayout dataLayout;
    BuiltInDTypeId baseType;
} ImageDataTypeProperties;

#endif
