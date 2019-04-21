#pragma once

namespace Ajy
{

	template<class T>
	struct EnableSharedFromThis;
	
	template<class T>
	struct SharedType
	{
		//typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type data;
		//char data[sizeof(T)];
		T* data = nullptr;
		int copys;
		int weaks;
		typedef std::function<void()> DT;
		DT deleter;
		
		template<class... Args>
		SharedType(Args&&... args)
			: copys(1)
			, weaks(0)
		{
			/*T* p = new ((T*)&data) T(std::forward<Args>(args)...);
			SaveWeak(p);*/
			data = new T(std::forward<Args>(args)...);
			SaveWeak(data);
		}
		~SharedType()
		{
		}
		T* Ptr()
		{
			//return (T*)&data;
			return data;
		}

		template<class OT>
		typename std::enable_if<!std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type SaveWeak(OT* o) {}
		template<class OT>
		typename std::enable_if<std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type SaveWeak(OT* o)
		{
			o->weak = this;
		}
	};

	template<class T>
	struct SharedPtr
	{
		typedef SharedPtr<T> MT;
		typedef typename SharedType<T>::DT DT;
		SharedType<T>* st;
		SharedPtr()
			:st(nullptr)
		{
		}
		void Clear()
		{
			if (!st) return;
			if (st->copys > 1)
			{
				--st->copys;
			}
			else // copys <= 1
			{
				st->Ptr()->~T();
				st->copys = 0;
				if (!st->weaks)
				{
					st->deleter();
				}
			}
			st = nullptr;
		}
		void Assign(SharedType<T>* _st, DT _deleter = nullptr)
		{
			assert(_st && _st->copys);
			Clear();
			++_st->copys;
			st = _st;
			if (_deleter) st->deleter = _deleter;
			else st->deleter = [_st] { delete _st; };
		}
		SharedPtr(SharedType<T>* _st, DT _deleter = nullptr)
		{
			st = _st;
			if (_deleter) st->deleter = _deleter;
			else st->deleter = [_st] { delete _st; };
		}
		SharedPtr(MT const& o)
		{
			if (!o.st || !o.st->copys)
			{
				st = nullptr;
			}
			else
			{
				st = o.st;
				++o.st->copys;
			}
		}
		SharedPtr(MT&& o)
		{
			st = o.st;
			o.st = nullptr;
		}
		SharedPtr& operator=(SharedType<T>* _st)
		{
			Clear();
			if (!_st || !_st->copys)
			{
				st = nullptr;
			}
			else
			{
				st = _st;
				++_st->copys;
			}
			return *this;
		}
		SharedPtr& operator=(MT const& o)
		{
			return operator=(o.st);
		}
		SharedPtr& operator=(MT&& o)
		{
			Clear();
			st = o.st;
			o.st = nullptr;
			return *this;
		}

		~SharedPtr()
		{
			Clear();
		}
		T* Ptr()
		{
			if (st == nullptr || !st->copys) return nullptr;
			return st->Ptr();
		}
		T* operator->()
		{
			return Ptr();
		}
		/*operator bool() const
		{
			if (st == nullptr || !st->copys) return false;
			return true;
		}*/
	};

	template<class T>
	struct WeakPtr
	{
		typedef WeakPtr<T> MT;
		SharedType<T>* st;
		void Clear()
		{
			if (!st) return;
			--st->weaks;
			if (!st->copys && !st->weaks)
			{
				st->deleter();
			}
			st = nullptr;
		}
		WeakPtr& operator=(SharedType<T>* _st)
		{
			Clear();
			if (_st && _st->copys)
			{
				st = _st;
				++_st->weaks;
			}
			else
			{
				st = nullptr;
			}
			return *this;
		}
		WeakPtr& operator=(SharedPtr<T> const& o)
		{
			return operator=(o.st);
		}
		WeakPtr& operator=(MT const& o)
		{
			return operator=(o.st);
		}
		WeakPtr& operator=(MT&& o)
		{
			Clear();
			st = o.st;
			o.st = nullptr;
			return *this;
		}
		WeakPtr()
			: st(nullptr)
		{
		}
		WeakPtr(SharedPtr<T> const& o)
		{
			if (o.st && o.st->copys)
			{
				st = o.st;
				++o.st->weaks;
			}
			else
			{
				st = nullptr;
			}
		}
		WeakPtr(MT const& o)
		{
			if (o.st && o.st->copys)
			{
				st = o.st;
				++o.st->weaks;
			}
			else
			{
				st = nullptr;
			}
		}
		WeakPtr(MT&& o)
		{
			st = o.st;
			o.st = nullptr;
		}
		WeakPtr(SharedType<T>* _st)
		{
			if (_st && _st->copys)
			{
				st = _st;
				++_st->weaks;
			}
			else
			{
				st = nullptr;
			}
		}
		~WeakPtr()
		{
			Clear();
		}
		T* Ptr()
		{
			if (st == nullptr || !st->copys) return nullptr;
			return st->Ptr();
		}
		T* operator->()
		{
			return Ptr();
		}
		SharedPtr<T> Share()
		{
			SharedPtr<T> rtv;
			if (!st) return rtv;
			rtv.st = st;
			++st->copys;
			return rtv;
		}
		/*operator bool() const
		{
			if (st == nullptr || !st->copys) return false;
			return true;
		}*/
	};


	template<class T>
	struct EnableSharedFromThis
	{
		WeakPtr<T> weak;
		SharedPtr<T> sharedFromThis()
		{
			return weak.Share();
		}
		friend SharedType<T>;
	};

	template<class T, class... Args>
	SharedPtr<T> MakeSharedPtr(Args&&... args)
	{
		return new SharedType<T>(std::forward<Args>(args)...);
	}

	template<class T, class... Args>
	SharedPtr<T> MakeSharedPtrEx(void* addr, typename SharedType<T>::DT _deleter, Args&&... args)
	{
		auto _st = new (addr)SharedType<T>(std::forward<Args>(args)...);
		return SharedPtr<T>(_st, _deleter);
	}

	template<class T, class... Args>
	SharedPtr<T> MakeSharedPtrEx(MemoryPool* pool, Args&&... args)
	{
		assert(pool->ItemSize() >= sizeof(SharedType<T>));
		auto addr = pool->Alloc();
		auto _st = new (addr)SharedType<T>(std::forward<Args>(args)...);
		return SharedPtr<T>(_st, [pool, addr] { pool->Free(addr); });
	}

}
