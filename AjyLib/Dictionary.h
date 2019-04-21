#pragma once

namespace Ajy
{
	// 内存优化原则, 亲和 cpu cache
	// hash, next, key 与 value 分离( value 通常较大且不参与 find ), 数组列存储, 内存相对集中

	// 在 node 中加入 prev 设计, 可大幅提升 Remove By Index 性能  但降低 Add 性能
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
		// 暴露在外方便遍历( 复制 Foreach 函数的实现自己小改 )
		int         freeList;               // 自由空间链表头( next 指向下一个未使用单元 )
		size_t      freeCount;              // 自由空间链长
		size_t      count;                  // 已使用空间数
		size_t      bucketsLen;             // 桶映射区长度( 质数 )
		int        *buckets;                // 桶映射区
		NT         *entries;                // 存节点
		TVA        *values;                 // 存值( 与 nodes 一一对应 )
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
		// 用于 复制构造 和 operator=
		void FastAdd(Dictionary const& o);
		// 用于 析构, 复制, Clear
		void DeleteKVs();
		// 用于析构, 移动复制
		void DeleteAll();
	};

}
