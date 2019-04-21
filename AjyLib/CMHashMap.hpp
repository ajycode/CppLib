#pragma once

namespace Ajy
{

	// ContiguousMemoryHashMap
	template <class Key, class Value>
	struct CMHashMap : MemoryMoveable
	{
		struct Node
		{
			size_t    hashCode;
			int             next;
		};
		struct Data
		{
			Key              key;
			Value              value;
			int             prev;
		};
		
		int					freeList;               // 自由空间链表头( next 指向下一个未使用单元 )
		size_t              freeCount;              // 自由空间链长
		size_t              count;                  // 已使用空间数
		size_t              bucketsLen;             // 桶数组长( 质数 )
		int                *buckets;                // 桶数组
		Node               *nodes;                  // 节点数组
		Data               *items;                  // 数据数组( 与节点数组同步下标 )

		explicit CMHashMap(size_t capacity)
			: freeList(-1)
			, freeCount(0)
			, count(0)
			, bucketsLen(Ajy::GetPrime(capacity, false))
		{
			Alloc();
		}

		CMHashMap()
			: CMHashMap(4)
		{
		}

		CMHashMap(CMHashMap const& o)
			: CMHashMap(o.count)
		{
			CopyFrom(o);
		}

		CMHashMap(CMHashMap&& o)
			: freeList(o.freeList)
			, freeCount(o.freeCount)
			, count(o.count)
			, bucketsLen(o.bucketsLen)
			, buckets(o.buckets)
			, nodes(o.nodes)
			, items(o.items)
		{
			o.buckets = nullptr;
			o.nodes = nullptr;
			o.items = nullptr;
			o.count = 0;
			o.freeCount = 0;
			o.freeList = -1;
			o.bucketsLen = 0;
		}

		CMHashMap& operator=(CMHashMap const& o)
		{
			if (bucketsLen < o.count)           // 确保 this.bucketsLen 大于等于 o.count( 不够就扩容 )
			{
				Destory();
				bucketsLen = Ajy::GetPrime(o.count, false);
				Alloc();
			}
			else
			{
				Dispose();
				memset(buckets, -1, bucketsLen * sizeof(int));
			}
			freeList = -1;
			freeCount = 0;

			CopyFrom(o);     // 复制数据 并设置 count 的值
			return *this;
		}

		CMHashMap& operator=(CMHashMap&& o)
		{
			Destory();

			freeList = o.freeList;
			freeCount = o.freeCount;
			count = o.count;
			bucketsLen = o.bucketsLen;
			buckets = o.buckets;
			nodes = o.nodes;
			items = o.items;
			o.buckets = nullptr;
			o.nodes = nullptr;
			o.items = nullptr;
			o.count = 0;
			o.freeCount = 0;
			o.freeList = -1;
			o.bucketsLen = 0;
		}

		~CMHashMap()
		{
			if (!buckets) return;               // 判断一个就够了. 右值的情况下会为 nullptr
			Destory();
		}

		template<class K, class... Args>
		std::pair<bool, int> EmplaceAdd(bool override, K&& key, Args&&... args)
		{
			// hash 按桶数取模 定位到具体 链表, 扫找
			size_t hashCode = Ajy::GetHashCode(key);
			size_t targetBucket = hashCode % bucketsLen;
			for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
			{
				if (nodes[i].hashCode == hashCode && Ajy::Equals(items[i].key, key))
				{
					if (override)                       // 允许覆盖 value
					{
						items[i].value = Value(std::forward<Args>(args)...);
					}
					return std::make_pair(false, i);
				}
			}
			// 没找到则新增
			int index;
			if (freeCount > 0)                          // 如果 自由节点链表 不空, 取一个来当容器
			{                                           // 这些节点来自 Remove 操作. next 指向下一个
				index = freeList;
				freeList = nodes[index].next;
				--freeCount;
			}
			else
			{
				if (count == bucketsLen)               // 所有空节点都用光了, Resize
				{
					Reserve();
					targetBucket = hashCode % bucketsLen;
				}
				index = count;                         // 指向 Resize 后面的空间起点
				++count;
			}

			// 执行到此处时, freeList 要么来自 自由节点链表, 要么为 Reserve 后新增的空间起点.
			nodes[index].hashCode = hashCode;
			nodes[index].next = buckets[targetBucket];

			// 如果当前 bucket 中有 index, 则令其 prev 等于即将添加的 index
			if (buckets[targetBucket] >= 0)
			{
				items[buckets[targetBucket]].prev = index;
			}
			buckets[targetBucket] = index;

			// 移动复制构造写 key, value
			new (&items[index].key) Key(std::forward<K>(key));
			new (&items[index].value) Value(std::forward<Args>(args)...);
			items[index].prev = -1;

			return std::make_pair(true, index);
		}


