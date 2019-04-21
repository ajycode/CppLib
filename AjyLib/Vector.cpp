#include "AjyLib.h"

namespace Ajy
{

	Vector<bool>::Vector(size_t capacity)
	{
		assert(capacity > 0);
		size_t len = (capacity - 1) / 8 + 1;
		buf = (char*)malloc(len);
		bufLen = len * 8;
		dataLen = 0;
		memset(buf, 0, len);
	}

	Vector<bool>::Vector()
		: Vector(8)
	{
	}

	Vector<bool>::~Vector()
	{
		if (buf)
		{
			Clear();
			free(buf);
			buf = nullptr;
			bufLen = 0;
			dataLen = 0;
		}
	}

	Vector<bool>::Vector(Vector<bool>&& copy)
		: buf(copy.buf)
		, bufLen(copy.bufLen)
		, dataLen(copy.dataLen)
	{
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
	}

	Vector<bool>::Vector(Vector<bool> const& copy)
		: Vector<bool>(copy.bufLen)
	{
		memcpy(buf, copy.buf, copy.DataByteLen());
		dataLen = copy.dataLen;
	}

	Vector<bool>& Vector<bool>::operator=(Vector<bool>&& copy)
	{
		free(buf);
		buf = copy.buf;
		dataLen = copy.dataLen;
		bufLen = copy.bufLen;
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
		return *this;
	}

	Vector<bool>& Vector<bool>::operator=(Vector<bool> const& copy)
	{
		if (this != &copy)
		{
			dataLen = copy.dataLen;
			if (bufLen < copy.dataLen)
			{
				bufLen = copy.bufLen;
				free(buf);
				buf = (char*)malloc(copy.BufByteLen());
				memset(buf, 0, BufByteLen());
			}
			memcpy(buf, copy.buf, copy.DataByteLen());
		}
		return *this;
	}

	void Vector<bool>::Push(bool v)
	{
		if (dataLen == bufLen) Reserve(dataLen + 1);
		Set(dataLen++, v);
	}

	void Vector<bool>::Pop()
	{
		assert(dataLen > 0);
		--dataLen;
	}

	bool Vector<bool>::Top() const
	{
		assert(dataLen > 0);
		return At(dataLen - 1);
	}

	void Vector<bool>::Clear()
	{
		dataLen = 0;
	}

	void Vector<bool>::Reserve(size_t capacity)
	{
		assert(capacity > 0);
		if (capacity <= bufLen) return;
		size_t len = (capacity - 1) / 8 + 1;
		char* newBuf = (char*)malloc(len);
		bufLen = len * 8;
		memset(newBuf, 0, len);
		memcpy(newBuf, buf, DataByteLen());
		free(buf);
		buf = newBuf;
	}

	bool Vector<bool>::Empty() const
	{
		return !dataLen;
	}

	size_t Vector<bool>::DataLen() const
	{
		return dataLen;
	}

	size_t Vector<bool>::BufLen() const
	{
		return bufLen;
	}


	size_t Vector<bool>::DataByteLen() const
	{
		if (dataLen) return (dataLen - 1) / 8 + 1;
		return 0;
	}

	size_t Vector<bool>::BufByteLen() const
	{
		if (bufLen) return (bufLen - 1) / 8 + 1;
		return 0;
	}

	BitArrayHelper Vector<bool>::operator[](size_t idx)
	{
		return BitArrayHelper(this, idx);
	}

	bool Vector<bool>::operator[](size_t idx) const
	{
		return At(idx);
	}

	bool Vector<bool>::At(size_t idx) const
	{
		assert(idx >= 0 && idx < dataLen);
		return (((size_t*)buf)[idx / (sizeof(size_t) * 8)] &
			(size_t(1) << (idx % (sizeof(size_t) * 8)))) > 0;
	}

	void Vector<bool>::Set(size_t idx, bool v)
	{
		assert(idx >= 0 && idx < dataLen);
		if (v) SetTrue(idx);
		else SetFalse(idx);
	}

	void Vector<bool>::SetRange(size_t idx, bool v)
	{
		assert(idx >= 0 && idx < dataLen);
		if (v) SetTrue(idx);
		else SetFalse(idx);
	}

	void Vector<bool>::SetTrue(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		/*char& tmp = buf[ idx / 8 ];
		tmp |= 1 << ( idx % 8 );*/
		((size_t*)buf)[idx / (sizeof(size_t) * 8)] |=
			(size_t(1) << (idx % (sizeof(size_t) * 8)));
	}

	void Vector<bool>::SetFalse(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		/*char& tmp = buf[ idx / 8 ];
		tmp &= ~( 1 << ( idx % 8 ) );*/
		((size_t*)buf)[idx / (sizeof(size_t) * 8)] &=
			~(size_t(1) << (idx % (sizeof(size_t) * 8)));
	}

