#pragma once

namespace Ajy
{

	template <class TK, class TV>
	Dictionary<TK, TV>::Dictionary(size_t capacity)
		: freeList(-1)
		, freeCount(0)
		, count(0)
		, bucketsLen(Ajy::GetPrime(Ajy::Round2n(capacity)))
	{
		buckets = new int[bucketsLen];
		entries = new NT[bucketsLen];
		values = new TVA[bucketsLen];
		memset(buckets, -1, bucketsLen * sizeof(int));          // -1 代表 "空"
	}

	template <class TK, class TV>
	Dictionary<TK, TV>::Dictionary()
		: Dictionary(8)
	{
	}

	template <class TK, class TV>
	Dictionary<TK, TV>::Dictionary(Dictionary const& o)
		: Dictionary(o.count)
	{
		FastAdd(o);
	}

	template <class TK, class TV>
	Dictionary<TK, TV>::Dictionary(Dictionary&& o)
		: freeList(o.freeList)
		, freeCount(o.freeCount)
		, count(o.count)
		, bucketsLen(o.bucketsLen)
		, buckets(o.buckets)
		, entries(o.entries)
		, values(o.values)
	{
		o.buckets = nullptr;
	}

	template <class TK, class TV>
	Dictionary<TK, TV>::~Dictionary()
	{
		if (!buckets)                      // 判断一个就够了. 右值的情况下会为 nullptr
		{
			return;
		}
		DeleteAll();
	}

	template <class TK, class TV>
	Dictionary<TK, TV>& Dictionary<TK, TV>::operator =(Dictionary const& o)
	{
		DeleteKVs();
		// 确保 this.bucketsLen 大于等于 o.count( 不够就扩容 )
		if (bucketsLen < o.count)
		{
			size_t capacity = Ajy::GetPrime(Ajy::Round2n(o.count));
			delete[] buckets;
			buckets = new int[capacity];
			delete[] entries;
			entries = new NT[capacity];
			delete[] values;
			values = new TVA[capacity];
			bucketsLen = capacity;
		}
		memset(buckets, -1, bucketsLen * sizeof(int));
		freeList = -1;
		freeCount = 0;
		////count = 0;      // FastAdd 里面会填
		// 复制数据
		FastAdd(o);
		return *this;
	}

	template <class TK, class TV>
	Dictionary<TK, TV>& Dictionary<TK, TV>::operator =(Dictionary&& o)
	{
		DeleteAll();
		freeList = o.freeList;
		freeCount = o.freeCount;
		count = o.count;
		bucketsLen = o.bucketsLen;
		buckets = o.buckets;
		entries = o.entries;
		values = o.values;
		o.buckets = nullptr;
	}

