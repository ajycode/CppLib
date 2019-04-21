#pragma once

//#ifdef _MSC_VER
//#define aligned_alloc( alignment, size )   _aligned_malloc( size, alignment )
//#define aligned_free( aligned_ptr )       _aligned_free( aligned_ptr )
//#else
inline void* aligned_alloc(size_t alignment, size_t size)
{
	assert(!(alignment & (alignment - 1)));         //确保单元分配内存是2的n次幂
	size_t voidPtrSize = sizeof(void*);
	if (alignment < 2 * voidPtrSize)                 // 将alignment置为至少为2*sizeof(void*),一种优化做法。
	{
		alignment = 2 * voidPtrSize;
	}
	size_t offset = voidPtrSize + (--alignment);	// 计算出一个最大的offset，sizeof(void*)是为了存储原始指针地址  
	char* raw_malloc_ptr = (char*)malloc(offset + size);	// 分配一块带offset的内存  
	if (!raw_malloc_ptr)
	{
		throw std::bad_alloc(); // Report no memory
		//return nullptr;
	}

	// 向上舍入操作  
	// alignment - 1指明的是实际内存对齐的粒度  
	// 例如alignment = 8时, 我们只需要7就可以实际表示8个数(0~7)  
	// 那么~(alignment - 1)就是进行舍入的粒度  
	// 我们将(bytes) + alignment-1)就是先进行进位, 然后截断  
	// 这就保证了我是向上舍入的  
	// 例如bytes = 100, alignment = 8的情况  
	// ~(alignment - 1) = (1 000)B  
	// ((bytes) + alignment-1) = (1 101 011)B  
	// (((bytes) + alignment-1) & ~(alignment - 1)) = (1 101 000 )B = (104)D  
	// 104 / 8 = 13, 这就实现了向上舍入  
	// 对于byte刚好满足内存对齐的情况下, 结果保持byte大小不变  
	// 这个表达式与下面给出的等价,但是效率更高  
	// ((((bytes) + alignment - 1) * alignment) / alignment)   

	/*uintptr_t start = (uintptr_t)raw_malloc_ptr + voidPtrSize;
	void* aligned = (void*)( ( start + alignment - 1 ) & ~( alignment - 1 ) );*/

	// 通过“& (~alignment)”把多计算的offset减掉 
	void* aligned_ptr = reinterpret_cast<void*>(reinterpret_cast<size_t>(raw_malloc_ptr + offset) & (~alignment));
	// 将aligned_ptr当做一个指向void*的指针，在aligned_ptr当前地址前面放入原始地址
	((void**)aligned_ptr)[-1] = raw_malloc_ptr;
	// 返回经过对齐的内存地址  
	return aligned_ptr;
}
inline void aligned_free(void* aligned_ptr)
{
	// 还原回原始地址，并free  
	if (aligned_ptr)
	{
		free(static_cast<void**>(aligned_ptr)[-1]);
	}
}
//#endif