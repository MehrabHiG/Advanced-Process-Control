#ifndef ODK_CPUREADDATA_H
#define ODK_CPUREADDATA_H

#include "ODK_Types.h"
#include <string.h>


//
// Declarations for the ODK_CpuReadData class
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

class CODK_CpuReadData
{
public:

    //
    // CODK_CpuReadData : Class constructor, initializes the input data area to 0
    //
    CODK_CpuReadData() noexcept;

    //
    // CODK_CpuReadData : Class constructor, initializes the input data area and data size
    //
    explicit CODK_CpuReadData(const ODK_CLASSIC_DB *db);  // Pointer to a classic db structure

    //
    // CODK_CpuReadData : Class constructor, initializes the input data area and data size
    //
    CODK_CpuReadData(long      nBytes, // Size, in bytes, of the data buffer
                     ODK_BYTE *data);  // Pointer to the data buffer

    //
    // ~CODK_CpuReadData : Class destructor
    //
    virtual ~CODK_CpuReadData();

    //
    // ODK_SetBuffer : Initializes the input data area and data size
    //
    virtual void SetBuffer(long     nBytes,  // Size, in bytes, of the data buffer
                           ODK_BYTE data[]); // Array pointer to the data buffer

    //
    // GetBufferSize : Returns the size of the data area (in bytes)
    //
    long GetBufferSize(void) const
    {
        return m_nBytes;
    }

    //
    // ReadS7BOOL : Reads a boolean value (1 bit) from the data area
    //
    bool ReadS7BOOL(const long  byteOffset,      // Offset to begin reading
                    int         bitNo,           // Bit index to read (bit 0 is least significant bit)
                    bool       &value) const;    // Boolean value read

    //
    // ReadS7BYTE : Reads a byte (1 byte) from the data area
    //
    bool ReadS7BYTE(const long  byteOffset,      // Offset to begin reading
                    ODK_UINT8  &value) const     // 8-bit unsigned value read
    {
        return ReadUINT8(byteOffset, value);
    }

    //
    // ReadS7WORD : Reads a word (2 bytes) from the data area
    //
    bool ReadS7WORD(const long  byteOffset,      // Offset to begin reading
                    ODK_UINT16 &value) const     // 16-bit unsigned value read
    {
        return ReadUINT16(byteOffset, value);
    }

    //
    // ReadS7DWORD : Reads a double word (4 bytes) from the data area
    //
    bool ReadS7DWORD(const long  byteOffset,     // Offset to begin reading
                     ODK_UINT32 &value) const    // 32-bit unsigned value read
    {
        return ReadUINT32(byteOffset, value);
    }

    //
    // ReadS7LWORD : Reads a long word (8 bytes) from the data area
    //
    bool ReadS7LWORD(const long  byteOffset,     // Offset to begin reading
                     ODK_UINT64 &value) const    // 64-bit unsigned value read
    {
        return ReadUINT64(byteOffset, value);
    }

    //
    // ReadS7SINT : Reads a short integer (1 byte) byte from the data area
    //
    bool ReadS7SINT(const long  byteOffset,      // Offset to begin reading
                    ODK_INT8   &value) const     // 8-bit integer read
    {
        return ReadUINT8(byteOffset, reinterpret_cast<ODK_UINT8&>(value));
    }

    //
    // ReadS7INT : Reads an integer (2 bytes) from the data area
    //
    bool ReadS7INT(const long  byteOffset,       // Offset to begin reading
                   ODK_INT16  &value) const      // 16-bit integer read
    {
        return ReadUINT16(byteOffset, reinterpret_cast<ODK_UINT16&>(value));
    }

    //
    // ReadS7DINT : Reads a double integer (4 bytes) from the data area
    //
    bool ReadS7DINT(const long  byteOffset,      // Offset to begin reading
                    ODK_INT32  &value) const     // 32-bit integer value read
    {
        return ReadUINT32(byteOffset, reinterpret_cast<ODK_UINT32&>(value));
    }

