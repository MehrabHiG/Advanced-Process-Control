#include "ODK_CpuReadWriteData.h"

//
// ODK_CpuReadWriteData.cpp, implementation for the class CODK_CpuReadWriteData
//
// The read/write data access helper class serves as a wrapper to the output 
// buffer passed into the Execute function. You can use the functions in 
// this class to read and write STEP 7 data types to the output buffer.
// It takes the raw input from the ODK Module Execute function and provides 
// structured access to the output data area.  Multibyte values are 
// converted from little endian to big endian formats as necessary.  C++ 
// data types are converted to S7 data types in the write operation.  This 
// class provides write access and inherits read access (from CODK_CpuReadData) 
// to the data area.
//

//
// CODK_CpuReadWriteData : Class constructor, initializes the output data area to 0
//
CODK_CpuReadWriteData::CODK_CpuReadWriteData() noexcept
    : CODK_CpuReadData(),
      m_LowIndex(0),
      m_HighIndex(0)
{
}

//
// CODK_CpuReadWriteData : Class constructor. Initializes the output data area
//
CODK_CpuReadWriteData::CODK_CpuReadWriteData(const ODK_CLASSIC_DB *db) // Pointer to a classic db structure
    :m_LowIndex(0),
     m_HighIndex(0)
{
    SetBuffer(db->Len, (ODK_BYTE *)db->Data);
}

//
// CODK_CpuReadWriteData : Class constructor. Initializes the output data area
//
CODK_CpuReadWriteData::CODK_CpuReadWriteData(long     nBytes, // Number of bytes in the data buffer
                                             ODK_BYTE data[]) // Array pointer to the data buffer
    :m_LowIndex(0),
     m_HighIndex(0)											 
{
    SetBuffer(nBytes, data);
}

//
// ~CODK_CpuReadWriteData : Class destructor
//
CODK_CpuReadWriteData::~CODK_CpuReadWriteData()
{
}

//
// SetBuffer : Initializes the output data area and data size
//
void CODK_CpuReadWriteData::SetBuffer(long     nBytes, // Number of bytes in the data buffer
                                      ODK_BYTE data[]) // Array pointer to the data buffer
{
      if (0 == data)
      {
            m_nBytes = 0;
            m_LowIndex = 0;
            m_HighIndex = 0;
      }
      else
      {
            m_nBytes = nBytes;
            m_LowIndex = nBytes;
            m_HighIndex = -1;
      }
      m_data = data;
}

//
// WriteUINT8 : Writes a generic 8-bit value to the data area
//
bool CODK_CpuReadWriteData::WriteUINT8(const long      byteOffset, // Index into the data buffer
                                       const ODK_UINT8 value)      // Value to write into the buffer
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT8) > m_nBytes))
    {
        return false;
    }

    // Update highest index accessed
    if ((byteOffset + ODK_SIZEBIT8 - 1) > m_HighIndex)
    {
        m_HighIndex = byteOffset + ODK_SIZEBIT8 - 1;
    }

    // Update lowest index accessed
    if (byteOffset < m_LowIndex)
    {
        m_LowIndex = byteOffset;
    }

    // Convert and write the data
    m_data[byteOffset] = (ODK_UINT8)value; 

    return true;
}

//
// WriteUINT16 : Writes a generic 16-bit value to the data area
//
bool CODK_CpuReadWriteData::WriteUINT16(const long       byteOffset, // Index into the data buffer
                                        const ODK_UINT16 value)      // Value to write into the buffer
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT16) > m_nBytes))
    {
        return false;
    }

    // Update highest index accessed
    if ((byteOffset + ODK_SIZEBIT16 - 1) > m_HighIndex)
    {
        m_HighIndex = byteOffset + ODK_SIZEBIT16 - 1;
    }

    // Update lowest index accessed
    if (byteOffset < m_LowIndex)
    {
        m_LowIndex = byteOffset;
    }

    // Convert and write the data
    ODK_UINT16 word = (((value & 0x00FFU) << 8) |
                       ((value & 0xFF00U) >> 8));

    reinterpret_cast<ODK_UINT16&>(m_data[byteOffset]) = word;

    return true;
}