		std::pair<bool, int> Add(Key const& k, Value const& v, bool override = false)
		{
			return EmplaceAdd(override, k, v);
		}
		std::pair<bool, int> Add(Key const& k, Value&& v, bool override = false)
		{
			return EmplaceAdd(override, k, (Value&&)v);
		}
		std::pair<bool, int> Add(Key&& k, Value const& v, bool override = false)
		{
			return EmplaceAdd(override, (Key&&)k, v);
		}
		std::pair<bool, int> Add(Key&& k, Value&& v, bool override = false)
		{
			return EmplaceAdd(override, (Key&&)k, (Value&&)v);
		}


		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(size_t capacity = 0)
		{
			assert(count == 0 || count == bucketsLen);          // 确保扩容函数使用情型

																// 得到空间利用率最高的扩容长度并直接修改 bucketsLen( count 为当前数据长 )
			if (capacity == 0)
			{
				capacity = count * 2;                           // 2倍扩容
			}
			if (capacity <= bucketsLen) return;
			bucketsLen = Ajy::GetPrime(capacity, false);

			// 桶扩容并全部初始化( 后面会重新映射一次 )
			free(buckets);
			buckets = (int*)malloc(bucketsLen * sizeof(int));
			memset(buckets, -1, bucketsLen * sizeof(int));

			// 节点数组扩容( 保留老数据 )
			nodes = (Node*)realloc(nodes, bucketsLen * sizeof(Node));

			// item 数组扩容
			if ((std::is_base_of<MemoryMoveable, Key>::value || std::is_pod<Key>::value)
				&& (std::is_base_of<MemoryMoveable, Value>::value || std::is_pod<Value>::value))
			{
				items = (Data*)realloc(items, bucketsLen * sizeof(Data));
			}
			else
			{
				Data* newItems = (Data*)malloc(bucketsLen * sizeof(Data));
				for (int i = 0; i < count; ++i)
				{
					new (&newItems[i].key) Key((Key&&)items[i].key);
					items[i].key.Key::~Key();
					new (&newItems[i].value) Value((Value&&)items[i].value);
					items[i].value.Value::~Value();
				}
				free(items);
				items = newItems;
			}

			// 遍历所有节点, 重构桶及链表( 扩容情况下没有节点空洞 )
			for (int i = 0; i < count; i++)
			{
				size_t index = nodes[i].hashCode % bucketsLen;
				if (buckets[index] >= 0)
				{
					items[buckets[index]].prev = i;
				}
				items[i].prev = -1;
				nodes[i].next = buckets[index];
				buckets[index] = i;
			}
		}


		int Find(Key const& key) const
		{
			size_t hashCode = Ajy::GetHashCode(key);
			for (int i = buckets[hashCode % bucketsLen]; i >= 0; i = nodes[i].next)
			{
				if (nodes[i].hashCode == hashCode && Ajy::Equals(items[i].key, key))
				{
					return i;
				}
			}
			return -1;
		}