	template <class TK, class TV>
	template<class K, class... Args>
	std::pair<bool, int> Dictionary<TK, TV>::EmplaceAdd(bool replace, K&& key, Args&&... args)
	{
		// hash 按桶数取模 定位到具体 链表, 扫找
		int hashCode = MyGetHashCode(std::forward<K>(key));
		int targetBucket = int((unsigned int)hashCode % bucketsLen);
		for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
		{
			if (entries[i].hashCode == hashCode && Ajy::Equals(RefKat(i), key))
			{
				if (replace)                                      // 允许覆盖 value
				{
					RefVat(i) = TV(std::forward<Args>(args)...);
					return std::make_pair(true, i);
				}
				return std::make_pair(false, i);
			}
		}
		// 没找到则新增
		int index;
		if (freeCount > 0)                  // 如果 自由节点链表 不空, 取一个来当容器
		{                                   // 这些节点来自 Remove 操作. next 指向下一个
			index = freeList;
			freeList = entries[index].next;
			freeCount--;
		}
		else
		{
			if (count == bucketsLen)               // 所有空节点都用光了, Resize
			{
				Reserve();
				targetBucket = int((unsigned int)hashCode % bucketsLen);
			}
			index = count;                         // 指向 Resize 后面的空间起点
			count++;
		}

#if ENABLE_FAST_REMOVE_AT_INDEX
		// 如果当前 bucket 中有 index, 则令其 prev 等于即将添加的 index
		if (buckets[targetBucket] >= 0)
		{
			entries[buckets[targetBucket]].prev = index;
		}
		entries[index].prev = -1;
#endif

		// 执行到此处时, freeList 要么来自 自由节点链表, 要么为 Resize 后面的空间起点.
		entries[index].hashCode = hashCode;
		entries[index].next = buckets[targetBucket];
		buckets[targetBucket] = index;

		// 移动复制构造写 key, value
		new (&entries[index].key) TK(std::forward<K>(key));
		new (&values[index]) TV(std::forward<Args>(args)...);
		return std::make_pair(true, index);
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::Reserve(size_t capacity/* = 0*/)
	{
		if (capacity == 0)
		{
			// 2倍扩容法, 找一个刚好小于 2^n 的质数来做桶长
			capacity = count * 2;
		}
		else if (capacity <= bucketsLen)
		{
			return;
		}
		else
		{
			capacity = Ajy::GetPrime(Ajy::Round2n(capacity));
		}
		// 桶扩容并初始化
		delete[] buckets;
		buckets = new int[capacity];
		memset(buckets, -1, capacity * sizeof(int));
		bucketsLen = capacity;

		// 节点数组扩容
		NT* oldEntries = entries;
		entries = new NT[capacity];
		if (std::is_base_of<MemoryMoveable, TK>::value || std::is_pod<TK>::value)
		{
			memcpy(entries, oldEntries, sizeof(NT) * count);
		}
		else                        // 对于 pod 或标记为 可直接复制的数据类型, 用 memcpy
		{                           // 其他情形走 移动复制构造函数 + 析构旧数据( 下同 )
			for (int i = 0; i < count; ++i)
			{
				entries[i].hashCode = oldEntries[i].hashCode;
				// next 啥的 不必复制
				TK& oldk = RefK(oldEntries[i].key);
				new (&entries[i].key) TK((TK&&)oldk);
				oldk.TK::~TK();
			}
		}
		delete[] oldEntries;

		// 值数组扩容
		TVA* oldValues = values;
		values = new TVA[capacity];
		if (std::is_base_of<MemoryMoveable, TV>::value || std::is_pod<TV>::value)
		{
			memcpy(values, oldValues, sizeof(TVA) * count);
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
			{
				TV& oldv = RefV(oldValues[i]);
				new (&values[i]) TV((TV&&)oldv);
				oldv.TV::~TV();
			}
		}
		delete[] oldValues;

		// 遍历所有节点, 重构桶及链表
		for (size_t j = 0; j < count; j++)
		{
			if (entries[j].hashCode >= 0)
			{
				int index = int((unsigned int)entries[j].hashCode % capacity);
#if ENABLE_FAST_REMOVE_AT_INDEX
				if (buckets[index] >= 0)
				{
					entries[buckets[index]].prev = j;
				}
				entries[j].prev = -1;
#endif
				entries[j].next = buckets[index];
				buckets[index] = j;
			}
		}
	}

	template <class TK, class TV>
	int Dictionary<TK, TV>::Find(TK const& key) const
	{
		int hashCode = MyGetHashCode(key);
		for (int i = buckets[(unsigned int)hashCode % bucketsLen]; i >= 0; i = entries[i].next)
		{
			if (entries[i].hashCode == hashCode && Ajy::Equals(RefKat(i), key))
			{
				return i;
			}
		}
		return -1;
	}

	template <class TK, class TV>
	int Dictionary<TK, TV>::Remove(TK const& key)
	{
		int hashCode = MyGetHashCode(key);
		int targetBucket = int((unsigned int)hashCode % bucketsLen);
		int last = -1;                                          // 存 "上一个节点". -1 表示节点位于桶

		for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
		{
			if (entries[i].hashCode == hashCode && Ajy::Equals(RefKat(i), key))
			{
				if (last < 0)
				{
					// 没有上一个节点
					buckets[targetBucket] = entries[i].next;
				}
				else
				{
					// 上一个节点. next 原本指向 当前节点, 令其指向 当前节点指向的下一个节点
					entries[last].next = entries[i].next;
#if ENABLE_FAST_REMOVE_AT_INDEX
					// 如果存在当前节点的下一个节点, 令其 prev 指向 上一个节点
					if (entries[i].next >= 0)
					{
						entries[entries[i].next].prev = last;
					}
#endif
				}

				// 当前节点已被移出链表, 令其 next 指向  自由节点链表头( next 有两种功用 )
				entries[i].next = freeList;
				entries[i].hashCode = -1;
				freeList = i;
				freeCount++;

				// 析构内存结构
				RefKat(i).TK::~TK();
				RefVat(i).TV::~TV();

				return i;
			}
			last = i;
		}
		return -1;
	}

	template <class TK, class TV>
	size_t Dictionary<TK, TV>::Size() const
	{
		return size_t(count - freeCount);
	}

	template <class TK, class TV>
	std::pair<bool, int> Dictionary<TK, TV>::Add(TK const& k, TV const& v, bool replace = false)
	{
		return EmplaceAdd(replace, k, v);
	}

	template <class TK, class TV>
	std::pair<bool, int> Dictionary<TK, TV>::Add(TK const& k, TV&& v, bool replace = false)
	{
		return EmplaceAdd(replace, k, (TV&&)v);
	}

	template <class TK, class TV>
	std::pair<bool, int> Dictionary<TK, TV>::Add(TK&& k, TV const& v, bool replace = false)
	{
		return EmplaceAdd(replace, (TK&&)k, v);
	}

	template <class TK, class TV>
	std::pair<bool, int> Dictionary<TK, TV>::Add(TK&& k, TV&& v, bool replace = false)
	{
		return EmplaceAdd(replace, (TK&&)k, (TV&&)v);
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::Clear()
	{
		if (!count)
		{
			return;
		}
		DeleteKVs();
		memset(buckets, -1, bucketsLen * sizeof(int));
		freeList = -1;
		freeCount = 0;
		count = 0;
	}

	template <class TK, class TV>
	TV* Dictionary<TK, TV>::TryGetValue(TK const& key)
	{
		int idx = Find(key);
		if (idx >= 0)
		{
			return RefVat(idx);
		}
		return nullptr;
	}

	template <class TK, class TV>
	template<class K>
	TV& Dictionary<TK, TV>::operator[](K&& key)
	{
		int idx = Find(key);
		if (idx < 0)
		{
			idx = EmplaceAdd(true, std::forward<K>(key), TV()).second;
		}
		return RefVat(idx);
	}

	template <class TK, class TV>
	template<class K>
	TV& Dictionary<TK, TV>::At(K&& key)
	{
		return operator[](std::forward<K>(key));
	}

	template <class TK, class TV>
	TK& Dictionary<TK, TV>::KeyIndexAt(int idx)
	{
		assert(idx < count);
		return RefKat(idx);
	}

	template <class TK, class TV>
	TV& Dictionary<TK, TV>::IndexAt(int idx)
	{
		assert(idx < count);
		return RefVat(idx);
	}

	template <class TK, class TV>
	TV const& Dictionary<TK, TV>::IndexAt(int idx) const
	{
		return const_cast<Dictionary*>(this)->IndexAt(idx);
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::RemoveIndexAt(int idx)
	{
#if ENABLE_FAST_REMOVE_AT_INDEX
		if (entries[idx].prev >= 0)
		{
			entries[entries[idx].prev].next = entries[idx].next;
		}
		else
		{
			int targetBucket = int((unsigned int)entries[idx].hashCode % bucketsLen);
			buckets[targetBucket] = entries[idx].next;
		}
		if (entries[idx].next >= 0)
		{
			entries[entries[idx].next].prev = entries[idx].prev;
		}
		entries[idx].hashCode = -1;
		entries[idx].next = freeList;
		freeList = idx;
		freeCount++;

		RefKat(idx).TK::~TK();
		RefVat(idx).TV::~TV();
#else
		int targetBucket = int((unsigned int)entries[idx].hashCode % bucketsLen);
		int last = -1;                                          // 存 "上一个节点". -1 表示节点位于桶

		for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
		{
			if (i == idx)
			{
				if (last < 0)
				{
					buckets[targetBucket] = entries[i].next;
				}
				else
				{
					entries[last].next = entries[i].next;
				}

				entries[i].next = freeList;
				freeList = i;
				freeCount++;

				RefKat(i).TK::~TK();
				RefVat(i).TV::~TV();

				return;
			}
			last = i;
		}
		assert(false);
#endif
	}

	template <class TK, class TV>
	bool Dictionary<TK, TV>::Empty() const
	{
		return count == 0;
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::Foreach(std::function<bool(TK const& key, TV& value)> handler)
	{
		for (size_t j = 0; j < count; ++j)
		{
			if (entries[j].hashCode >= 0)
			{
				if (!handler(RefKat(j), RefVat(j)))
				{
					return;
				}
			}
		}
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::Foreach(std::function<bool(TK const& key, TV& value)> handler) const
	{
		return const_cast<Dictionary*>(this)->Foreach(handler);
	}

	template <class TK, class TV>
	TK& Dictionary<TK, TV>::RefK(TKA& ka)
	{
		return *(TK*)&ka;
	}

	template <class TK, class TV>
	TK& Dictionary<TK, TV>::RefKat(int idx)
	{
		return *(TK*)&entries[idx].key;
	}

	template <class TK, class TV>
	TK const& Dictionary<TK, TV>::RefKat(int idx) const
	{
		return *(TK*)&entries[idx].key;
	}

	template <class TK, class TV>
	TV& Dictionary<TK, TV>::RefV(TVA& va)
	{
		return *(TV*)&va;
	}

	template <class TK, class TV>
	TV& Dictionary<TK, TV>::RefVat(int idx)
	{
		return *(TV*)&values[idx];
	}

	template <class TK, class TV>
	const TV& Dictionary<TK, TV>::RefVat(int idx) const
	{
		return const_cast<Dictionary*>(this)->RefVat(idx);
	}

	template <class TK, class TV>
	template<class T>
	static int Dictionary<TK, TV>::MyGetHashCode(T const& v)
	{
		return int(Ajy::GetHashCode(v) & 0x7FFFFFFF);
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::FastAdd(Dictionary const& o)
	{
		size_t i = 0;
		for (size_t oj = 0; oj < o.count; ++oj)
		{
			if (o.entries[i].hashCode >= 0)
			{
				int targetBucket = int((unsigned int)o.entries[oj].hashCode % bucketsLen);
				entries[i].hashCode = o.entries[oj].hashCode;
				entries[i].next = buckets[targetBucket];
				buckets[targetBucket] = i;
				new (&entries[i].key) TK(RefK(o.entries[oj].key));
				new (&values[i]) TV(RefV(o.values[oj]));
				++i;
			}
		}
		count = i;
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::DeleteKVs()
	{
		for (size_t j = 0; j < count; ++j)
		{
			if (entries[j].hashCode >= 0)
			{
				RefKat(j).TK::~TK();
				RefVat(j).TV::~TV();
			}
		}
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::DeleteAll()
	{
		DeleteKVs();
		delete[] buckets;
		delete[] entries;
		delete[] values;
	}

}
