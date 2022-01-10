#ifndef __TARGET_PARAMS_H__
#define __TARGET_PARAMS_H__

#ifdef __GNUC__
#define AtomicAdd(ptr,val) __sync_fetch_and_add (ptr, val)
#define AtomicSub(ptr,val) __sync_fetch_and_sub (ptr, val)
#define CAS(ptr, oldVal, newVal) __sync_bool_compare_and_swap(ptr, oldVal, newVal)
#else
#error Atomic functions (CAS, AtomicAdd, AtomicSub) cannot be defined
#endif // __GNUC__

#include <stdint.h>
#include <assert.h>

// Raynal's implementation for a readers-writer lock which is read-preferring
class RWLock
{
private:
	static const uint32_t UNLOCKED = 0;
	static const uint32_t LOCKED = UINT32_MAX;
	uint32_t r; // mutex to protect b and to be only used by readers
	uint32_t g; // global mutex to ensure mutual exclusion of writers
	uint32_t b; // the number of blocking readers
	void lock(uint32_t *locker);
	void unlock(uint32_t *locker);
public:
	RWLock();
    virtual ~RWLock();
    void RdLock();
    void WrLock();
    void RdUnlock();
    void WrUnlock();
};

RWLock::RWLock()
{
	r = UNLOCKED;
	g = UNLOCKED;
	b = 0;
}

RWLock::~RWLock()
{
}

void RWLock::lock(uint32_t *locker)
{
	do
	{
		if (CAS(locker, UNLOCKED, LOCKED))
		{
			return;
		}
	} while(1);

    assert(0); // something went wrong
}

void RWLock::unlock(uint32_t *locker)
{
	CAS(locker, LOCKED, UNLOCKED);
}

void RWLock::RdLock()
{
	lock(&r);
	AtomicAdd(&b, 1);
	if (b==1)
		lock(&g);
	unlock(&r);
}

void RWLock::WrLock()
{
	lock(&g);
}

void RWLock::RdUnlock()
{
	lock(&r);
	AtomicSub(&b, 1);
	if (b==0)
		unlock(&g);
	unlock(&r);
}

void RWLock::WrUnlock()
{
	unlock(&g);
}

union OdkParam
{
	ODK_FLOAT m_ODK_FLOAT;
	ODK_DOUBLE m_ODK_DOUBLE;
	ODK_INT32 m_ODK_INT32;
	ODK_INT64 m_ODK_INT64;
	ODK_UINT32 m_ODK_UINT32;
	ODK_UINT64 m_ODK_UINT64;
	ODK_INT16 m_ODK_INT16;
	ODK_UINT16 m_ODK_UINT16;
	ODK_INT8 m_ODK_INT8;
	ODK_UINT8 m_ODK_UINT8;
	ODK_CHAR m_ODK_CHAR;
	//ODK_BOOL m_ODK_BOOL; // not necessary since it is the same as ODK_UINT8
};

union SlParamPointer
{
	real32_T *m_real32_T;
	real_T *m_real_T;
	//real64_T *m_real64_T; // not necessary since it is the same as real_T
	int32_T *m_int32_T;
	int_T *m_int_T;
	int64_T *m_int64_T;
	uint32_T *m_uint32_T;
	uint_T *m_uint_T;
	uint64_T *m_uint64_T;
	int16_T *m_int16_T;
	uint16_T *m_uint16_T;
	int8_T *m_int8_T;
	uint8_T *m_uint8_T;
	char_T *m_char_T;
	uchar_T *m_uchar_T;
	//byte_T *m_byte_T; // not necessary since it is the same as uchar_T
	//boolean_T *m_boolean_T; // not necessary since it is the same as uint8_T
};

