#pragma once

namespace Ajy
{

	template <class T>
	Queue<T>::Queue(size_t capacity)
	{
		if (capacity <= 0) capacity = 8;
		size_t byteLen = Round2n(capacity * sizeof(T));
		bufLen = byteLen / sizeof(T);
		buf = (T*)malloc(byteLen);
		// FR..............................
		front = 0;
		rear = 0;
	}

	template <class T>
	Queue<T>::Queue()
		: Queue(8)
	{

	}

	template<class T>
	Queue<T>::Queue(Queue const& copy)
		: Queue(copy.bufLen)
	{
		operator=(copy);
	}

	template<class T>
	Queue<T>::Queue(Queue&& copy)
		: buf(copy.buf)
		, bufLen(copy.bufLen)
		, front(copy.front)
		, rear(copy.rear)
	{
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.front = 0;
		copy.rear = 0;
	}

	template<class T>
	Queue<T>& Queue<T>::operator=(Queue const& copy)
	{
		if (this != &copy)
		{
			Clear();
			size_t byteLen = Round2n(copy.bufLen * sizeof(T));
			bufLen = byteLen / sizeof(T);
			buf = (T*)malloc(byteLen);
			if (copy.front < copy.rear)
			{
				if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
				{
					memcpy(buf, copy.buf + copy.front, copy.Size() * sizeof(T));
				}
				else
				{
					for (size_t i = 0; i < copy.Size(); ++i)
					{
						new (buf + i) T(copy.buf[copy.front + i]);
					}
				}
			}
			// ++++++Rear...........Front++++++
			// ++++++++++++++RF++++++++++++++++
			else
			{
				//...Front++++++
				size_t frontDataLen = copy.bufLen - copy.front;
				if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
				{
					memcpy(buf, copy.buf + copy.front, frontDataLen * sizeof(T));
				}
				else
				{
					for (size_t i = 0; i < frontDataLen; ++i)
					{
						new (buf + i) T(copy.buf[copy.front + i]);
					}
				}
				// ++++++Rear...
				T* dstPos = buf + frontDataLen;
				if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
				{
					memcpy(dstPos, copy.buf, copy.rear * sizeof(T));
				}
				else
				{
					for (size_t i = 0; i < copy.rear; ++i)
					{
						new (dstPos + i) T(copy.buf[i]);
					}
				}
			}

			front = 0;
			rear = copy.Size();
		}
		return *this;
	}

	template<class T>
	Queue<T>& Queue<T>::operator=(Queue&& copy)
	{
		Clear();
		free(buf);
		buf = copy.buf;
		bufLen = copy.bufLen;
		front = copy.front;
		rear = copy.rear;
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.front = 0;
		copy.rear = 0;
		return *this;
	}

	template <class T>
	Queue<T>::~Queue()
	{
		if (buf)
		{
			Clear();
			free(buf);
			buf = nullptr;
			front = rear = 0;
		}
	}

	template <class T>
	size_t Queue<T>::Size() const
	{
		//......Front+++++++++++Rear......
		//...............FR...............
		if (front <= rear)
		{
			return rear - front;
		}
		// ++++++Rear...........Front++++++
		else
		{
			return rear + (bufLen - front);
		}
	}

	template <class T>
	bool Queue<T>::Empty() const
	{
		return front == rear;
	}

	template <class T>
	void Queue<T>::Pop()
	{
		assert(front != rear);
		buf[front++].~T();
		if (front == bufLen)
		{
			front = 0;
		}
	}

	template <class T>
	T const& Queue<T>::Front() const
	{
		assert(front != rear);
		return buf[front];
	}

	template <class T>
	T& Queue<T>::Front()
	{
		assert(front != rear);
		return buf[front];
	}


	template <class T>
	template<class... Args>
	T& Queue<T>::EmplacePush(Args&&... args)
	{
		size_t idx = rear;
		new (buf + rear++) T(std::forward<Args>(args)...);

		if (rear == bufLen)		// cycle
		{
			rear = 0;
		}
		if (rear == front)			// no more space
		{
			idx = bufLen;
			Reserve(bufLen + 1, true);
		}
		return buf[idx];
	}
	template <class T>
	void Queue<T>::Push(T const& v)
	{
		EmplacePush(v);
	}
	template <class T>
	void Queue<T>::Push(T&& v)
	{
		EmplacePush((T&&)v);
	}

	template<class T>
	template<class... Args>
	void Queue<T>::Push(T const& v, Args&&... args)
	{
		Push(v);
		Push(std::forward<Args>(args)...);
	}
	template<class T>
	template<class... Args>
	void Queue<T>::Push(T&& v, Args&&... args)
	{
		Push((T&&)v);
		Push(std::forward<Args>(args)...);
	}

