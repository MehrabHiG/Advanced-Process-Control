/*
 * This file is AUTO GENERATED - DO NOT MODIFY this file. 
 * This file contains the execute function and trace helpers for ODK 1500S.
 *
 * File created by ODK_CodeGenerator version 205.100.101.18
 * at Mon August 30 17:02:50 2021
*/

#include "ODK_Functions.h"
#include "../src_odk_helpers/ODK_StringHelper.h"
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "ODK_CpuReadData.h"
#include "ODK_CpuReadWriteData.h"

#define ODK_COMMAND_NOT_IMPLEMENTED    0x8098 
#define MAX_LOG_ENTRY_LEN 				126
#define MAX_USER_TRACE_LEN				(100)
typedef struct TraceEntry_s
{
	ODK_UINT64  TickCounter;
	ODK_UINT16  OB_Number; 
	char        Text[MAX_USER_TRACE_LEN];
} TraceEntry_t;

#define ODK_TRACE_GET_TICK(x)  __asm__ __volatile__ ("rdtsc" : "=a" (((int*)(x))[0]), "=d"(((int*)(x))[1]))
#ifdef ODK_TRACE_ON
    #define MAX_NUMBER_OF_LOG_ENTRIES		(1 << 8) // must be 2^x (here 2^8=256)
    TraceEntry_t g_TraceBuffer[MAX_NUMBER_OF_LOG_ENTRIES] = {0, 0, {0}};
#else
	#define MAX_NUMBER_OF_LOG_ENTRIES		(1) // must be 1 trace only
	TraceEntry_t g_TraceBuffer[MAX_NUMBER_OF_LOG_ENTRIES] = {0, 0, "<trace off>"};
#endif 
volatile unsigned long g_idxNextFreeEntry = 0;
volatile unsigned short g_currentOBnumber = 0;
ODK_UINT64 g_tickResolutionPerSec = (ODK_UINT64) 1u;
ODK_UINT64 g_tickTimeBase = 0;
ODK_UINT64 g_TimeBaseSec = 0;
ODK_UINT32 g_TimeBaseNanos = 0;
ODK_UINT32 g_ODK1500sBuildVersion = (2 << 24) + (5 << 16) + (1 << 8) + 0;
ODK_UINT8  g_SyncCallParallelCount = 1;
ODK_UINT32 g_SyncCallStackSize = (32 * 1024);
ODK_UINT32 g_SyncCallDataSize = (1 * 1024);
ODK_UINT32 g_PostMortemExceptionNr = ~0;   // 0=division by zero, 14=page fault (see https://en.wikipedia.org/wiki/Interrupt_descriptor_table)

#ifndef _DEBUG
  const int g_DsoHeapSize  = (32 * 1024);
  const int g_DsoAdditionalHeapSize = (20 + 16);
  char      g_DsoHeapMemory [g_DsoHeapSize + g_DsoAdditionalHeapSize];
  const int g_DsoHeapMaxBlockSize   = 1024;
  const int g_DsoHeapFreeStackSize  = (((g_DsoHeapMaxBlockSize-1)>>3)+1) * 24;
  char      g_DsoHeapFreeStack [g_DsoHeapFreeStackSize];
  const int ODK_GetHeapSize()
  {
	return g_DsoHeapSize;
  }
  const int ODK_GetAdditionalHeapSize()
  {
	return g_DsoAdditionalHeapSize;
  }
  const int ODK_GetHeapMaxBlockSize()
  {
	return g_DsoHeapMaxBlockSize;
  }
  const int ODK_GetHeapFreeStackSize()
  {
	return g_DsoHeapFreeStackSize;
  }
#endif

 // first if

// declaration of the callbacks for testing them in Execute()
EXPORT_API ODK_RESULT OnLoad (void);
EXPORT_API ODK_RESULT OnUnload (void);
EXPORT_API ODK_RESULT OnRun (void);
EXPORT_API ODK_RESULT OnStop (void);

//command enums
typedef enum CommandHash_e
{
  FCT_HASH_OneStep = 0x11A44258,
  FCT_HASH_ReadWriteParameters = 0x7F6ED6DF
}CommandHash_t;

