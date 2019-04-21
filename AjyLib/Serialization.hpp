#pragma once

namespace Ajy
{

	template<class T>
	void ByteBuffer::WriteCore(char* dest, T const& src)
	{
#ifdef Ajy_IA
		*(T*)dest = src;
#else
		char* p = (char*)&src;
		if (sizeof(T) >= 1)
		{
			dest[0] = p[0];
		}
		if (sizeof(T) >= 2)
		{
			dest[1] = p[1];
		}
		if (sizeof(T) >= 4)
		{
			dest[2] = p[2];
			dest[3] = p[3];
		}
		if (sizeof(T) == 8)
		{
			dest[4] = p[4];
			dest[5] = p[5];
			dest[6] = p[6];
			dest[7] = p[7];
		}
#endif
	}



	template<class T>
	void ByteBuffer::ReadCore(T& dest, char const* src)
	{
#ifdef Ajy_IA
		dest = *(T*)src;
#else
		char* p = (char*)&dest;
		if (sizeof(T) >= 1)
		{
			p[0] = src[0];
		}
		if (sizeof(T) >= 2)
		{
			p[1] = src[1];
		}
		if (sizeof(T) >= 4)
		{
			p[2] = src[2];
			p[3] = src[3];
		}
		if (sizeof(T) == 8)
		{
			p[4] = src[4];
			p[5] = src[5];
			p[6] = src[6];
			p[7] = src[7];
		}
#endif
	}





	HAS_MEMBER_FUNCTION(HasFunc_WriteTo, WriteTo, void (T::*)(ByteBuffer&) const)
	template<class T>
	typename std::enable_if<HasFunc_WriteTo<T>::value, void>::type WriteSwitch(ByteBuffer& b, T const& v)
	{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		if (b.ptrStore)
		{
			std::pair<typename HashMap<void*, size_t>::Node*,bool> rtv = b.ptrStore->Add((void*)&v, (uint32)b.dataLen);
			b.VarWrite(rtv.first->value);
			if (!rtv.second) return;
		}
#endif
		v.WriteTo(b);
	};

	template<class T>
	typename std::enable_if<!HasFunc_WriteTo<T>::value, void>::type WriteSwitch(ByteBuffer& b, T const& v)
	{
		static_assert(std::is_pod<T>::value, "forget impl WriteTo func ?");
		b.Reserve(b.dataLen + sizeof(T));
		ByteBuffer::WriteCore(b.buf + b.dataLen, v);
		b.dataLen += sizeof(T);
	};

	template<class T>
	void ByteBuffer::Write(T const& v)
	{
		WriteSwitch(*this, v);
	}

	template<class T>
	void ByteBuffer::WriteMultiCore(T const& v)
	{
		Write(v);
	}

	template<class T, class... Args>
	void ByteBuffer::WriteMultiCore(T const& v, Args const&... args)
	{
		Write(v);
		WriteMultiCore(args...);
	}

	template<class K, class V>
	void ByteBuffer::Write(std::pair<K, V> const& v)
	{
		Write(v.first);
		Write(v.second);
	}

	template<class... Args>
	void ByteBuffer::WriteMulti(Args const&... args)
	{
		WriteMultiCore(args...);
	}


#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
	template<class T>
	void ByteBuffer::Write(T* v)
	{
		PtrStoreInit();
		if (v)
		{
			Write(true);
			Write(*v);
		}
		else
		{
			Write(false);
		}
	}

	template<class T>
	void ByteBuffer::RootWrite(T const& v)
	{
		PtrStoreInit();
		Write(v);
	}

	template<class T>
	bool ByteBuffer::Read(T*& v)
	{
		IdxStoreInit();
		bool b;
		if (!Read(b)) return false;
		if (b)
		{
			uint32 _offset, _offset_bak = (uint32)offset;
			if (!TryVarRead(_offset)) return false;
			if (_offset == _offset_bak)
			{
				assert(!v);                       // todo: obj pool support ?
				v = new T();
				if (!Read(*v))
				{
					delete v;
					v = nullptr;
					return false;
				}
				idxStore->Add(_offset, v);     // todo: check
			}
			else
			{
				v = (T*)idxStore->At(_offset);
			}
		}
		else
		{
			v = nullptr;
		}
		return true;
	}

	template<class T>
	bool ByteBuffer::RootRead(T& v)
	{
		IdxStoreInit();
		return Read(v);
	}
#endif



	HAS_MEMBER_FUNCTION(HasFunc_ReadFrom, ReadFrom, bool (T::*)(ByteBuffer&))
	template<class T>
	typename std::enable_if<HasFunc_ReadFrom<T>::value, bool>::type ReadSwitch(ByteBuffer& b, T& v)
	{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		if (b.idxStore)
		{
			uint32  _offset;
			if (!b.VarRead(_offset)) return false;
			b.idxStore->Add(_offset, (void*)&v);   // todo: assert?
		}
#endif
		return v.ReadFrom(b);
	};

