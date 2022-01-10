#include "ODK_StringHelper.h"

int Convert_S7STRING_to_SZSTR(const ODK_S7STRING* const src,  char* dest, const int maxDestLen)
{
  int currentLen = -1;
  if (NULL != src  &&  NULL != dest  &&  0 < maxDestLen  &&  src[0] >= src[1])
  {
    currentLen = src[1];
    if (currentLen >= maxDestLen)
    {
      currentLen = maxDestLen - 1;
    }
    memcpy(dest, &(src[2]), currentLen); 
    dest[currentLen] = '\0';
  }
  return currentLen;
}

int Convert_SZSTR_to_S7STRING(const char* const src, ODK_S7STRING* dest)
{
  int currentLen = -1;
  if (NULL != src  &&  NULL != dest  &&  dest[0] > 0)
  {
    currentLen = (int) strlen(src);
	if (currentLen > dest[0])
    {
      currentLen = dest[0];
    }
    memcpy(&(dest[2]), src, currentLen);
    dest[1] = (ODK_S7STRING) currentLen;
  }
  return currentLen;
}

int Get_S7STRING_Length(const ODK_S7STRING* const src)
{
  int len = -1;
  if (NULL != src  &&  src[0] >= src[1])
  {
    len = src[1];
  }
  return len;
}

int Get_S7STRING_MaxLength(const ODK_S7STRING* const src)
{
  int len = -1;
  if (NULL != src)
  {
    len = src[0];
  }
  return len;
}
