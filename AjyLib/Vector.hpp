#pragma once

namespace Ajy
{

	template<class T>
	Vector<T>::Vector(size_t capacity)
	{
		if (capacity <= 0) capacity = 8;
		size_t byteLen = Round2n(capacity * sizeof(T));
		buf = (T*)malloc(byteLen);
		bufLen = byteLen / sizeof(T);
		dataLen = 0;
	}

	template<class T>
	Vector<T>::Vector()
		: Vector(8)
	{
	}

	template<class T>
	Vector<T>::~Vector()
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

	template<class T>
	Vector<T>::Vector(Vector&& copy)
		: buf(copy.buf)
		, bufLen(copy.bufLen)
		, dataLen(copy.dataLen)
	{
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
	}

	template<class T>
	Vector<T>::Vector(Vector const& copy)
		: Vector(copy.dataLen)
	{
		if (std::is_pod<T>::value)
		{
			memcpy(buf, copy.buf, copy.DataByteLen());
		}
		else
		{
			for (size_t i = 0; i < copy.dataLen; ++i)
			{
				new (buf + i) T(copy.buf[i]);
			}
		}
		dataLen = copy.dataLen;
	}

	template<class T>
	Vector<T>& Vector<T>::operator=(Vector&& copy)
	{
		Clear();
		free(buf);
		buf = copy.buf;
		bufLen = copy.bufLen;
		dataLen = copy.dataLen;
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
		return *this;
	}

	template<class T>
	Vector<T>& Vector<T>::operator=(Vector const& copy)
	{
		if (this != &copy)
		{
			Clear();
			Reserve(copy.dataLen);
			dataLen = copy.dataLen;
			if (std::is_pod<T>::value)
			{
				memcpy(buf, copy.buf, copy.DataByteLen());
			}
			else
			{
				for (size_t i = 0; i < copy.dataLen; ++i)
				{
					new (buf + i) T(copy.buf[i]);
				}
			}
		}
		return *this;
	}