	template<class T>
	typename std::enable_if<!HasFunc_ReadFrom<T>::value, bool>::type ReadSwitch(ByteBuffer& b, T& v)
	{
		assert(std::is_pod<T>::value);
		if (b.offset + sizeof(T) > b.dataLen) return false;
		ByteBuffer::ReadCore(v, b.buf + b.offset);
		b.offset += sizeof(T);
		return true;
	};

	template<class T>
	bool ByteBuffer::Read(T& v)
	{
		return ReadSwitch(*this, v);
	}

	template<class K, class V>
	bool ByteBuffer::Read(std::pair<K, V>& v)
	{
		if (!Read(v.first)) return false;
		return Read(v.second);
	}

	template<class T>
	bool ByteBuffer::Read(T& v, size_t minLen, size_t maxLen)
	{
		return v.ReadFrom(*this, minLen, maxLen);
	}

	template<class T>
	bool ByteBuffer::ReadMultiCore(T& v)
	{
		return Read(v);
	}

	template<class T, class... Args>
	bool ByteBuffer::ReadMultiCore(T& v, Args&... args)
	{
		if (!Read(v)) return false;
		return ReadMultiCore(args...);
	}

	template<class... Args>
	bool ByteBuffer::ReadMulti(Args&... args)
	{
		return ReadMultiCore(args...);
	}





	HAS_MEMBER_FUNCTION(HasFunc_VarWriteTo, VarWriteTo, void (T::*)(ByteBuffer&) const)
	template<class T>
	typename std::enable_if<HasFunc_VarWriteTo<T>::value, void>::type VarWriteSwitch(ByteBuffer& b, T const& v)
	{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		if (b.ptrStore)
		{
			std::pair<typename HashMap<void*, size_t>::Node*, bool> rtv = b.ptrStore->Add((void*)&v, (uint32)b.dataLen);
			b.VarWrite(rtv.first->value);
			if (!rtv.second) return;
		}
#endif
		v.VarWriteTo(b);
	};

	template<class T>
	typename std::enable_if<!HasFunc_VarWriteTo<T>::value, void>::type VarWriteSwitch(ByteBuffer& b, T const& v)
	{
		static_assert(std::is_pod<T>::value, "forget impl VarWriteTo func ?");
		b.Reserve(b.dataLen + sizeof(T));
		ByteBuffer::WriteCore(b.buf + b.dataLen, v);
		b.dataLen += sizeof(T);
	};

	template<class T>
	void ByteBuffer::VarWrite(T const& v)
	{
		VarWriteSwitch(*this, v);
	}

	template<class T>
	void ByteBuffer::VarWriteMultiCore(T const& v)
	{
		VarWrite(v);
	}

	template<class T, class... Args>
	void ByteBuffer::VarWriteMultiCore(T const& v, Args const&... args)
	{
		VarWrite(v);
		VarWriteMultiCore(args...);
	}

	template<class K, class V>
	void ByteBuffer::VarWrite(std::pair<K, V> const& v)
	{
		VarWrite(v.first);
		VarWrite(v.second);
	}

	template<class... Args>
	void ByteBuffer::VarWriteMulti(Args const&... args)
	{
		VarWriteMultiCore(args...);
	}




	HAS_MEMBER_FUNCTION(HasFunc_VarReadFrom, VarReadFrom, bool (T::*)(ByteBuffer&))
	template<class T>
	typename std::enable_if<HasFunc_VarReadFrom<T>::value, bool>::type VarReadSwitch(ByteBuffer& b, T& v)
	{
#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		if (b.idxStore)
		{
			uint32 _offset;
			if (!b.VarRead(_offset)) return false;
			b.idxStore->Add(_offset, (void*)&v);   // todo: assert?
		}
#endif
		return v.VarReadFrom(b);
	};
	template<class T>
	typename std::enable_if<!HasFunc_VarReadFrom<T>::value, bool>::type VarReadSwitch(ByteBuffer& b, T& v)
	{
		assert(std::is_pod<T>::value);
		if (b.offset + sizeof(T) > b.dataLen) return false;
		ByteBuffer::ReadCore(v, b.buf + b.offset);
		b.offset += sizeof(T);
		return true;
	};
	template<class T>
	bool ByteBuffer::VarRead(T& v)
	{
		return VarReadSwitch(*this, v);
	}

	template<class T>
	bool ByteBuffer::VarRead(T& v, size_t minLen, size_t maxLen)
	{
		return v.VarReadFrom(*this, minLen, maxLen);
	}

	template<class T>
	bool ByteBuffer::VarReadMultiCore(T& v)
	{
		return VarRead(v);
	}

	template<class T, class... Args>
	bool ByteBuffer::VarReadMultiCore(T& v, Args&... args)
	{
		if (!VarRead(v)) return false;
		return VarReadMultiCore(args...);
	}

	template<class K, class V>
	bool ByteBuffer::VarRead(std::pair<K, V>& v)
	{
		if (!VarRead(v.first)) return false;
		return VarRead(v.second);
	}

	template<class... Args>
	bool ByteBuffer::VarReadMulti(Args&... args)
	{
		return VarReadMultiCore(args...);
	}

}
