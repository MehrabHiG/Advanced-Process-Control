#include "ODK_CpuReadData.h"

//
// ODK_CpuReadData.cpp, implementation of the CODK_CpuReadData class
//
// The read-only data access helper class serves as a wrapper to the input 
// buffer passed into the Execute function. You can use functions in this 
// class to access the data in the input buffer as STEP 7 data types.
// It takes the raw input from the ODK Module Execute function and provides 
// structured access to the input data area.  Multibyte values are converted 
// from little endian to big endian as necessary.  S7 data types are 
// converted to C++ data types in the read operation.  This class provides 
// only Read access to the data area.
//

//
// CODK_CpuReadData : Class constructor, initializes the input data area to 0
//
CODK_CpuReadData::CODK_CpuReadData() noexcept
    : m_nBytes(0), 
      m_data(0)
{
}

//
// CODK_CpuReadData : Class constructor. Initializes the output data area
//
CODK_CpuReadData::CODK_CpuReadData(const ODK_CLASSIC_DB *db) // Pointer to a classic db structure
    : m_nBytes(0), 
      m_data(0)
{
    SetBuffer(db->Len, (ODK_BYTE *)db->Data);
}

//
// CODK_CpuReadData : Class constructor, initializes the input data area
//
CODK_CpuReadData::CODK_CpuReadData(long      nBytes, // Size, in bytes, of the data buffer
                                   ODK_BYTE *data)   // Pointer to the data buffer
    : m_nBytes(0), 
      m_data(0)								   
{
    SetBuffer(nBytes, data);
}

//
// ~CODK_CpuReadData : Class destructor
//
CODK_CpuReadData::~CODK_CpuReadData()
{
}

//
// ODK_SetBuffer : Initializes the input data area and data size
//
void CODK_CpuReadData::SetBuffer(long     nBytes, 
                                 ODK_BYTE data[])
{
    if (0 == data)
    {
        m_nBytes = 0;
    }
    else
    {
        m_nBytes = nBytes;
    }
    m_data = data;
}

//
// ReadUINT8 : Reads a generic 8-bit value from the data area (used by public read methods)
//
bool CODK_CpuReadData::ReadUINT8(const long  byteOffset,   // Buffer index to the data
                                 ODK_UINT8  &value) const  // Variable to store the read value
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT8) > m_nBytes))
    {
        return false;
    }

    // Read, convert, and return the data
    value = reinterpret_cast<ODK_UINT8&>(m_data[byteOffset]);

    return true;
}

//
// ReadUINT16 : Reads a generic 16-bit value from the data area. Used by public read methods
//
bool CODK_CpuReadData::ReadUINT16(const long  byteOffset,  // Buffer index to the data
                                  ODK_UINT16 &value) const // Variable to store the read value
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT16) > m_nBytes))
    {
        return false;
    }

    // Read, convert, and return the data
    ODK_UINT16 word = reinterpret_cast<ODK_UINT16&>(m_data[byteOffset]);
    value = (((word & 0x00FFU) << 8) |
             ((word & 0xFF00U) >> 8));

    return true;
}

//
// ReadUINT32 : Reads a generic 32-bit value from the data area. Used by public read methods
//
bool CODK_CpuReadData::ReadUINT32(const long   byteOffset, // Buffer index to the data
                                  ODK_UINT32 &value) const // Variable to store the read value
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT32) > m_nBytes))
    {
        return false;
    }

    // Read, convert, and return the data
    ODK_UINT32 dword = reinterpret_cast<ODK_UINT32&>(m_data[byteOffset]);
    value = (((dword & 0x000000FFUL) << 24) |
             ((dword & 0x0000FF00UL) <<  8) |
             ((dword & 0x00FF0000UL) >>  8) |
             ((dword & 0xFF000000UL) >> 24));

    return true;
}

//
// ReadUINT64 : Reads a generic 64-bit value from the data area. Used by public read methods
//
bool CODK_CpuReadData::ReadUINT64(const long  byteOffset,  // Buffer index to the data
                                  ODK_UINT64 &value) const // Variable to store the read value
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT64) > m_nBytes))
    {
        return false;
    }

    // Read, convert, and return the data
    ODK_UINT64 qword = reinterpret_cast<ODK_UINT64&>(m_data[byteOffset]);
    value = (((qword & 0x00000000000000FFULL) << 56) |
             ((qword & 0x000000000000FF00ULL) << 40) |
             ((qword & 0x0000000000FF0000ULL) << 24) |
             ((qword & 0x00000000FF000000ULL) <<  8) |
             ((qword & 0x000000FF00000000ULL) >>  8) |
             ((qword & 0x0000FF0000000000ULL) >> 24) |
             ((qword & 0x00FF000000000000ULL) >> 40) |
             ((qword & 0xFF00000000000000ULL) >> 56));

    return true;
}

