#include "AjyLib.h"

namespace Ajy
{

	ByteBuffer::ByteBuffer(size_t capacity)
	{
		if (capacity <= 0)
		{
			capacity = 1024;
		}
		bufLen = Round2n(capacity);
		isRef = false;
		buf = (char*)malloc(bufLen);
		dataLen = offset = 0;
	}

	ByteBuffer::ByteBuffer()
		: ByteBuffer(1024)
	{
	}

	ByteBuffer::ByteBuffer(char* _buf, size_t _bufLen, size_t _dataLen /*= 0*/, bool _isRef /*= false */)
	{
		dataLen = _dataLen;
		offset = 0;
		if (_isRef)
		{
			buf = _buf;
			bufLen = _bufLen;
			isRef = true;
		}
		else
		{
			bufLen = Round2n(_dataLen);
			buf = (char*)malloc(bufLen);
			memcpy(buf, _buf, _dataLen);
			isRef = false;
		}
	}

	ByteBuffer::ByteBuffer(ByteBuffer const& copy)
		: ByteBuffer(copy.dataLen + 1)
	{
		memcpy(buf, copy.buf, copy.dataLen + 1);
		dataLen = copy.dataLen;
	}

	ByteBuffer::ByteBuffer(ByteBuffer&& copy)
		: buf(copy.buf)
		, bufLen(copy.bufLen)
		, dataLen(copy.dataLen)
		, offset(copy.offset)
		, isRef(copy.isRef)
	{
		//other.buf = nullptr;
		//other.bufLen = 0;
		//other.dataLen = 0;
		//other.offset = 0;
		copy.isRef = true;     // 防回收
	}

	void ByteBuffer::Destroy()
	{
		if (isRef)
		{
			return;
		}
		delete[] buf;
#ifdef Ajy_DEBUG
		buf = nullptr;
#endif
	}

	ByteBuffer::~ByteBuffer()
	{
		Destroy();
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		PtrStoreDestroy();
		IdxStoreDestroy();
#endif
	}

	void ByteBuffer::Clear()
	{
		dataLen = offset = 0;
	}

	bool ByteBuffer::Empty() const
	{
		return dataLen == 0;
	}

	bool ByteBuffer::Full() const
	{
		return dataLen == offset;
	}


	void ByteBuffer::Assign(char const* _buf, size_t _bufLen, size_t _dataLen /*= 0*/, bool _isRef /*= false */)
	{
		assert(buf != _buf);
		offset = 0;
		if (_isRef)
		{
			Destroy();
			buf = (char*)_buf;
			bufLen = _bufLen;
			dataLen = _dataLen;
			isRef = _isRef;
			return;
		}
		if (bufLen < _dataLen)
		{
			Destroy();
			bufLen = Round2n(_dataLen);
			_isRef = false;
			buf = (char*)malloc(bufLen);
		}
		dataLen = _dataLen;
		memcpy(buf, _buf, _dataLen);
	}

	void ByteBuffer::Reserve(size_t capacity)
	{
		if (capacity <= bufLen)
		{
			return;
		}
		bufLen = Round2n(capacity);
		char* newBuf = (char*)malloc(bufLen);
		memcpy(newBuf, buf, dataLen);
		Destroy();
		isRef = false;
		buf = newBuf;
	}

	void ByteBuffer::Resize(size_t len)
	{
		if (len == dataLen)
		{
			return;
		}
		else if (len < dataLen)
		{
			dataLen = len;
			if (dataLen > offset)
			{
				offset = dataLen;
			}
		}
		else
		{
			Reserve(len);
			dataLen = len;
		}
	}

	ByteBuffer& ByteBuffer::operator=(ByteBuffer const& copy)
	{
		if (this != &copy)
		{
			offset = copy.offset;
			dataLen = copy.dataLen;
			if (bufLen >= copy.dataLen)
			{
				memcpy(buf, copy.buf, copy.dataLen);
			}
			else
			{
				Destroy();
				bufLen = Round2n(copy.dataLen);
				isRef = false;
				buf = (char*)malloc(bufLen);
				memcpy(buf, copy.buf, copy.dataLen);
			}
		}
		return *this;
	}

	ByteBuffer& ByteBuffer::operator=(ByteBuffer&& copy)
	{
		if (this != &copy)
		{
			Destroy();
			buf = copy.buf;
			bufLen = copy.bufLen;
			dataLen = copy.dataLen;
			offset = copy.offset;
			isRef = copy.isRef;
			//other.buf = nullptr;
			//other.bufLen = 0;
			//other.dataLen = 0;
			//other.offset = 0;
			copy.isRef = true;
		}
		return *this;
	}

