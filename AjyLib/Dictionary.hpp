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
		memset(buckets, -1, bucketsLen * sizeof(int));          // -1 ���� "��"
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
		if (!buckets)                      // �ж�һ���͹���. ��ֵ������»�Ϊ nullptr
		{
			return;
		}
		DeleteAll();
	}

	template <class TK, class TV>
	Dictionary<TK, TV>& Dictionary<TK, TV>::operator =(Dictionary const& o)
	{
		DeleteKVs();
		// ȷ�� this.bucketsLen ���ڵ��� o.count( ���������� )
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
		////count = 0;      // FastAdd �������
		// ��������
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
		// hash ��Ͱ��ȡģ ��λ������ ����, ɨ��
		int hashCode = MyGetHashCode(std::forward<K>(key));
		int targetBucket = int((unsigned int)hashCode % bucketsLen);
		for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
		{
			if (entries[i].hashCode == hashCode && Ajy::Equals(RefKat(i), key))
			{
				if (replace)                                      // ������ value
				{
					RefVat(i) = TV(std::forward<Args>(args)...);
					return std::make_pair(true, i);
				}
				return std::make_pair(false, i);
			}
		}
		// û�ҵ�������
		int index;
		if (freeCount > 0)                  // ��� ���ɽڵ����� ����, ȡһ����������
		{                                   // ��Щ�ڵ����� Remove ����. next ָ����һ��
			index = freeList;
			freeList = entries[index].next;
			freeCount--;
		}
		else
		{
			if (count == bucketsLen)               // ���пսڵ㶼�ù���, Resize
			{
				Reserve();
				targetBucket = int((unsigned int)hashCode % bucketsLen);
			}
			index = count;                         // ָ�� Resize ����Ŀռ����
			count++;
		}

#if ENABLE_FAST_REMOVE_AT_INDEX
		// �����ǰ bucket ���� index, ������ prev ���ڼ�����ӵ� index
		if (buckets[targetBucket] >= 0)
		{
			entries[buckets[targetBucket]].prev = index;
		}
		entries[index].prev = -1;
#endif

		// ִ�е��˴�ʱ, freeList Ҫô���� ���ɽڵ�����, ҪôΪ Resize ����Ŀռ����.
		entries[index].hashCode = hashCode;
		entries[index].next = buckets[targetBucket];
		buckets[targetBucket] = index;

		// �ƶ����ƹ���д key, value
		new (&entries[index].key) TK(std::forward<K>(key));
		new (&values[index]) TV(std::forward<Args>(args)...);
		return std::make_pair(true, index);
	}

	template <class TK, class TV>
	void Dictionary<TK, TV>::Reserve(size_t capacity/* = 0*/)
	{
		if (capacity == 0)
		{
			// 2�����ݷ�, ��һ���պ�С�� 2^n ����������Ͱ��
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
		// Ͱ���ݲ���ʼ��
		delete[] buckets;
		buckets = new int[capacity];
		memset(buckets, -1, capacity * sizeof(int));
		bucketsLen = capacity;

		// �ڵ���������
		NT* oldEntries = entries;
		entries = new NT[capacity];
		if (std::is_base_of<MemoryMoveable, TK>::value || std::is_pod<TK>::value)
		{
			memcpy(entries, oldEntries, sizeof(NT) * count);
		}
		else                        // ���� pod ����Ϊ ��ֱ�Ӹ��Ƶ���������, �� memcpy
		{                           // ���������� �ƶ����ƹ��캯�� + ����������( ��ͬ )
			for (int i = 0; i < count; ++i)
			{
				entries[i].hashCode = oldEntries[i].hashCode;
				// next ɶ�� ���ظ���
				TK& oldk = RefK(oldEntries[i].key);
				new (&entries[i].key) TK((TK&&)oldk);
				oldk.TK::~TK();
			}
		}
		delete[] oldEntries;

		// ֵ��������
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

		// �������нڵ�, �ع�Ͱ������
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
		int last = -1;                                          // �� "��һ���ڵ�". -1 ��ʾ�ڵ�λ��Ͱ

		for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
		{
			if (entries[i].hashCode == hashCode && Ajy::Equals(RefKat(i), key))
			{
				if (last < 0)
				{
					// û����һ���ڵ�
					buckets[targetBucket] = entries[i].next;
				}
				else
				{
					// ��һ���ڵ�. next ԭ��ָ�� ��ǰ�ڵ�, ����ָ�� ��ǰ�ڵ�ָ�����һ���ڵ�
					entries[last].next = entries[i].next;
#if ENABLE_FAST_REMOVE_AT_INDEX
					// ������ڵ�ǰ�ڵ����һ���ڵ�, ���� prev ָ�� ��һ���ڵ�
					if (entries[i].next >= 0)
					{
						entries[entries[i].next].prev = last;
					}
#endif
				}

				// ��ǰ�ڵ��ѱ��Ƴ�����, ���� next ָ��  ���ɽڵ�����ͷ( next �����ֹ��� )
				entries[i].next = freeList;
				entries[i].hashCode = -1;
				freeList = i;
				freeCount++;

				// �����ڴ�ṹ
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
		int last = -1;                                          // �� "��һ���ڵ�". -1 ��ʾ�ڵ�λ��Ͱ

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