    //
    // ReadS7USINT : Reads an unsigned short integer (1 byte) from the data area
    //
    bool ReadS7USINT(const long   byteOffset,    // Offset to begin reading
                     ODK_UINT8   &value) const   // 8-bit integer read
    {
        return ReadUINT8(byteOffset, value);
    }

    //
    // ReadS7UINT : Reads an unsigned  integer (2 bytes) from the data area
    //
    bool ReadS7UINT(const long   byteOffset,     // Offset to begin reading
                    ODK_UINT16  &value) const    // 16-bit integer read
    {
        return ReadUINT16(byteOffset, value);
    }

    //
    // ReadS7UDINT : Reads a double integer (4 bytes) from the data area
    //
    bool ReadS7UDINT(const long   byteOffset,    // Offset to begin reading
                     ODK_UINT32  &value) const   // 32-bit integer value read
    {
        return ReadUINT32(byteOffset, value);
    }

    //
    // ReadS7REAL : Reads a real number (4 bytes) from the data area
    //
    bool ReadS7REAL(const long  byteOffset,      // Offset to begin reading
                    ODK_FLOAT  &value) const     // Real number read
    {
        return ReadUINT32(byteOffset, reinterpret_cast<ODK_UINT32&>(value));
    }

    //
    // ReadS7LREAL : Reads a real number (4 bytes) from the data area
    //
    bool ReadS7LREAL(const long  byteOffset,     // Offset to begin reading
                    ODK_DOUBLE  &value) const    // Real number read
    {
        return ReadUINT64(byteOffset, reinterpret_cast<ODK_UINT64&>(value));
    }

    //
    // ReadS7LINT : Reads an long integer (8 bytes) from the data area
    //
    bool ReadS7LINT(const long  byteOffset,      // Offset to begin reading
                   ODK_INT64  &value) const      // 16-bit integer read
    {
        return ReadUINT64(byteOffset, reinterpret_cast<ODK_UINT64&>(value));
    }

    //
    // ReadS7UINT : Reads an unsigned  integer (2 bytes) from the data area
    //
    bool ReadS7ULINT(const long   byteOffset,    // Offset to begin reading
                    ODK_UINT64  &value) const    // 16-bit integer read
    {
        return ReadUINT64(byteOffset, value);
    }

    //
    // ReadS7S5TIME : Reads a 16-bit (2 bytes) time value
    //
    bool ReadS7S5TIME(const long  byteOffset,    // Offset to begin reading
                      ODK_UINT16 &value) const;  // 16-bit time value read

    //
    // ReadS7TIME : Reads a time value (4 bytes) as milliseconds from the data area
    //
    bool ReadS7TIME(const long byteOffset,       // Offset to begin reading
                    ODK_TIME   &value) const     // 32-bit signed time value read
    {
        return ReadUINT32(byteOffset, reinterpret_cast<ODK_UINT32&>(value));
    }

	//
    // ReadS7LTIME : Reads a time duration (8 bytes) as nanoseconds from the data area
    //
    bool ReadS7LTIME(const long byteOffset,      // Offset to begin reading
                     ODK_LTIME  &value) const    // 64-bit signed time value read
    {
        return ReadUINT64(byteOffset, reinterpret_cast<ODK_UINT64&>(value));
    }

    //
    // ReadS7DATE : Reads a date value (2 bytes) from the data area
    //
    bool ReadS7DATE(const long  byteOffset,      // Offset to begin reading
                    ODK_UINT16 &value) const     // 16-bit date value read
    {
        return ReadUINT16(byteOffset, value);
    }

    //
    // ReadS7TIME_OF_DAY : Reads the time of day (4 bytes) from the data area
    //
    bool ReadS7TIME_OF_DAY(const long  byteOffset,      // Offset to begin reading
                           ODK_UINT32 &value) const;    // 32-bit time of day value read

    //
    // ReadS7LTIME_OF_DAY : Reads the time of day (8 bytes) as nanoseconds since midnight from the data area
    //
    bool ReadS7LTIME_OF_DAY(const long  byteOffset,     // Offset to begin reading
                            ODK_UINT64 &value) const;   // 64-bit long time of day value read