	char ByteBuffer::operator[](size_t idx) const
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	char& ByteBuffer::operator[](size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	char ByteBuffer::IndexAt(size_t idx) const
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	char& ByteBuffer::IndexAt(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}


	bool ByteBuffer::operator==(ByteBuffer const& o) const
	{
		if (dataLen != o.dataLen)
		{
			return false;
		}
		if (dataLen == 0)
		{
			return true;
		}
		return memcmp(buf, o.buf, dataLen) == 0;
	}
	bool ByteBuffer::operator!=(ByteBuffer const& o) const
	{
		return !operator==(o);
	}

	void ByteBuffer::Write(char const* _buf, size_t _dataLen)
	{
		Reserve(dataLen + _dataLen);
		memcpy(buf + dataLen, _buf, _dataLen);
		dataLen += _dataLen;
	}

	void ByteBuffer::Write(std::string const& v)
	{
		VarWrite(v.size());
		Write(v.c_str(), v.size());
	}

	bool ByteBuffer::Read(char* _buf, size_t _dataLen)
	{
		if (offset + _dataLen > dataLen)
		{
			return false;
		}
		memcpy(_buf, buf + offset, _dataLen);
		offset += _dataLen;
		return true;
	}

	bool ByteBuffer::Read(std::string& v)
	{
		size_t len;
		if (!VarRead(len)) return false;
		if (offset + len > dataLen) return false;
		v.assign(buf + offset, len);
		offset += len;
		return true;
	}








	void ByteBuffer::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(dataLen);
		if (!dataLen)
		{
			return;
		}
		bb.Write(buf, dataLen);
	}

	bool ByteBuffer::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	bool ByteBuffer::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (bb.offset + len > bb.dataLen)
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		Clear();
		Reserve(len);
		dataLen = len;
		memcpy(buf, bb.buf + bb.offset, len);
		bb.offset += len;
		return true;
	}


	void ByteBuffer::VarWriteTo(ByteBuffer& bb) const
	{
		WriteTo(bb);
	}

	bool ByteBuffer::VarReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb);
	}
	bool ByteBuffer::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		return ReadFrom(bb, minLen, maxLen);
	}










#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
	void ByteBuffer::PtrStoreInit()
	{
		if (ptrStore)
		{
			ptrStore->Clear();
		}
		else
		{
			ptrStore = new HashMap<void*, size_t>();
		}
	}
	void ByteBuffer::PtrStoreDestroy()
	{
		if (ptrStore)
		{
			delete ptrStore;
			ptrStore = nullptr;
		}
	}

	void ByteBuffer::IdxStoreInit()
	{
		if (idxStore)
		{
			idxStore->Clear();
		}
		else
		{
			idxStore = new HashMap<size_t, void*>();
		}
	}
	void ByteBuffer::IdxStoreDestroy()
	{
		if (idxStore)
		{
			delete idxStore;
			idxStore = nullptr;
		}
	}