//
// WriteUINT32 : Writes a generic 32-bit value to the data area
//
bool CODK_CpuReadWriteData::WriteUINT32(const long       byteOffset, // Index into the data buffer
                                        const ODK_UINT32 value)      // Value to write into the buffer
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT32) > m_nBytes))
    {
        return false;
    }

    // Update highest index accessed
    if ((byteOffset + ODK_SIZEBIT32 - 1) > m_HighIndex)
    {
        m_HighIndex = byteOffset + ODK_SIZEBIT32 - 1;
    }

    // Update lowest index accessed
    if (byteOffset < m_LowIndex)
    {
        m_LowIndex = byteOffset;
    }

    // Convert and write the data
    ODK_UINT32 dword = (((value & 0x000000FFUL) << 24) |
                        ((value & 0x0000FF00UL) <<  8) |
                        ((value & 0x00FF0000UL) >>  8) |
                        ((value & 0xFF000000UL) >> 24));

    reinterpret_cast<ODK_UINT32&>(m_data[byteOffset]) = dword;

    return true;
}

//
// WriteUINT64 : Writes a generic 64-bit value to the data area
//
bool CODK_CpuReadWriteData::WriteUINT64(const long       byteOffset, // Index into the data buffer
                                        const ODK_UINT64 value)      // Value to write into the buffer
{
    // Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + ODK_SIZEBIT64) > m_nBytes))
    {
        return false;
    }

    // Update highest index accessed
    if ((byteOffset + ODK_SIZEBIT64 - 1) > m_HighIndex)
    {
        m_HighIndex = byteOffset + ODK_SIZEBIT64 - 1;
    }

    // Update lowest index accessed
    if (byteOffset < m_LowIndex)
    {
        m_LowIndex = byteOffset;
    }

    // Convert and write the data
    ODK_UINT64 qword = (((value & 0x00000000000000FFULL) << 56) |
                        ((value & 0x000000000000FF00ULL) << 40) |
                        ((value & 0x0000000000FF0000ULL) << 24) |
                        ((value & 0x00000000FF000000ULL) <<  8) |
                        ((value & 0x000000FF00000000ULL) >>  8) |
                        ((value & 0x0000FF0000000000ULL) >> 24) |
                        ((value & 0x00FF000000000000ULL) >> 40) |
                        ((value & 0xFF00000000000000ULL) >> 56));

    reinterpret_cast<ODK_UINT64&>(m_data[byteOffset]) = qword;

    return true;
}

//
// WriteS7BOOL : Writes a boolean value to the data area
//
bool CODK_CpuReadWriteData::WriteS7BOOL(const long         byteOffset, // Index into the data buffer
                                        const unsigned int bitNo,      // Number of the bit to write (index from right to left)
                                        const bool         value)      // Value of the bit to write
{
    // verify correct bit range
    if (bitNo > 7)
    {
        return false;
    }

    // Bit indexing: least significant bit is 0 (i.e. go right to left)

    ODK_UINT8 temp;  // variable for the byte's current value

    // Get the current byte so the other bit values can be maintained
    if (ReadUINT8(byteOffset, temp) == false)
    {
        return false;
    }

    // Set the bit in the byte to be written
    if (true == value)
    {
        temp = static_cast<ODK_UINT8>(temp | (1U << bitNo));
    }
    else
    {
        temp = static_cast<ODK_UINT8>(temp & (~(1U << bitNo)));
    }
    // Write the byte
    return WriteUINT8(byteOffset, temp);
}

//
// WriteS7S5TIME : Writes a 2 byte time value to the data area
//
bool CODK_CpuReadWriteData::WriteS7S5TIME(const long       byteOffset, // Offset to begin writing
                                          const ODK_UINT16 value)     // 16-bit unsigned value to write
{
    if (!CheckS7S5Time(value))
    {
        return false;
    }

    return WriteUINT16(byteOffset, value);
}