    //
    // ReadS7DATE_AND_TIME : Reads a Date and Time in BCD format from the data area
    //
    bool ReadS7DATE_AND_TIME(long        byteOffset,    // Offset to begin reading
                             ODK_UINT64 &value) const;  // Date and time in BCD format read

    //
    // ReadS7DATE_AND_LTIME : Reads a Date and Time as nanoseconds from the data area
    //
    bool ReadS7DATE_AND_LTIME(long        byteOffset,   // Offset to begin reading
                              ODK_UINT64 &value) const  // Date and time in nanoseconds read
    {
        return ReadUINT64(byteOffset, value);
	}

    //
    // ReadS7DTL : Reads a Date and Time as DTL from the data area
    //
    bool ReadS7DTL(long     byteOffset,       // Offset to begin reading
                   ODK_DTL &dtl) const;       // DTL structure read

    //
    // ReadS7CHAR : Reads a character (1 byte) from the data area
    //
    bool ReadS7CHAR(const long  byteOffset,   // Offset to begin reading
                    ODK_CHAR   &value) const  // Character read
    {
        return ReadUINT8(byteOffset, reinterpret_cast<ODK_UINT8&>(value));
    }

    //
    // ReadS7STRING_LEN : Reads the string length information for an S7 String in the data area
    //
    bool ReadS7STRING_LEN(const long  byteOffset,    // Offset to begin reading
                          ODK_UINT8  &maxLen,        // Maximum length of the string
                          ODK_UINT8  &curLen) const; // Current length of the string

    //
    // ReadS7STRING : Reads an S7 string from the data area, and returns it as a C++ character string
	//                Truncate destination string, when source string is too long.
    //
    bool ReadS7STRING(const long  byteOffset,    // Offset to begin reading
                      ODK_UINT8   readMax,       // Maximum number of characters to read (including terminating zero)
                      ODK_CHAR*   string) const; // Pointer to a buffer to hold the string

	static const ODK_UINT32 sm_maxTOD = 0x05265BFF;
	static const ODK_UINT64 sm_maxLTOD = 0x00004E94914EFFFF;

protected:

    //
    // ReadUINT8 : Reads a generic 8-bit value from the data area. Used by public read methods
    //
    bool ReadUINT8(const long  byteOffset,     // Offset to begin reading
                   ODK_UINT8  &value) const;   // 8-bit value read

    //
    // ReadUINT16 : Reads a generic 16-bit value from the data area. Used by public read methods
    //
    bool ReadUINT16(const long  byteOffset,    // Offset to begin reading
                    ODK_UINT16 &value) const;  // 16-bit value read

    //
    // ReadUINT32 : Reads a generic 32-bit value from the data area. Used by public read methods
    //
    bool ReadUINT32(const long  byteOffset,    // Offset to begin reading
                    ODK_UINT32 &value) const;  // 32-bit value read

    //
    // ReadUINT64 : Reads a generic 64-bit value from the data area. Used by public read methods
    //
    bool ReadUINT64(const long  byteOffset,    // Offset to begin reading
                    ODK_UINT64 &value) const;  // 64-bit value read

    //
    // Class Data
    //
    long      m_nBytes; // number of bytes in m_data
    ODK_BYTE *m_data;   // array of data memory - defines the number of bytes used in 8, 16, and 32 bit values

    // defines the number of bytes used in 8, 16, and 32 bit values
    enum ODK_DataSize
    {
        ODK_SIZEBIT8  = 1,
        ODK_SIZEBIT16 = 2,
        ODK_SIZEBIT32 = 4,
        ODK_SIZEBIT64 = 8
    };

    // return true, when DTL is in valid range
    static bool CheckS7DTL(const ODK_DTL& dtl);

    // return true, when S5Time is in valid range
    bool CheckS7S5Time(const ODK_UINT16& value) const;

    // return true, when DT is in valid range
    bool CheckS7DateAndTime(const ODK_UINT64& value) const;

private:

    CODK_CpuReadData(const CODK_CpuReadData& right);
    CODK_CpuReadData& operator= (const CODK_CpuReadData& right);

    // return bcd number of value, or -1 when value is no BCD number
    ODK_INT8 GetBCD(const ODK_UINT8 value) const;
};

#endif