ODK_RESULT Execute ( const ODK_UINT32 cmd
                   , const char* const in
                   , const char*       inout
                   , const char*       out)
{
  switch (cmd)
  {
    case FCT_HASH_OneStep:
    {
      return OneStep ((ODK_DOUBLE*) &(in[0]), *((ODK_DOUBLE*) &(in[40])), *((ODK_DOUBLE*) &(in[48])), *((ODK_DOUBLE*) &(out[0])), *((ODK_DOUBLE*) &(out[8])), *((InternalModelSignals*) &(out[16])));
    }
    case FCT_HASH_ReadWriteParameters:
    {
      return ReadWriteParameters (*((ODK_BOOL*) &(in[0])), *((Params*) &(inout[0])));
    }
    default:
    {
      typedef ODK_RESULT (*t_CallbackFunction)(void);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wunused-variable"
      // check the existence of the callback functions
      // (but suppress warning of not used variables)
      {
        t_CallbackFunction tmp1 = &OnLoad;
        t_CallbackFunction tmp2 = &OnUnload;
        t_CallbackFunction tmp3 = &OnRun;
        t_CallbackFunction tmp4 = &OnStop;
      }
      #pragma GCC diagnostic pop
      return ODK_COMMAND_NOT_IMPLEMENTED;
    }
  }
}

typedef struct Internal_ExecuteRT_s
{
  ODK_UINT32    Command;
  char*        _IN;
  char*        _INOUT;
  char*        _OUT;
} Internal_ExecuteRT_t;

EXPORT_API int ExecuteRT(void* p_pMsg)
{
  Internal_ExecuteRT_t* pMsg = (Internal_ExecuteRT_t*) p_pMsg;
  return Execute (pMsg->Command, pMsg->_IN, pMsg->_INOUT, pMsg->_OUT);
}


int GetTraceTime(const ODK_UINT64* p_pTicks, char* p_str, const int p_maxLen)
{
  ODK_UINT64 tickDiff = *p_pTicks - g_tickTimeBase;
  ODK_UINT64 seconds = g_TimeBaseSec + (tickDiff / g_tickResolutionPerSec);
  ODK_UINT64 nanosecDiff = ((tickDiff % g_tickResolutionPerSec) * 1000000000L) / g_tickResolutionPerSec;
  ODK_UINT32 microsec = (g_TimeBaseNanos + (ODK_UINT32) nanosecDiff) / 1000;
  
  if (1000000 <= microsec)
  {
    seconds++;
    microsec -= 1000000;
  }
  time_t* pGmTime = (time_t*) &seconds;
  struct tm* pTraceTime = gmtime(pGmTime);
  
  if (p_maxLen >= 32)
  {
    return sprintf(p_str, "%d/%02d/%02d %.2d:%.2d:%.2d.%.6lu"
                 , pTraceTime->tm_year + 1900, pTraceTime->tm_mon + 1, pTraceTime->tm_mday
                 , pTraceTime->tm_hour, pTraceTime->tm_min, pTraceTime->tm_sec, microsec);
  }
  
  p_str[0] = 0;
  return 0;
}

ODK_RESULT GetTrace (const ODK_INT16& TraceCount, ODK_S7STRING TraceBuffer[256][127]) 
{
  ODK_UINT32 myIdx = g_idxNextFreeEntry - 1;
  ODK_INT16  myTraceCount = TraceCount;
  int filled = 0;

  if (myTraceCount > MAX_NUMBER_OF_LOG_ENTRIES ||  myTraceCount <= 0)
  {
	myTraceCount = MAX_NUMBER_OF_LOG_ENTRIES;
  }
  int _odk_internal_i = 0;
  // copy TraceCount trace entries
  for (; _odk_internal_i < myTraceCount; _odk_internal_i++)
  {
    char str[MAX_LOG_ENTRY_LEN] = "";
    int index = myIdx & (MAX_NUMBER_OF_LOG_ENTRIES - 1);
    #ifdef ODK_TRACE_ON
      if(g_TraceBuffer[index].TickCounter != 0)
      {
         filled = GetTraceTime(&(g_TraceBuffer[index].TickCounter), str, MAX_LOG_ENTRY_LEN);
      }
      else
      {
        // if tickcount is zero, do not write anything to buffer
        filled = MAX_LOG_ENTRY_LEN;
      }
    #endif
	snprintf(&(str[filled]), MAX_LOG_ENTRY_LEN - (int64_t)filled, " OB%d %s", g_TraceBuffer[index].OB_Number, g_TraceBuffer[index].Text);
	str[MAX_LOG_ENTRY_LEN - 1] = 0;
    Convert_SZSTR_to_S7STRING (str, TraceBuffer[_odk_internal_i]);
    myIdx--;
    filled = 0;
  }
  // fill empty strings for not wished trace entries
  for (; _odk_internal_i < 256; _odk_internal_i++)
  {
    Convert_SZSTR_to_S7STRING ("", TraceBuffer[_odk_internal_i]);
    myIdx--;
  }

  return myTraceCount;
}

// function for call from SFx (exception)
EXPORT_API void ODK_Trace_00(const char* p_szMsg)
{
  #ifdef ODK_TRACE_ON
  ODK_Trace("<unknown>", 0, p_szMsg);
  #endif
}