	template <class T>
	void Queue<T>::Clear()
	{
		if (front == rear)
		{
			return;
		}
		if (!std::is_pod<T>::value)
		{
			//......Front+++++++++++Rear......
			if (front < rear)
			{
				for (size_t i = front; i < rear; ++i)
				{
					buf[i].~T();
				}
			}
			// ++++++Rear...........Front++++++
			else
			{
				size_t i = 0;
				for (; i < rear; ++i)
				{
					buf[i].~T();
				}
				for (i = front; i < bufLen; ++i)
				{
					buf[i].~T();
				}
			}
		}
		//........FR......................
		front = rear;
	}

	template <typename T>
	size_t Queue<T>::PopMulti(size_t count)
	{
		if (count <= 0)
		{
			return 0;
		}
		size_t dataLen = Size();
		if (count >= dataLen)
		{
			Clear();
			return dataLen;
		}
		// count < dataLen

		//......Front+++++++++++Rear......
		if (front < rear)
		{
			if (!std::is_pod<T>::value)
			{
				for (size_t i = front; i < count; i++)
				{
					buf[i].~T();
				}
			}
			front += count;
		}
		// ++++++Rear...........Front++++++
		else
		{
			size_t frontDataLen = bufLen - front;
			if (!std::is_pod<T>::value)
			{
				//...Front+++
				if (count < frontDataLen)
				{
					for (size_t i = front; i < front + count; ++i)
					{
						buf[i].~T();
					}
					front += count;
				}
				else
				{
					//...Front++++++
					size_t i = front;
					for (; i < bufLen; ++i)
					{
						buf[i].~T();
					}
					// ++++++Rear...
					count -= frontDataLen;
					for (i = 0; i < count; ++i)
					{
						buf[i].~T();
					}
					front = count;
				}
			}
		}
		return count;
	}

	template <class T>
	void Queue<T>::Reserve(size_t capacity, bool isFull)
	{
		assert(capacity > 0);
		if (capacity <= bufLen)
		{
			return;
		}

		// afterPush: ++++++++++++++RF++++++++++++++++
		size_t dataLen = isFull ? bufLen : Size();

		size_t newBufByteLen = Round2n(capacity * sizeof(T));
		size_t newBufLen = newBufByteLen / sizeof(T);
		T* newBuf = (T*)malloc(newBufByteLen);

		//......Front+++++++++++Rear......
		if (front < rear)
		{
			if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
			{
				memcpy(newBuf, buf + front, dataLen * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < dataLen; ++i)
				{
					new (newBuf + i) T(std::move(buf[front + i]));
				}
			}
		}
		// ++++++Rear...........Front++++++
		// ++++++++++++++RF++++++++++++++++
		else
		{
			//...Front++++++
			size_t frontDataLen = bufLen - front;
			if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
			{
				memcpy(newBuf, buf + front, frontDataLen * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < frontDataLen; ++i)
				{
					new (newBuf + i) T(std::move(buf[front + i]));
				}
			}
			// ++++++Rear...
			T* dstPos = newBuf + frontDataLen;
			if (std::is_pod<T>::value || std::is_base_of<MemoryMoveable, T>::value)
			{
				memcpy(dstPos, buf, rear * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < rear; ++i)
				{
					new (dstPos + i) T(std::move(buf[i]));
				}
			}
		}

		if (!std::is_pod<T>::value && !std::is_base_of<MemoryMoveable, T>::value)
		{
			for (size_t i = 0; i < bufLen; ++i)		// clean up old data
			{
				buf[i].~T();
			}
		}
		free(buf);

		buf = newBuf;
		bufLen = newBufLen;
		// Front+++++++++++Rear............
		front = 0;
		rear = dataLen;
	}

	//template<typename T>
	//T const& Queue<T>::operator[](size_t idx) const
	//{
	//	return At(idx);
	//}

	//template<typename T>
	//T& Queue<T>::operator[](size_t idx)
	//{
	//	return At(idx);
	//}

	//template<typename T>
	//T const& Queue<T>::At(size_t idx) const
	//{
	//	return const_cast<Queue<T>*>(this)->At(idx);
	//}

	//template<typename T>
	//T& Queue<T>::At(size_t idx)
	//{
	//	assert(idx < Size());
	//	// = buf[ ( front + idx ) % bufLen ];  but slowly 3 times than following code
	//	if (front + idx >= bufLen)
	//	{
	//		return buf[front + idx - bufLen];
	//	}
	//	else
	//	{
	//		return buf[front + idx];
	//	}
	//}

	template <typename T>
	bool Queue<T>::Pop(T& outVal)
	{
		if (front == rear)
		{
			return false;
		}
		outVal = std::move(buf[front]);
		Pop();
		return true;
	}


}