#pragma once

namespace Ajy
{

	template<class T>
	Stack<T>::Stack(size_t capacity)
	{
		if (capacity <= 0) capacity = 8;
		size_t byteLen = Round2n(capacity * sizeof(T));
		buf = (T*)malloc(byteLen);
		bufLen = byteLen / sizeof(T);
		dataLen = 0;
	}

	template<class T >
	Stack<T>::Stack()
		:Stack(8)
	{

	}

	template<class T>
	Stack<T>::Stack(Stack const& copy)
		:Stack(copy.bufLen)
	{
		if (std::is_pod<T>::value)
		{
			memcpy(buf, copy.buf, copy.DataByteLen());
		}
		else
		{
			for (int i = 0; i < copy.dataLen; i++)
			{
				new (buf + i) T(copy.buf[i]);
			}
		}
		dataLen = copy.dataLen;
	}

	template<class T>
	Stack<T>::Stack(Stack&& copy)
		:bufLen(copy.bufLen)
		, dataLen(copy.dataLen)
		, buf(copy.buf)
	{
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
	}

	template<class T>
	Stack<T>& Stack<T>::operator=(Stack const& copy)
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
				for (int i = 0; i < copy.dataLen; i++)
				{
					new (buf + i) T(copy.buf[i]);
				}
			}
		}
		return *this;
	}

	template<class T>
	Stack<T>& Stack<T>::operator=(Stack&& copy)
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
	Stack<T>::~Stack()
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
	void Stack<T>::Clear()
	{
		for (int i = 0; i < dataLen; ++i)
		{
			buf[i].~T();
		}
		dataLen = 0;
	}

	template<class T>
	bool Stack<T>::IsEmpty() const
	{
		return dataLen == 0;
	}

	template<class T>
	size_t Stack<T>::BufLen() const
	{
		return bufLen;
	}

	template<class T>
	size_t Stack<T>::DataLen() const
	{
		return dataLen;
	}

	template<class T>
	size_t Stack<T>::BufByteLen() const
	{
		return bufLen*sizeof(T);
	}

	template<class T>
	size_t Stack<T>::DataByteLen() const
	{
		return dataLen*sizeof(T);
	}

	template<class T>
	void Stack<T>::Reserve(size_t capacity)
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
			for (int i = 0; i < dataLen; ++i)
			{
				new (newBuf + i) T(std::move(buf[i]));
				buf[i].~T();
			}
		}
		free(buf);
		buf = newBuf;
	}

	template<class T>
	void Stack<T>::Push(T const& v)
	{
		if (dataLen == bufLen)
		{
			Reserve(dataLen + 1);
		}
		new (buf + dataLen++) T(v);
	}

	template<class T>
	void Stack<T>::Push(T&& v)
	{
		if (dataLen == bufLen)
		{
			Reserve(dataLen + 1);
		}
		new (buf + dataLen++) T((T&&)v);
	}

	template<class T>
	template<class ...Args>
	void Stack<T>::Push(T const& v, Args&&... args)
	{
		Push(v);
		Push(std::forward<Args>(args)...);
	}

	template<class T>
	template<class ...Args>
	void Stack<T>::Push(T&& v, Args&&... args)
	{
		//Push( std::move( v ) );
		Push((T&&)v);
		Push(std::forward<Args>(args)...);
	}

	template<class T>
	T& Stack<T>::Top()
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}

	template<class T>
	T const& Stack<T>::Top() const
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}

	template<class T>
	void Stack<T>::Pop()
	{
		assert(dataLen > 0);
		buf[--dataLen].~T();
	}

	template<class T>
	bool Stack<T>::Pop(T& output)
	{
		if (!dataLen)
		{
			return false;
		}
		output = std::move(buf[--dataLen]);
		buf[dataLen].~T();
		return true;
	}

	/*template<class T>
	T const& Stack<T>::At(size_t idx) const
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}

	template<class T>
	T& Stack<T>::At(size_t idx)
	{
		assert(idx < dataLen && idx >= 0);
		return buf[idx];
	}*/

}
