#include "AjyLib.h"

namespace Ajy
{

	MemoryPool::MemoryPool(size_t itemSize, size_t pageSize)
	{
		assert(itemSize >= sizeof(void*));
		Init(itemSize, pageSize);
	}

	MemoryPool::MemoryPool(MemoryPool&& copy)
	{
		pages = std::move(copy.pages);
		header = copy.header;
		itemSize = copy.itemSize;
		pageSize = copy.pageSize;
		allPageItemCounter = copy.allPageItemCounter;
#ifdef Ajy_DEBUG
		allocatedCounter = copy.allocatedCounter;
#endif
		copy.ReSet();
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& copy)
	{
		if (this != &copy)
		{
			for (size_t i = 0; i < pages.DataLen(); ++i)
			{
				aligned_free(pages[i]);
			}
			pages = std::move(copy.pages);
			header = copy.header;
			itemSize = copy.itemSize;
			pageSize = copy.pageSize;
			allPageItemCounter = copy.allPageItemCounter;
#ifdef Ajy_DEBUG
			allocatedCounter = copy.allocatedCounter;
#endif
			copy.ReSet();
		}
		return *this;
	}

	MemoryPool::~MemoryPool()
	{
		Clear();
	}

	void MemoryPool::Init(size_t itemSize_, size_t pageSize_)
	{
		assert(!itemSize);				// ensure init only execute 1 times
		if (itemSize_ < sizeof(void*))
		{
			itemSize_ = sizeof(void*);
		}
		itemSize = Round2n(itemSize_);
		SetPageSize(pageSize_);
		NewPage();
	}

	void MemoryPool::SetPageSize(size_t newPageSize)
	{
		assert(itemSize);
		if (newPageSize < itemSize)
		{
			newPageSize = itemSize;
		}
		pageSize = Round2n(newPageSize);
	}

	void MemoryPool::SetPageSizeByItemCount(size_t itemCount)
	{
		assert(itemSize);
		assert(itemCount);
		pageSize = Round2n(itemCount * itemSize);
	}

	Ajy_NOINLINE void MemoryPool::NewPage()
	{
		assert(itemSize);					// need init
		allPageItemCounter += (pageSize / itemSize);
		char* p = (char*)aligned_alloc(itemSize, pageSize);
		if (!p)
		{
			return;
		}
		/*while(!p)
		{
			AlignedFree(p);
			pageSize /= 2;
			p = (char*)AlignedAlloc(itemSize, pageSize);
		}*/
		pages.Push(p);
		char* bak = p;
		char* pp = p + pageSize - itemSize;	// - itemSize: skip last cell
		for (; p < pp; p += itemSize)
		{
			*(void**)p = p + itemSize;		// cell1 = &cell2
		}
		*(void**)p = header;				// last cell link to header or set nullptr
		header = bak;
		pageSize *= 2;
	}

	void* MemoryPool::Alloc()
	{
		assert(itemSize);					// need init
		if (!header)
		{
			NewPage();
		}
		void* rtv = header;
		header = *(void**)header;
#ifdef Ajy_DEBUG
		++allocatedCounter;
#endif
		return rtv;
	}

	void* MemoryPool::FastAlloc()
	{
		assert(itemSize);					// need init
		void* rtv = header;
		header = *(void**)header;
#ifdef Ajy_DEBUG
		++allocatedCounter;
#endif
		return rtv;
	}

	void MemoryPool::Free(void* p)
	{
		assert(itemSize);					// need init
		assert(p);
		*(void**)p = header;
		header = p;
#ifdef Ajy_DEBUG
		--allocatedCounter;
#endif
	}

	void MemoryPool::ReSet()
	{
		header = nullptr;
		itemSize = 0;
		pageSize = 0;
		allPageItemCounter = 0;
#ifdef Ajy_DEBUG
		allocatedCounter = 0;
#endif
	}

	void MemoryPool::Clear()
	{
		for (size_t i = 0; i < pages.DataLen(); ++i)
		{
			aligned_free(pages[i]);
		}
		pages.Clear();
		ReSet();
	}

}