//
// ReadS7BOOL : Reads a boolean value from the data area
//
bool CODK_CpuReadData::ReadS7BOOL(const long  byteOffset,  // Buffer index to the data
                                  int         bitNo,       // Bit number within the byte to read
                                  bool       &value) const // Variable to store the read value
{
    ODK_UINT8 temp;

    // Verify valid bit index
    if ((bitNo < 0) || (bitNo > 7))
    {
        return false;
    }

    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT8) > m_nBytes))
    {
        return false;
    }

    // read the byte containing the bit
    if (!ReadUINT8(byteOffset, temp))
    {
        return false;
    }

    // Shift the bit to the first location
    temp = static_cast<ODK_UINT8>(temp >> bitNo);

    // Isolate the value
    if ((temp & 1) == 1)
    {
        value = true;
    }
    else
    {
        value = false;
    }
    return true;
}

//
// ReadS7S5TIME : Reads a 16-bit (2 bytes) time value
//
bool CODK_CpuReadData::ReadS7S5TIME(const long  byteOffset,  // Offset to begin reading
                                    ODK_UINT16 &value) const // 16-bit time value read
{
    if (!ReadUINT16(byteOffset, value))
    {
        return false;
    }

    return CheckS7S5Time(value);
}

//
// ReadS7TIME_OF_DAY : Reads the time of day (4 bytes) from the data area
//
bool CODK_CpuReadData::ReadS7TIME_OF_DAY(const long  byteOffset,  // Offset to begin reading
                                         ODK_UINT32 &value) const // 32-bit time of day value read
{
    if (!ReadUINT32(byteOffset, value))
    {
        return false;
    }

    return (sm_maxTOD >= value);
}

//
// ReadS7LTIME_OF_DAY : Reads the time of day (8 bytes) as nanoseconds since midnight from the data area
//
bool CODK_CpuReadData::ReadS7LTIME_OF_DAY(const long  byteOffset,  // Offset to begin reading
                                          ODK_UINT64 &value) const // 64-bit long time of day value read
{
    if (!ReadUINT64(byteOffset, value))
    {
        return false;
    }

    return (sm_maxLTOD >= value);
}

//
// ReadS7DATE_AND_TIME : Reads a Date and Time in BCD format from the data area
//
bool CODK_CpuReadData::ReadS7DATE_AND_TIME(long        byteOffset,  // Offset to begin reading
                                           ODK_UINT64 &value) const // Date and time in BCD format read
{
    if (!ReadUINT64(byteOffset, value))
    {
        return false;
    }

	return CheckS7DateAndTime(value);
}

//
// ReadS7DTL : Reads a Date and Time as DTL from the data area
//
bool CODK_CpuReadData::ReadS7DTL(long     byteOffset, // Offset to begin reading
                                 ODK_DTL &dtl) const  // DTL structure read
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + static_cast<long>(sizeof(dtl))) > m_nBytes))
    {
        return false;
    }

    // year
    ReadUINT16(byteOffset, dtl.Year);
    byteOffset += ODK_SIZEBIT16;
    // month
    ReadUINT8(byteOffset, dtl.Month);
    byteOffset += ODK_SIZEBIT8;
    // day
    ReadUINT8(byteOffset, dtl.Day);
    byteOffset += ODK_SIZEBIT8;
    // weekday
    ReadUINT8(byteOffset, dtl.Weekday);
    byteOffset += ODK_SIZEBIT8;
    // hour
    ReadUINT8(byteOffset, dtl.Hour);
    byteOffset += ODK_SIZEBIT8;
    // minute
    ReadUINT8(byteOffset, dtl.Minute);
    byteOffset += ODK_SIZEBIT8;
    // second
    ReadUINT8(byteOffset, dtl.Second);
    byteOffset += ODK_SIZEBIT8;
    // nanosecond
    ReadUINT32(byteOffset, dtl.Nanosecond);

	return CheckS7DTL(dtl);
}

//
// ReadS7STRING_LEN : Reads the string length information for an S7 String in the data area
//
bool CODK_CpuReadData::ReadS7STRING_LEN(const long  byteOffset,   // Buffer index to the data
                                        ODK_UINT8  &maxLen,       // Variable to store the maximum length of the string
                                        ODK_UINT8  &curLen) const // Variable to store the current length of the string
{
    if (ReadUINT8(byteOffset, maxLen)  &&  ReadUINT8(byteOffset + sizeof(ODK_CHAR), curLen))
    {
        return true;
    }

    return false;
}