//
// WriteS7TIME_OF_DAY : Writes the time of day (4 bytes) to the data area
//
bool CODK_CpuReadWriteData::WriteS7TIME_OF_DAY(const long       byteOffset, // Offset to begin writing
                                               const ODK_UINT32 value)      // 32-bit unsigned value to write
{
    if (sm_maxTOD < value)
    {
        return false;
    }

    return WriteUINT32(byteOffset, value);
}

//
// WriteS7LTIME_OF_DAY : Writes the time of day (8 bytes) as nanoseconds since midnight to the data area
//
bool CODK_CpuReadWriteData::WriteS7LTIME_OF_DAY(const long       byteOffset, // Offset to begin writing
                                                const ODK_UINT64 value)      // 64-bit long time of day value to write
{
    if (sm_maxLTOD < value)
    {
        return false;
    }

    return WriteUINT64(byteOffset, value);
}

//
// WriteS7DATE_AND_TIME : Writes Date and Time in BCD format to the data area
//
bool CODK_CpuReadWriteData::WriteS7DATE_AND_TIME(long             byteOffset, // Index into the data buffer
                                                 const ODK_UINT64 &value)     // Date and time in BCD format to write
{
    if (!CheckS7DateAndTime(value))
	{
        return false;
    }

	return WriteUINT64(byteOffset, value);
}

//
// WriteS7DTL : Writes a Date and Time as DTL to the data area
//
bool CODK_CpuReadWriteData::WriteS7DTL(long          byteOffset, // Offset to begin reading
                                       const ODK_DTL &dtl)       // DTL structure to write
{
	// Verify offset is within data area
    if ((byteOffset < 0) || ((byteOffset + static_cast<long>(sizeof(dtl))) > m_nBytes))
    {
        return false;
    }

	// Check content of DTL
	if (!CheckS7DTL(dtl))
	{
        return false;
	}

    // year
    WriteUINT16(byteOffset, dtl.Year);
    byteOffset += ODK_SIZEBIT16;
    // month
    WriteUINT8(byteOffset, dtl.Month);
    byteOffset += ODK_SIZEBIT8;
    // day
    WriteUINT8(byteOffset, dtl.Day);
    byteOffset += ODK_SIZEBIT8;
    // weekday
    WriteUINT8(byteOffset, dtl.Weekday);
    byteOffset += ODK_SIZEBIT8;
    // hour
    WriteUINT8(byteOffset, dtl.Hour);
    byteOffset += ODK_SIZEBIT8;
    // minute
    WriteUINT8(byteOffset, dtl.Minute);
    byteOffset += ODK_SIZEBIT8;
    // second
    WriteUINT8(byteOffset, dtl.Second);
    byteOffset += ODK_SIZEBIT8;
    // nanosecond
    WriteUINT32(byteOffset, dtl.Nanosecond);

	return true;
}

//
// WriteS7STRING : Writes a string to the data area
//                 Truncate destination string, when source string is too long.
//
bool CODK_CpuReadWriteData::WriteS7STRING(const long      byteOffset, // Index into the data buffer
                                          const ODK_CHAR* string)     // Pointer to a string buffer containing the string to write
{
    ODK_UINT8 maxLen = 0;
    ODK_UINT8 curLen = 0;

    // check parameters and read maxLen
    if (0 == string
     || ! ReadUINT8(byteOffset, maxLen))
    {
        return false;
    }

    // get the current size of the string
    curLen = static_cast<ODK_UINT8>(strlen(string));

	// get the value to write to
    if (curLen > maxLen)
    {
        curLen = maxLen;
    }

    // verify offset not out of bounds
    if (static_cast<long>(byteOffset + curLen + (2 * sizeof(ODK_CHAR))) > m_nBytes)
    {
        return false;
    }

    // set the current length
    WriteUINT8(byteOffset + 1, curLen);

	// Copy the string
	memcpy(&(m_data[byteOffset + 2]), string, curLen);

    return true;
}