	void Vector<bool>::FillTrue()
	{
		assert(dataLen > 0);
		memset(buf, 0xFFFFFFFFu, (dataLen - 1) / 8 + 1);
	}

	void Vector<bool>::FillFalse()
	{
		assert(dataLen > 0);
		memset(buf, 0, (dataLen - 1) / 8 + 1);
	}

	void Vector<bool>::Fill(bool v, size_t idxFrom, size_t idxTo)
	{
		assert(dataLen > 0 && idxFrom >= 0 && idxTo >= 0 && idxFrom < dataLen && idxTo < dataLen);
		if (idxFrom == idxTo)
		{
			Set(idxFrom, v);
			return;
		}
		if (idxFrom > idxTo)
		{
			std::swap(idxFrom, idxTo);
		}
		size_t byteIdxFrom = idxFrom >> 3;
		size_t byteIdxTo = idxTo >> 3;

		if (byteIdxFrom == byteIdxTo)
		{
			if (v)
			{
				buf[byteIdxFrom] |= (uint8)0xFFu >> (7 - (idxTo - idxFrom)) << (idxFrom & 7);
			}
			else
			{
				buf[byteIdxFrom] &= ~((uint8)0xFFu >> (7 - (idxTo - idxFrom)) << (idxFrom & 7));
			}
		}
		else
		{
			size_t idxFrom7 = idxFrom & 7;
			size_t idxTo7 = idxTo & 7;
			if (v)
			{
				buf[byteIdxFrom] |= (uint8)0xFFu << idxFrom7;
				buf[byteIdxTo] |= (uint8)0xFFu >> (7 - idxTo7);
			}
			else
			{
				buf[byteIdxFrom] &= ~((uint8)0xFFu << idxFrom7);
				buf[byteIdxTo] &= ~((uint8)0xFFu >> (7 - idxTo7));
			}
			++byteIdxFrom;
			if (byteIdxFrom < byteIdxTo)
			{
				memset(buf + byteIdxFrom, v ? 0xFFFFFFFFu : 0, byteIdxTo - byteIdxFrom);
			}
		}
	}

	void Vector<bool>::Resize(size_t capacity, bool init)
	{
		if (capacity == dataLen) return;
		if (capacity < dataLen)
		{
			dataLen = capacity;
			return;
		}
		Reserve(capacity);
		size_t oldLen = dataLen;
		dataLen = capacity;
		if (init)
		{
			Fill(false, oldLen, capacity - 1);
		}
	}

	void Vector<bool>::CleanUp() const
	{
		if (size_t mod = dataLen % 8)
		{
			buf[(dataLen - 1) / 8] &= ~(0xFF << mod);
		}
	}

	BitArrayHelper::BitArrayHelper(BitArray_Ptr _bitarr, size_t _idx)
	{
		bitarr = _bitarr;
		index = _idx;
	}

	BitArrayHelper::BitArrayHelper(BitArrayHelper const& copy)
	{
		bitarr->Set(index, (*copy.bitarr)[copy.index]);
	}

	BitArrayHelper::BitArrayHelper(BitArrayHelper&& copy)
	{
		bitarr->Set(index, (*copy.bitarr)[copy.index]);
		copy.bitarr = nullptr;
		copy.index = 0;
	}

	BitArrayHelper& BitArrayHelper::operator=(BitArrayHelper const& copy)
	{
		if (this != &copy)
		{
			bitarr->Set(index, (*copy.bitarr)[copy.index]);
		}
		return *this;
	}

	BitArrayHelper & BitArrayHelper::operator=(BitArrayHelper&& copy)
	{
		if (this != &copy)
		{
			bitarr->Set(index, (*copy.bitarr)[copy.index]);
			copy.bitarr = nullptr;
			copy.index = 0;
		}
		return *this;
	}

	BitArrayHelper& BitArrayHelper::operator=(bool _flag)
	{
		bitarr->Set(index, _flag);
		return *this;
	}

	BitArrayHelper::~BitArrayHelper()
	{
		bitarr = nullptr;
	}

	BitArrayHelper::operator bool()
	{
		return bitarr->At(index);
	}

	void Vector<bool>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite((uint)dataLen);
		if (dataLen)
		{
			CleanUp();
			bb.Write(buf, DataByteLen());
		}
	}

	bool Vector<bool>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	bool Vector<bool>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(*(uint*)&len))
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		if (len == 0)
		{
			Clear();
			return true;
		}
		size_t byteLen = (len - 1) / 8 + 1;
		if (bb.offset + byteLen > bb.dataLen)
		{
			return false;
		}
		Reserve(len);
		dataLen = len;
		memcpy(buf, bb.buf + bb.offset, byteLen);
		bb.offset += byteLen;

		return true;
	}

	void Vector<bool>::VarWriteTo(ByteBuffer& bb) const
	{
		WriteTo(bb);
	}

	bool Vector<bool>::VarReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb);
	}

	bool Vector<bool>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		return ReadFrom(bb, minLen, maxLen);
	}

}