//
// ReadS7STRING : Reads an S7 string from the data area, and returns it as a C++ character string
//                Truncate destination string, when source string is too long.
//
bool CODK_CpuReadData::ReadS7STRING(const long  byteOffset,   // Buffer index to the data
                                    ODK_UINT8   readMax,      // Maximum characters to read including the string terminating zero
                                    ODK_CHAR*   string) const // Buffer to receive the string characters
{
    ODK_UINT8  curLen  = 0; // Current length of the string
    ODK_UINT8  maxLen  = 0; // Maximum length of the string
    ODK_UINT8  stop    = 0; // Maximum bytes to read
    ODK_UINT16 strData = static_cast<ODK_UINT16>(byteOffset + ODK_SIZEBIT16); // Offset to the string data (characters)

    // must receive a valid buffer for the string  AND  must have space for 1 character and the string terminator
    if (0 == string  ||  readMax < 2)
    {
        return false;
    }

    // Get the current string length
    if (! ReadS7STRING_LEN(byteOffset, maxLen, curLen))
    {
        return false;
    }

    // Verify offset not out of bounds
    if ((byteOffset < 0) || (byteOffset + static_cast<long>(maxLen) + ODK_SIZEBIT16 > m_nBytes))
    {
        return false;
    }

    // Get the value to read to
    if (curLen < readMax)
    {
        stop = curLen;
    }
    else
    {
        stop = static_cast<ODK_UINT8>(readMax - 1);
    }

    // Read the value
    memcpy(string, &(m_data[strData]), stop);
    string[stop] = '\0';  // terminate the string

    return true;
}

// return true, when DTL is in valid range
// (static)
bool CODK_CpuReadData::CheckS7DTL(const ODK_DTL& dtl)
{
	// year
    if (1970 > dtl.Year  ||  2262 < dtl.Year)
    {
        return false;
    }
    // month
    if (1 > dtl.Month  ||  12 < dtl.Month)
    {
        return false;
    }
    // day
    if (1 > dtl.Day  ||  31 < dtl.Day)
    {
        return false;
    }
    // weekday
    if (1 > dtl.Weekday  ||  7 < dtl.Weekday)
    {
        return false;
    }
    // hour
    if (23 < dtl.Hour)
    {
        return false;
    }
    // minute
    if (59 < dtl.Minute)
    {
        return false;
    }
    // second
    if (59 < dtl.Second)
    {
        return false;
    }
    // nanosecond
    if (999999999 < dtl.Nanosecond)
    {
        return false;
    }

    return true;
}

// return true, when S5Time is in valid range
bool CODK_CpuReadData::CheckS7S5Time(const ODK_UINT16& value) const
{
    ODK_INT8 bcd;

    // check time value
    bcd = GetBCD(value & 0xFF);
    if (bcd < 0  ||  bcd > 99) return false;
    bcd = GetBCD((value >> 8) & 0x0F);
    if (bcd < 0  ||  bcd > 9) return false;
    // check time base is always valid (two bits, four states)

    return true;
}

// return true, when DT is in valid range
bool CODK_CpuReadData::CheckS7DateAndTime(const ODK_UINT64& value) const
{
    ODK_INT8 bcd;

    // check year
    bcd = GetBCD((value >> 56) & 0xFF);
    if (bcd < 0  ||  bcd > 99) return false;
    // check month
    bcd = GetBCD((value >> 48) & 0xFF);
    if (bcd < 1 || bcd > 12) return false;
    // check day
    bcd = GetBCD((value >> 40) & 0xFF);
    if (bcd < 1 || bcd > 31) return false;
    // check hour
    bcd = GetBCD((value >> 32) & 0xFF);
    if (bcd < 0 || bcd > 23) return false;
    // check minute
    bcd = GetBCD((value >> 24) & 0xFF);
    if (bcd < 0 || bcd > 59) return false;
    // check second
    bcd = GetBCD((value >> 16) & 0xFF);
    if (bcd < 0 || bcd > 59) return false;
    // check millisecond
    bcd = GetBCD((value >> 8) & 0xFF);
    if (bcd < 0 || bcd > 99) return false;
    bcd = GetBCD((value >> 4) & 0x0F);
    if (bcd < 0 || bcd > 9) return false;
    // check weekday
    bcd = GetBCD(value & 0x0F);
    if (bcd < 1 || bcd > 7) return false;

    return true;
}

// return bcd number of value, or -1 when value is no BCD number
ODK_INT8 CODK_CpuReadData::GetBCD(const ODK_UINT8 value) const
{
    if ((value & 0x0F) > 9) return -1;
    if (((value >> 4) & 0x0F) > 9) return -1;

    return ((value >> 4) * 10) + (value & 0x0F);
}
