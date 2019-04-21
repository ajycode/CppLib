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
		
		int					freeList;               // ���ɿռ�����ͷ( next ָ����һ��δʹ�õ�Ԫ )
		size_t              freeCount;              // ���ɿռ�����
		size_t              count;                  // ��ʹ�ÿռ���
		size_t              bucketsLen;             // Ͱ���鳤( ���� )
		int                *buckets;                // Ͱ����
		Node               *nodes;                  // �ڵ�����
		Data               *items;                  // ��������( ��ڵ�����ͬ���±� )

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
			if (bucketsLen < o.count)           // ȷ�� this.bucketsLen ���ڵ��� o.count( ���������� )
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

			CopyFrom(o);     // �������� ������ count ��ֵ
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
			if (!buckets) return;               // �ж�һ���͹���. ��ֵ������»�Ϊ nullptr
			Destory();
		}

		template<class K, class... Args>
		std::pair<bool, int> EmplaceAdd(bool override, K&& key, Args&&... args)
		{
			// hash ��Ͱ��ȡģ ��λ������ ����, ɨ��
			size_t hashCode = Ajy::GetHashCode(key);
			size_t targetBucket = hashCode % bucketsLen;
			for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
			{
				if (nodes[i].hashCode == hashCode && Ajy::Equals(items[i].key, key))
				{
					if (override)                       // ������ value
					{
						items[i].value = Value(std::forward<Args>(args)...);
					}
					return std::make_pair(false, i);
				}
			}
			// û�ҵ�������
			int index;
			if (freeCount > 0)                          // ��� ���ɽڵ����� ����, ȡһ����������
			{                                           // ��Щ�ڵ����� Remove ����. next ָ����һ��
				index = freeList;
				freeList = nodes[index].next;
				--freeCount;
			}
			else
			{
				if (count == bucketsLen)               // ���пսڵ㶼�ù���, Resize
				{
					Reserve();
					targetBucket = hashCode % bucketsLen;
				}
				index = count;                         // ָ�� Resize ����Ŀռ����
				++count;
			}

			// ִ�е��˴�ʱ, freeList Ҫô���� ���ɽڵ�����, ҪôΪ Reserve �������Ŀռ����.
			nodes[index].hashCode = hashCode;
			nodes[index].next = buckets[targetBucket];

			// �����ǰ bucket ���� index, ������ prev ���ڼ�����ӵ� index
			if (buckets[targetBucket] >= 0)
			{
				items[buckets[targetBucket]].prev = index;
			}
			buckets[targetBucket] = index;

			// �ƶ����ƹ���д key, value
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


		// ֻ֧��û����ʱ���ݻ�ռ��þ�����( �������������, ����ʱ�� ������� �Դ� )
		void Reserve(size_t capacity = 0)
		{
			assert(count == 0 || count == bucketsLen);          // ȷ�����ݺ���ʹ������

																// �õ��ռ���������ߵ����ݳ��Ȳ�ֱ���޸� bucketsLen( count Ϊ��ǰ���ݳ� )
			if (capacity == 0)
			{
				capacity = count * 2;                           // 2������
			}
			if (capacity <= bucketsLen) return;
			bucketsLen = Ajy::GetPrime(capacity, false);

			// Ͱ���ݲ�ȫ����ʼ��( ���������ӳ��һ�� )
			free(buckets);
			buckets = (int*)malloc(bucketsLen * sizeof(int));
			memset(buckets, -1, bucketsLen * sizeof(int));

			// �ڵ���������( ���������� )
			nodes = (Node*)realloc(nodes, bucketsLen * sizeof(Node));

			// item ��������
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

			// �������нڵ�, �ع�Ͱ������( ���������û�нڵ�ն� )
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
			if (nodes[idx].next >= 0)       // ������ڵ�ǰ�ڵ����һ���ڵ�, ���� prev ָ�� ��һ���ڵ�
			{
				items[nodes[idx].next].prev = items[idx].prev;
			}

			nodes[idx].next = freeList;     // ��ǰ�ڵ��ѱ��Ƴ�����, ���� next ָ��  ���ɽڵ�����ͷ( next �����ֹ��� )
			freeList = idx;
			++freeCount;

			items[idx].key.Key::~Key();       // �����ڴ�ṹ
			items[idx].value.Value::~Value();
			items[idx].prev = -2;           // foreach ʱ����Ч��־
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
			memset(buckets, -1, bucketsLen * sizeof(int));          // -1 ���� "��"
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
