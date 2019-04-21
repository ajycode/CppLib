#pragma once

//#ifdef _MSC_VER
//#define aligned_alloc( alignment, size )   _aligned_malloc( size, alignment )
//#define aligned_free( aligned_ptr )       _aligned_free( aligned_ptr )
//#else
inline void* aligned_alloc(size_t alignment, size_t size)
{
	assert(!(alignment & (alignment - 1)));         //ȷ����Ԫ�����ڴ���2��n����
	size_t voidPtrSize = sizeof(void*);
	if (alignment < 2 * voidPtrSize)                 // ��alignment��Ϊ����Ϊ2*sizeof(void*),һ���Ż�������
	{
		alignment = 2 * voidPtrSize;
	}
	size_t offset = voidPtrSize + (--alignment);	// �����һ������offset��sizeof(void*)��Ϊ�˴洢ԭʼָ���ַ  
	char* raw_malloc_ptr = (char*)malloc(offset + size);	// ����һ���offset���ڴ�  
	if (!raw_malloc_ptr)
	{
		throw std::bad_alloc(); // Report no memory
		//return nullptr;
	}

	// �����������  
	// alignment - 1ָ������ʵ���ڴ���������  
	// ����alignment = 8ʱ, ����ֻ��Ҫ7�Ϳ���ʵ�ʱ�ʾ8����(0~7)  
	// ��ô~(alignment - 1)���ǽ������������  
	// ���ǽ�(bytes) + alignment-1)�����Ƚ��н�λ, Ȼ��ض�  
	// ��ͱ�֤���������������  
	// ����bytes = 100, alignment = 8�����  
	// ~(alignment - 1) = (1 000)B  
	// ((bytes) + alignment-1) = (1 101 011)B  
	// (((bytes) + alignment-1) & ~(alignment - 1)) = (1 101 000 )B = (104)D  
	// 104 / 8 = 13, ���ʵ������������  
	// ����byte�պ������ڴ����������, �������byte��С����  
	// ������ʽ����������ĵȼ�,����Ч�ʸ���  
	// ((((bytes) + alignment - 1) * alignment) / alignment)   

	/*uintptr_t start = (uintptr_t)raw_malloc_ptr + voidPtrSize;
	void* aligned = (void*)( ( start + alignment - 1 ) & ~( alignment - 1 ) );*/

	// ͨ����& (~alignment)���Ѷ�����offset���� 
	void* aligned_ptr = reinterpret_cast<void*>(reinterpret_cast<size_t>(raw_malloc_ptr + offset) & (~alignment));
	// ��aligned_ptr����һ��ָ��void*��ָ�룬��aligned_ptr��ǰ��ַǰ�����ԭʼ��ַ
	((void**)aligned_ptr)[-1] = raw_malloc_ptr;
	// ���ؾ���������ڴ��ַ  
	return aligned_ptr;
}
inline void aligned_free(void* aligned_ptr)
{
	// ��ԭ��ԭʼ��ַ����free  
	if (aligned_ptr)
	{
		free(static_cast<void**>(aligned_ptr)[-1]);
	}
}
//#endif