struct UpdatedParam {
	OdkParam odkParam;
	SlParamPointer slParamPointer;
	int type;
	UpdatedParam(real32_T *x, const ODK_FLOAT& y) : type(1) { slParamPointer.m_real32_T = x; odkParam.m_ODK_FLOAT = y; }
	UpdatedParam(real_T *x, const ODK_DOUBLE& y) : type(2) { slParamPointer.m_real_T = x; odkParam.m_ODK_DOUBLE = y; }
	UpdatedParam(int32_T *x, const ODK_INT32& y) : type(3) { slParamPointer.m_int32_T = x; odkParam.m_ODK_INT32 = y; }
	UpdatedParam(int_T *x, const ODK_INT64& y) : type(4) { slParamPointer.m_int_T = x; odkParam.m_ODK_INT64 = y; }
	UpdatedParam(int64_T *x, const ODK_INT64& y) : type(5) { slParamPointer.m_int64_T = x; odkParam.m_ODK_INT64 = y; }
	UpdatedParam(uint32_T *x, const ODK_UINT32& y) : type(6) { slParamPointer.m_uint32_T = x; odkParam.m_ODK_UINT32 = y; }
	UpdatedParam(uint_T *x, const ODK_UINT64& y) : type(7) { slParamPointer.m_uint_T = x; odkParam.m_ODK_UINT64 = y; }
	UpdatedParam(uint64_T *x, const ODK_UINT64& y) : type(8) { slParamPointer.m_uint64_T = x; odkParam.m_ODK_UINT64 = y; }
	UpdatedParam(int16_T *x, const ODK_INT16& y) : type(9) { slParamPointer.m_int16_T = x; odkParam.m_ODK_INT16 = y; }
	UpdatedParam(uint16_T *x, const ODK_UINT16& y) : type(10) { slParamPointer.m_uint16_T = x; odkParam.m_ODK_UINT16 = y; }
	UpdatedParam(int8_T *x, const ODK_INT8& y) : type(11) { slParamPointer.m_int8_T = x; odkParam.m_ODK_INT8 = y; }
	UpdatedParam(uint8_T *x, const ODK_UINT8& y) : type(12) { slParamPointer.m_uint8_T = x; odkParam.m_ODK_UINT8 = y; }
	UpdatedParam(char_T *x, const ODK_CHAR& y) : type(13) { slParamPointer.m_char_T = x; odkParam.m_ODK_CHAR = y; }
	UpdatedParam(uchar_T *x, const ODK_CHAR& y) : type(14) { slParamPointer.m_uchar_T = x; odkParam.m_ODK_CHAR = y; }
	void AssignValue()
	{
		switch(type) {
		case 1:
			*(slParamPointer.m_real32_T) = odkParam.m_ODK_FLOAT; break;
		case 2:
			*(slParamPointer.m_real_T) = odkParam.m_ODK_DOUBLE; break;
		case 3:
			*(slParamPointer.m_int32_T) = odkParam.m_ODK_INT32; break;
		case 4:
			*(slParamPointer.m_int_T) = odkParam.m_ODK_INT64; break;
		case 5:
			*(slParamPointer.m_int64_T) = odkParam.m_ODK_INT64; break;
		case 6:
			*(slParamPointer.m_uint32_T) = odkParam.m_ODK_UINT32; break;
		case 7:
			*(slParamPointer.m_uint_T) = odkParam.m_ODK_UINT64; break;
		case 8:
			*(slParamPointer.m_uint64_T) = odkParam.m_ODK_UINT64; break;
		case 9:
			*(slParamPointer.m_int16_T) = odkParam.m_ODK_INT16; break;
		case 10:
			*(slParamPointer.m_uint16_T) = odkParam.m_ODK_UINT16; break;
		case 11:
			*(slParamPointer.m_int8_T) = odkParam.m_ODK_INT8; break;
		case 12:
			*(slParamPointer.m_uint8_T) = odkParam.m_ODK_UINT8; break;
		case 13:
			*(slParamPointer.m_char_T) = odkParam.m_ODK_CHAR; break;
		case 14:
			*(slParamPointer.m_uchar_T) = odkParam.m_ODK_CHAR; break;
		}
	}
};

#endif // __TARGET_PARAMS_H__
