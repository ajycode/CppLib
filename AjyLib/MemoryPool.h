#pragma once

namespace Ajy
{

	class MemoryPool : MemoryMoveable
	{
	public:

		MemoryPool() {};
		MemoryPool(size_t itemSize, size_t pageSize);
		~MemoryPool();
		MemoryPool(MemoryPool&& copy);
		MemoryPool& operator=(MemoryPool&& copy);				
		MemoryPool(MemoryPool const& copy) = delete;
		MemoryPool& operator=(MemoryPool const& copy) = delete;

		void Init(size_t itemSize, size_t pageSize);
		template<typename T>
		inline void Init(size_t pageItemCount = 8)
		{
			Init(MAX(sizeof(T), sizeof(void*)), pageItemCount * MAX(sizeof(T), sizeof(void*)));
		}

		void SetPageSize(size_t newPageSize);
		void SetPageSizeByItemCount(size_t itemCount);
		void NewPage();

		// c style funcs
		void* Alloc();
		void* FastAlloc();							// unsafe: do not ensure the space is enough
		void Free(void* p);

		// c++ style funcs	
		template<class T, class... Args>
		inline T* New(Args&&... args)
		{
			assert(itemSize >= sizeof(T));
			auto p = Alloc();
			return new (p) T(std::forward<Args>(args)...);
		}

		template<typename T>
		inline void Delete(T* p)
		{
			p->T::~T();
			Free(p);
		}

		size_t ItemSize() const
		{
			return itemSize;
		}
	private:
		void ReSet();
		void Clear();
	private:
		void* header = nullptr;					// current alloc ptr( *header = next ptr )
	public:
		size_t itemSize = 0, pageSize = 0;		// size: aligned byte count
		size_t allPageItemCounter = 0;
#ifdef Ajy_DEBUG
		size_t allocatedCounter = 0;
#endif
	private:
		Vector<void*> pages;
	} const emptyPool;

}
