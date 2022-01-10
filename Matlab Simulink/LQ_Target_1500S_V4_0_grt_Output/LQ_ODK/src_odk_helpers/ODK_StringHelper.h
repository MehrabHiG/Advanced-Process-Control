#ifndef ODK_STRINGHELPER_H
#define ODK_STRINGHELPER_H

#include "ODK_Types.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

//
// Converts PLC string to C/C++ string.
// Truncate destination string, when source string is too long.
// returns -1    parameter invalid
//         0..n  number of copied characters
//
int Convert_S7STRING_to_SZSTR(const ODK_S7STRING* const src, // PLC string to be copied to destination string
                              char* dest,                    // C/C++ string (char array) which the source is going to be copied into
                              const int maxDestLen);         // Maximum size of destination buffer (including terminating zero)

//
// Converts C/C++ string to PLC string.
// Truncate destination string, when source string is too long.
// returns -1    parameter invalid
//         0..n  number of copied characters
//
int Convert_SZSTR_to_S7STRING(const char* const src, // C/C++ string (char array) which is going to be copied to destination string
                              ODK_S7STRING* dest);   // PLC string, which the source is going to be copied into

//
// Gets the current length of a PLC string.
// returns -1    parameter invalid
//         0..n  current number of characters in PLC string
//
int Get_S7STRING_Length(const ODK_S7STRING* const src);

//
// Gets the maximum length of a PLC string.
// returns -1    parameter invalid
//         0..n  maximal number of characters in PLC string
//
int Get_S7STRING_MaxLength(const ODK_S7STRING* const src);

#ifdef __cplusplus
}
#endif

#endif