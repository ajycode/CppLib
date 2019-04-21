#pragma once

namespace Ajy
{
	// �ڴ��Ż�ԭ��, �׺� cpu cache
	// hash, next, key �� value ����( value ͨ���ϴ��Ҳ����� find ), �����д洢, �ڴ���Լ���

	// �� node �м��� prev ���, �ɴ������ Remove By Index ����  ������ Add ����
#define ENABLE_FAST_REMOVE_AT_INDEX 0

	template <class TK, class TV>
	struct DictNode
	{
		int  hashCode;
		int  next;
		using TKA = typename std::aligned_storage<sizeof(TK), std::alignment_of<TK>::value>::type;
		TKA  key;
#if ENABLE_FAST_REMOVE_AT_INDEX
		int  prev;
#endif
	};

	template <class TK, class TV>
	class Dictionary : MemoryMoveable
	{
	public:
		using NT = DictNode<TK, TV>;
		using TKA = typename NT::TKA;
		using TVA = typename std::aligned_storage<sizeof(TV), std::alignment_of<TV>::value>::type;
		// ��¶���ⷽ�����( ���� Foreach ������ʵ���Լ�С�� )
		int         freeList;               // ���ɿռ�����ͷ( next ָ����һ��δʹ�õ�Ԫ )
		size_t      freeCount;              // ���ɿռ�����
		size_t      count;                  // ��ʹ�ÿռ���
		size_t      bucketsLen;             // Ͱӳ��������( ���� )
		int        *buckets;                // Ͱӳ����
		NT         *entries;                // ��ڵ�
		TVA        *values;                 // ��ֵ( �� nodes һһ��Ӧ )
	public:
		explicit Dictionary(size_t capacity);
		Dictionary();
		Dictionary(Dictionary const& o);
		Dictionary(Dictionary&& o);
		~Dictionary();
		Dictionary& operator=(Dictionary const& o);
		Dictionary& operator=(Dictionary&& o);
		template<class K, class... Args>
		std::pair<bool, int> EmplaceAdd(bool override, K&& key, Args&&... args);
		void Reserve(size_t capacity = 0);
		int Find(TK const& key) const;
		int Remove(TK const& key);
		size_t Size() const;
		std::pair<bool, int> Add(TK const& k, TV const& v, bool replace = false);
		std::pair<bool, int> Add(TK const& k, TV&& v, bool replace = false);
		std::pair<bool, int> Add(TK&& k, TV const& v, bool replace = false);
		std::pair<bool, int> Add(TK&& k, TV&& v, bool replace = false);
		void Clear();
		TV* TryGetValue(TK const& key);
		template<class K>
		TV& operator[](K&& key);
		template<class K>
		TV& At(K&& key);
		TK& KeyIndexAt(int idx);
		TV& IndexAt(int idx);
		TV const& IndexAt(int idx) const;
		void RemoveIndexAt(int idx);
		bool Empty() const;
		void Foreach(std::function<bool(TK const& key, TV& value)> handler);
		void Foreach(std::function<bool(TK const& key, TV& value)> handler) const;
	protected:
		static TK& RefK(TKA& ka);
		TK& RefKat(int idx);
		TK const& RefKat(int idx) const;
		static TV& RefV(TVA& va);
		TV& RefVat(int idx);
		TV const& RefVat(int idx) const;
		template<class T>
		static int MyGetHashCode(T const& v);
		// ���� ���ƹ��� �� operator=
		void FastAdd(Dictionary const& o);
		// ���� ����, ����, Clear
		void DeleteKVs();
		// ��������, �ƶ�����
		void DeleteAll();
	};

}