		void RemoveIndexAt(int idx)
		{
			assert(items[idx].prev != -2);
			if (items[idx].prev < 0)
			{
				buckets[nodes[idx].hashCode % bucketsLen] = nodes[idx].next;
			}
			else
			{
				nodes[items[idx].prev].next = nodes[idx].next;
			}
			if (nodes[idx].next >= 0)       // 如果存在当前节点的下一个节点, 令其 prev 指向 上一个节点
			{
				items[nodes[idx].next].prev = items[idx].prev;
			}

			nodes[idx].next = freeList;     // 当前节点已被移出链表, 令其 next 指向  自由节点链表头( next 有两种功用 )
			freeList = idx;
			++freeCount;

			items[idx].key.Key::~Key();       // 析构内存结构
			items[idx].value.Value::~Value();
			items[idx].prev = -2;           // foreach 时的无效标志
		}


		void Remove(Key const& key)
		{
			int idx = Find(key);
			if (idx != -1)
			{
				RemoveIndexAt(idx);
			}
		}


		void Clear()
		{
			if (!count) return;
			Dispose();
			memset(buckets, -1, bucketsLen * sizeof(int));
			freeList = -1;
			freeCount = 0;
			count = 0;
		}

		size_t Size() const
		{
			return size_t(count - freeCount);
		}

		bool Empty()
		{
			return count == 0;
		}

		Value* TryGetValue(Key const& key)
		{
			int idx = Find(key);
			if (idx >= 0) return items[idx].value;
			return nullptr;
		}

		template<class K>
		Value& operator[](K&& key)
		{
			int idx = Find(key);
			if (idx < 0)
			{
				idx = EmplaceAdd(true, std::forward<K>(key), Value()).second;
			}
			return items[idx].value;
		}
		template<class K>
		Value& At(K&& key)
		{
			return operator[](std::forward<K>(key));
		}

		Key& IndexAtKey(int idx)
		{
			assert(idx < count);
			return items[idx].key;
		}
		Value& IndexAtValue(int idx)
		{
			assert(idx < count);
			return items[idx].value;
		}
		Data& IndexAt(int idx)
		{
			assert(idx < count);
			return items[idx];
		}

		Key const& IndexAtKey(int idx) const
		{
			return const_cast<CMHashMap*>(this)->IndexAtKey(idx);
		}
		Value const& IndexAtValue(int idx) const
		{
			return const_cast<CMHashMap*>(this)->IndexAtValue(idx);
		}
		Data const& IndexAt(int idx) const
		{
			return const_cast<CMHashMap*>(this)->IndexAt(idx);
		}

	protected:
		void Alloc()                                      
		{
			items = (Data*)malloc(bucketsLen * sizeof(Data));
			nodes = (Node*)malloc(bucketsLen * sizeof(Node));
			buckets = (int*)malloc(bucketsLen * sizeof(int));
			memset(buckets, -1, bucketsLen * sizeof(int));          // -1 代表 "空"
		}

		void CopyFrom(CMHashMap const& o)                       
		{
			int i = 0;
			for (int oi = 0; oi < o.count; ++oi)
			{
				if (o.items[oi].prev != -2)
				{
					size_t targetBucket = o.nodes[oi].hashCode % bucketsLen;
					nodes[i].hashCode = o.nodes[oi].hashCode;
					nodes[i].next = buckets[targetBucket];
					if (buckets[targetBucket] >= 0)
					{
						items[buckets[targetBucket]].prev = i;
					}
					buckets[targetBucket] = i;
					new (&items[i].key) Key(o.items[oi].key);
					new (&items[i].value) Value(o.items[oi].value);
					items[i].prev = -1;
					++i;
				}
			}
			count = i;
		}

		void Dispose()                                  
		{
			for (int i = 0; i < count; ++i)
			{
				if (items[i].prev != -2)
				{
					items[i].key.Key::~Key();
					items[i].value.Value::~Value();
				}
			}
		}

		void Destory()                                    
		{
			Dispose();
			free(buckets);
			free(nodes);
			free(items);
			buckets = nullptr;
			nodes = nullptr;
			items = nullptr;
		}
	};

}