#endif



	// negative -> ZigZag positive
	// 效果：负数变正，正数 *= 2
	// 等效代码： if( i < 0 ) return -vi * 2 - 1; else return vi * 2;
	uint32 ByteBuffer::ZigZagEncode(int32 i)
	{
		return (i << 1) ^ (i >> 31);
	}
	uint64 ByteBuffer::ZigZagEncode(int64 i)
	{
		return (i << 1) ^ (i >> 63);
	}

	// ZigZag positive -> negative
	// 等效代码： if( (i & 1) > 0 ) return -(i + 1) / 2; else return i / 2;
	int32 ByteBuffer::ZigZagDecode(uint32 i)
	{
		return (int)(i >> 1) ^ (-(int)(i & 1));
	}
	int64 ByteBuffer::ZigZagDecode(uint64 i)
	{
		return (int64)(i >> 1) ^ (-(int64)(i & 1));
	}





	void ByteBuffer::Write7Core(char* buf, size_t& offset, uint32 v)
	{
	//Lab1:
	//	uint8 b7 = uint8(v);		//得到v头7位的数值
	//	//uint8 b7 = v & 0x7F;
	//	if ((v >>= 7))
	//	{
	//		buf[offset++] = b7 | 0x80u;		//	| 0x80u是表明后面的byte也是数字的一部分。
	//		goto Lab1;
	//	}
		uint8 b7 = uint8(v);		//得到v头7位的数值
		//uint8 b7 = v & 0x7F;
		while ((v >>= 7))
		{
			buf[offset++] = b7 | 0x80u;		//	| 0x80u是表明后面的byte也是数字的一部分。
			b7 = uint8(v);
			//b7 = v & 0x7F;
		}


		buf[offset++] = b7;
	}

	void ByteBuffer::Write7Core(char* buf, size_t& offset, uint64 v)
	{
		size_t idx8 = offset + 8;
	/*Lab1:
		uint8 b7 = uint8(v);
		if ((v >>= 7))
		{
			buf[offset++] = b7 | 0x80u;
			if (offset == idx8)
			{
				buf[offset++] = (uint8)v;
			}
			else
			{
				goto Lab1;
			}
		}
		else
		{
			buf[offset++] = b7;
		}*/
		uint8 b7 = uint8(v);
		while ((v >>= 7))
		{
			buf[offset++] = b7 | 0x80u;
			if (offset == idx8)
			{
				buf[offset++] = (uint8)v;
				break;
			}
			b7 = uint8(v);
		}
		if(!(v >>= 7))
		{
			buf[offset++] = b7;
		}
	}

	bool ByteBuffer::Read7Core(uint64& v, char* buf, size_t dataLen, size_t& offset)
	{
		if (offset >= dataLen)
		{
			return false;// ReadStatus::NotEnoughData;
		}
		char* p = buf + offset;
		uint64 i = 0, b7;
		int lshift = 0;
		size_t idx = 0;
		dataLen -= offset;
	//Lab1:
	//	b7 = p[idx++];
	//	if (b7 > 0x7F)
	//	{
	//		if (idx >= dataLen)
	//		{
	//			return false;// ReadStatus::NotEnoughData;
	//		}
	//		if (idx < 8)
	//		{
	//			i |= (b7 & 0x7F) << lshift;
	//			lshift += 7;
	//			goto Lab1;
	//		}
	//		else
	//		{
	//			i |= (b7 << lshift) | ((uint64)p[idx++] << 28 << 28);
	//		}
	//	}
	//	else
	//	{
	//		i |= b7 << lshift;
	//	}
		b7 = p[idx++];
		while(b7 > 0x7F)
		{
			if (idx >= dataLen)
			{
				return false;// ReadStatus::NotEnoughData;
			}
			if (idx < 8)
			{
				i |= (b7 & 0x7F) << lshift;
				lshift += 7;
				b7 = p[idx++];
			}
			else
			{
				i |= (b7 << lshift) | ((uint64)p[idx++] << 28 << 28);
				break;
			}
		}
		if(b7 <= 0x7F)
		{
			i |= b7 << lshift;
		}



		v = i;
		offset += idx;
		return true;// ReadStatus::Success;
	}

	bool ByteBuffer::Read7Core(uint32& v, char * buf, size_t len, size_t& offset)
	{
		if (offset >= len)
		{
			return false;// ReadStatus::NotEnoughData;
		}
		char* p = buf + offset;
		uint32 i = 0, b7;
		int lshift = 0;
		size_t idx = 0;
		len -= offset;
	//Lab1:
	//	b7 = p[idx++];
	//	if (b7 > 0x7F)
	//	{
	//		if (idx == 5)
	//		{
	//			return false;	// ReadStatus::Overflow;
	//		}
	//		if (idx >= len)
	//		{
	//			return false;	// ReadStatus::NotEnoughData;
	//		}
	//		i |= (b7 & 0x7F) << lshift;
	//		lshift += 7;
	//		goto Lab1;
	//	}
	//	else
	//	{
	//		if (idx == 5 && b7 > 15)
	//		{
	//			return false;	// ReadStatus::Overflow;
	//		}
	//		else
	//		{
	//			i |= b7 << lshift;
	//		}
	//	}
		b7 = p[idx++];
		while(b7 > 0x7F)
		{
			if (idx == 5)
			{
				return false;	// ReadStatus::Overflow;
			}
			if (idx >= len)
			{
				return false;	// ReadStatus::NotEnoughData;
			}
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			b7 = p[idx++];
		}
		if(b7 <= 0x7F)
		{
			if (idx == 5 && b7 > 15)
			{
				return false;	// ReadStatus::Overflow;
			}
			else
			{
				i |= b7 << lshift;
			}
		}


		v = i;
		offset += idx;
		return true;// ReadStatus::Success;
	}





	void ByteBuffer::VarWrite(int32 v)
	{
		Reserve(dataLen + 5);
		ByteBuffer::Write7Core(buf, dataLen, ByteBuffer::ZigZagEncode(v));
	}

	void ByteBuffer::VarWrite(uint32 v)
	{
		Reserve(dataLen + 5);
		ByteBuffer::Write7Core(buf, dataLen, v);
	}

	void ByteBuffer::VarWrite(int64 v)
	{
		Reserve(dataLen + 9);
		ByteBuffer::Write7Core(buf, dataLen, ByteBuffer::ZigZagEncode(v));
	}

	void ByteBuffer::VarWrite(uint64 v)
	{
		Reserve(dataLen + 9);
		ByteBuffer::Write7Core(buf, dataLen, v);
	}

	void ByteBuffer::VarWrite(long v)
	{
#ifdef Ajy_MSVC
		VarWrite((int32)v);
#else
		VarWrite((int3264)v);
#endif
	}

	void ByteBuffer::VarWrite(ulong v)
	{
#ifdef Ajy_MSVC
		VarWrite((uint32)v);
#else
		VarWrite((uint3264)v);
#endif
	}

	void ByteBuffer::VarWrite(std::string const& v)
	{
		Write(v);
	}




	bool ByteBuffer::VarRead(int32& v)
	{
		uint32 i;
		if (!VarRead(i))
		{
			return false;
		}
		v = ByteBuffer::ZigZagDecode(i);
		return true;
	}

	bool ByteBuffer::VarRead(uint32& v)
	{
		return ByteBuffer::Read7Core(v, buf, dataLen, offset);
	}

	bool ByteBuffer::VarRead(int64& v)
	{
		uint64 i;
		if (!VarRead(i))
		{
			return false;
		}
		v = ByteBuffer::ZigZagDecode(i);
		return true;
	}

	bool ByteBuffer::VarRead(uint64& v)
	{
		return ByteBuffer::Read7Core(v, buf, dataLen, offset);
	}

	bool ByteBuffer::VarRead(long& v)
	{
#ifdef Ajy_MSVC
		return VarRead((int&)(v));
#else
		return VarRead((int3264&)(v));
#endif
	}

	bool ByteBuffer::VarRead(ulong& v)
	{
#ifdef Ajy_MSVC
		return VarRead((uint32&)(v));
#else
		return VarRead((uint3264&)(v));
#endif
	}

	bool ByteBuffer::VarRead(std::string& v)
	{
		return Read(v);
	}

	bool ByteBuffer::TryVarRead(uint32& v)
	{
		return ByteBuffer::TryRead7Core(v, buf, dataLen, offset);
	}

	bool ByteBuffer::TryRead7Core(uint32& v, char* buf, size_t len, size_t offset)
	{
		if (offset >= len)
		{
			return false;// ReadStatus::NotEnoughData;
		}
		char* p = buf + offset;
		uint32 i = 0, b7;
		int lshift = 0;
		size_t idx = 0;
		len -= offset;
		//Lab1:
		//	b7 = p[idx++];
		//	if (b7 > 0x7F)
		//	{
		//		if (idx == 5)
		//		{
		//			return false;	// ReadStatus::Overflow;
		//		}
		//		if (idx >= len)
		//		{
		//			return false;	// ReadStatus::NotEnoughData;
		//		}
		//		i |= (b7 & 0x7F) << lshift;
		//		lshift += 7;
		//		goto Lab1;
		//	}
		//	else
		//	{
		//		if (idx == 5 && b7 > 15)
		//		{
		//			return false;	// ReadStatus::Overflow;
		//		}
		//		else
		//		{
		//			i |= b7 << lshift;
		//		}
		//	}
		b7 = p[idx++];
		while (b7 > 0x7F)
		{
			if (idx == 5)
			{
				return false;	// ReadStatus::Overflow;
			}
			if (idx >= len)
			{
				return false;	// ReadStatus::NotEnoughData;
			}
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			b7 = p[idx++];
		}
		if (b7 <= 0x7F)
		{
			if (idx == 5 && b7 > 15)
			{
				return false;	// ReadStatus::Overflow;
			}
			else
			{
				i |= b7 << lshift;
			}
		}


		v = i;
		return true;// ReadStatus::Success;
	}

}
