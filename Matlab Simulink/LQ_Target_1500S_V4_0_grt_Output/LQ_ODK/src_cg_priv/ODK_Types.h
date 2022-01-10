/* 
 * This file is AUTO GENERATED - DO NOT MODIFY this file. 
 * This file contains the data types of ODK 1500S.
 *
 * File created by ODK_CodeGenerator version 205.100.101.18 
 * at Mon August 30 17:02:50 2021 
 */

#if !defined ODK_Types_H
#define ODK_Types_H

#include <stdint.h>


#ifdef ODK_TRACE_ON
  #include <string.h>
  #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
  #define ODK_TRACE(msg, ...) ODK_Trace(__FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#else
  #define ODK_TRACE(msg, ...)
#endif
#ifdef __cplusplus
extern "C"
{
#endif
void ODK_Trace(const char* p_szFileName, const int p_nLine, const char* p_szMsg, ...);
#ifdef __cplusplus
}
#endif

#define  ODK_SUCCESS         0x0000
#define  ODK_USER_ERROR_BASE 0xF000

#define  ODK_TRUE            1
#define  ODK_FALSE           0

typedef  double              ODK_DOUBLE;
typedef  float               ODK_FLOAT;
typedef  int64_t             ODK_INT64;
typedef  int32_t             ODK_INT32;
typedef  int16_t             ODK_INT16;
typedef  int8_t              ODK_INT8;
typedef  uint64_t            ODK_UINT64;
typedef  uint32_t            ODK_UINT32;
typedef  uint16_t            ODK_UINT16;
typedef  uint8_t             ODK_UINT8;
typedef  uint64_t            ODK_LWORD;
typedef  uint32_t            ODK_DWORD;
typedef  uint16_t            ODK_WORD;
typedef  uint8_t             ODK_BYTE;
typedef  uint8_t             ODK_BOOL;
typedef  int64_t             ODK_LTIME;
typedef  int32_t             ODK_TIME;
typedef  uint64_t            ODK_LDT;
typedef  uint64_t            ODK_LTOD;
typedef  uint32_t            ODK_TOD;
typedef  uint8_t             ODK_S7STRING;
typedef  char                ODK_CHAR;
typedef  uint16_t            ODK_RESULT;

#pragma pack(push,1)
typedef struct ODK_DTL_s
{
  ODK_UINT16      Year;
  ODK_UINT8       Month;
  ODK_UINT8       Day;
  ODK_UINT8       Weekday;
  ODK_UINT8       Hour;
  ODK_UINT8       Minute;
  ODK_UINT8       Second;
  ODK_UINT32      Nanosecond;
} ODK_DTL;

// CLASSIC_DB
typedef struct ODK_CLASSIC_DB_s
{
  ODK_UINT32    Len;
  ODK_UINT8     Data[1];
} ODK_CLASSIC_DB;
#pragma pack(pop)



#pragma pack(push,1)
typedef struct
{
  ODK_DOUBLE Controlinput[2];
  ODK_DOUBLE Feedback[5];
  ODK_DOUBLE LQ_Refthickness;
  ODK_DOUBLE LQ_Refrollingforce;
}InternalModelSignals;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct
{
  ODK_DOUBLE Lr[4]; // [LQ Controller 1/Gain Lr]:LQ_P.Lr
  ODK_DOUBLE Ly[10]; // [LQ Controller 1/Gain Ly]:LQ_P.Ly
}Params;
#pragma pack(pop)

#endif // ODK_Types_H