	template<class T>
	bool Vector<T>::operator==(Vector const& o) const
	{
		if (dataLen != o.dataLen)
		{
			return false;
		}
		if (dataLen == 0)
		{
			return true;
		}
		for (size_t i = 0; i < dataLen; ++i)
		{
			if (!(buf[i] == o.buf[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<class T>
	bool Vector<T>::operator!=(Vector const& o) const
	{
		return !operator==(o);
	}

	template<class T>
	void Vector<T>::FastPush(T&& v)
	{
		new (buf + dataLen++) T((T&&)v);
	}

	template<class T>
	void Vector<T>::FastPush(T const& v)
	{
		new (buf + dataLen++) T(v);
	}

	template<class T>
	void Vector<T>::Push(T&& v)
	{
		if (dataLen == bufLen)
		{
			Reserve(dataLen + 1);
		}
		new (buf + dataLen++) T((T&&)v);
	}

	template<class T>
	void Vector<T>::Push(T const& v)
	{
		if (dataLen == bufLen)
		{
			Reserve(dataLen + 1);
		}
		new (buf + dataLen++) T(v);
	}

	template<class T>
	void Vector<T>::Pop()
	{
		assert(dataLen > 0);
		buf[--dataLen].~T();
	}

	template<class T>
	T& Vector<T>::Top()
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}
	template<class T>
	T const& Vector<T>::Top() const
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}

	template<class T>
	bool Vector<T>::Pop(T& output)
	{
		if (!dataLen)
		{
			return false;
		}
		output = std::move(buf[--dataLen]);
		buf[dataLen].~T();
		return true;
	}

	template<class T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0; i < dataLen; ++i)
		{
			buf[i].~T();
		}
		dataLen = 0;
	}

	template<class T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity <= bufLen)
		{
			return;
		}
		size_t byteLen = Round2n(capacity * sizeof(T));
		bufLen = byteLen / sizeof(T);
		T* newBuf = (T*)malloc(byteLen);
		if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
		{
			memcpy(newBuf, buf, DataByteLen());
		}
		else
		{
			for (size_t i = 0; i < dataLen; ++i)
			{
				new (&newBuf[i]) T(std::move(buf[i]));
				buf[i].~T();
			}
		}
		free(buf);
		buf = newBuf;
	}

	template<class T>
	void Vector<T>::Resize(size_t len, bool init /*= true */)
	{
		if (len == dataLen)
		{
			return;
		}
		else if (len < dataLen)
		{
			if (!std::is_pod<T>::value)
			{
				for (size_t i = len; i < dataLen; ++i)
				{
					buf[i].~T();
				}
			}
		}
		else // len > dataLen
		{
			Reserve(len);
			if (init)
			{
				if (std::is_pod<T>::value)
				{
					memset(buf + dataLen, 0, (len - dataLen) * sizeof(T));
				}
				else
				{
					for (size_t i = dataLen; i < len; ++i)
					{
						new (buf + i) T();
					}
				}
			}
		}
		dataLen = len;
	}

	template<class T>
	T* Vector<T>::Buf() const
	{
		return buf;
	}

	template<class T>
	bool Vector<T>::Empty() const
	{
		return !dataLen;
	}

	template<class T>
	size_t Vector<T>::DataLen() const
	{
		return dataLen;
	}

	template<class T>
	size_t Vector<T>::BufLen() const
	{
		return bufLen;
	}

	template<class T>
	size_t Vector<T>::DataByteLen() const
	{
		return dataLen * sizeof(T);
	}

	template<class T>
	size_t Vector<T>::BufByteLen() const
	{
		return bufLen * sizeof(T);
	}

	template<class T>
	T const& Vector<T>::operator[](size_t idx) const
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}

	template<class T>
	T& Vector<T>::operator[](size_t idx)
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}

	template<class T>
	T const& Vector<T>::At(size_t idx) const
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}

	template<class T>
	T& Vector<T>::At(size_t idx)
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}

	template<class T>
	void Vector<T>::Set(size_t idx, T&& v)
	{
		assert(idx >= 0 && idx < dataLen);
		buf[idx] = (T&&)v;
	}
	template<class T>
	void Vector<T>::Set(size_t idx, T const& v)
	{
		assert(idx >= 0 && idx < dataLen);
		buf[idx] = v;
	}

	template<class T>
	size_t Vector<T>::Find(T const& v) const
	{
		for (size_t i = 0; i < dataLen; ++i)
		{
			if (buf[i] == v)
			{
				return i;
			}
		}
		return -1;
	}

	template<class T>
	void Vector<T>::Remove(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		--dataLen;
		if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
		{
			buf[idx].~T();
			memmove(buf + idx, buf + idx + 1, (dataLen - idx) * sizeof(T));
		}
		else
		{
			for (size_t i = idx; i < dataLen; ++i)
			{
				buf[i] = (T&&)buf[i + 1];
			}
			buf[dataLen].~T();
		}
	}

	template<class T>
	void Vector<T>::FastRemove(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		--dataLen;
		if (dataLen == idx)	// last one
		{
			buf[idx].~T();
			return;
		}
		if (std::is_pod<T>::value)
		{
			buf[idx] = buf[dataLen];
		}
		else if (std::is_base_of<MemoryMoveable, T>::value)
		{
			buf[idx].~T();
			memcpy(buf + idx, buf + dataLen, sizeof(T));
		}
		else
		{
			buf[idx] = (T&&)buf[dataLen];
			buf[dataLen].~T();
		}
	}

	template<class T>
	template<class... Args>
	void Vector<T>::EmplaceInsert(size_t idx, Args&&... args)
	{
		assert(idx >= 0 && idx <= dataLen);
		Reserve(dataLen + 1);

		if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
		{
			memmove(buf + idx + 1, buf + idx, (dataLen - idx) * sizeof(T));
			++dataLen;
			new (buf + idx) T(std::forward<Args>(args)...);
		}
		else
		{
			if (idx < dataLen)
			{
				new (buf + dataLen) T((T&&)buf[dataLen - 1]);
				for (size_t i = dataLen - 1; i > idx; --i)
				{
					buf[i] = (T&&)buf[i - 1];
				}
				buf[idx].~T();
			}
			++dataLen;
			new (buf + idx) T(std::forward<Args>(args)...);
		}
	}

	template<class T>
	void Vector<T>::Insert(size_t idx, T&& v)
	{
		EmplaceInsert(idx, (T&&)v);
	}

	template<class T>
	void Vector<T>::Insert(size_t idx, T const& v)
	{
		EmplaceInsert(idx, v);
	}

	template<class T>
	template<class... Args>
	void Vector<T>::Push(T const& v, Args&&... args)
	{
		Push(v);
		Push(std::forward<Args>(args)...);
	}

	template<class T>
	template<class... Args>
	void Vector<T>::Push(T&& v, Args&&... args)
	{
		Push((T&&)v);
		Push(std::forward<Args>(args)...);
	}

	template<typename T>
	void Vector<T>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(dataLen);
		if (!std::is_pointer<T>::value && std::is_pod<T>::value)
		{
			if (!dataLen)
			{
				return;
			}
			bb.Write((char*)buf, dataLen * sizeof(T));
			return;
		}
		for (size_t i = 0; i < dataLen; ++i)
		{
 			bb.Write(buf[i]);
		}
	}

	template<typename T>
	bool Vector<T>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	template<typename T>
	bool Vector<T>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		if (!std::is_pointer<T>::value && std::is_pod<T>::value)
		{
			size_t siz = len * sizeof(T);
			if (bb.offset + siz > bb.dataLen)
			{
				return false;
			}
			Clear();
			Resize(len, false);
			memcpy(buf, bb.buf + bb.offset, siz);
			bb.offset += siz;
			return true;
		}
		Clear();
		if (!len)
		{
			return true;
		}
		Reserve(len);
		for (size_t i = 0; i < len; ++i)
		{
			new (buf + i) T();
			dataLen = i + 1;
			if (!bb.Read(buf[i]))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	void Vector<T>::VarWriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(dataLen);
		for (size_t i = 0; i < dataLen; ++i)
		{
			bb.VarWrite(buf[i]);
		}
	}

	template<typename T>
	bool Vector<T>::VarReadFrom(ByteBuffer& bb)
	{
		return VarReadFrom(bb, 0, 0);
	}

	template<typename T>
	bool Vector<T>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		Clear();
		if (!len)
		{
			return true;
		}
		Reserve(len);
		for (size_t i = 0; i < len; ++i)
		{
			new (buf + i) T();
			dataLen = i + 1;
			if (!bb.VarRead(buf[i]))
			{
				return false;
			}
		}
		return true;
	}

	template<class... Args>
	void Vector<bool>::Push(bool bit, Args&&... args)
	{
		Push(bit);
		Push(std::forward<Args>(args)...);
	}

	template<class... Args>
	void Vector<bool>::SetRange(size_t idx, bool bit, Args&&... args)
	{
		SetRange(idx, bit);
		SetRange(++idx, std::forward<Args>(args)...);
	}

}
