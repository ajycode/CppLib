#pragma once

namespace Ajy
{

	template<class Value>
	struct SkipListNode
	{
		size_t nextCount;
		Value val;
		SkipListNode* next[1];
	};

	template<class Value, class Ref, class Ptr>
	struct SkipListIterator
	{
		SkipListNode<Value>* node;
		SkipListIterator() : node(nullptr) {}
		explicit SkipListIterator(SkipListNode<Value>* p) : node(p) {}
		SkipListIterator(SkipListIterator<Value, Value&, Value*> const& it) : node(it.node) {}
		bool operator==(SkipListIterator const& r) const { return node == r.node; }
		bool operator!=(SkipListIterator const& r) const { return !(node == r.node); }
		SkipListIterator& operator++() { node = node->next[0]; return *this; }
		SkipListIterator operator++(int) { SkipListIterator iter = *this; ++(*this); return iter; }
		Ref operator*() { return node->val; }
		Ptr operator->() { return &node->val; }
	};

	template<class Value, class Comparator = std::less<Value>>
	class SkipList :MemoryMoveable
	{
	public:
		typedef SkipListNode<Value> Node;
		typedef SkipListNode<Value>* NodePtr;
		typedef SkipListIterator<Value, Value&, Value*> Iterator;
		typedef SkipListIterator<Value, Value const&, Value const*> ConstIterator;
	public:
		SkipList();
		explicit SkipList(size_t maxlevel);
		~SkipList();
		SkipList(SkipList const& copy);
		SkipList(SkipList&& copy);
		SkipList& operator=(SkipList const& copy);
		SkipList& operator=(SkipList&& copy);
		size_t GetMaxLevel() const { return maxLevel; }
		size_t GetLevel() const { return level; }
		size_t Size() const { return nodeCount; }
		bool Empty() const { return nodeCount == 0; }
		//若要支持C++11 range-based for循环，Begin和End首字母小写即可
		Iterator Begin() { return Iterator(head->next[0]); }
		ConstIterator Begin() const { return ConstIterator(head->next[0]); }
		Iterator End() { return Iterator(nullptr); }
		ConstIterator End() const { return ConstIterator(nullptr); }
		Iterator Insert(Value const& val);
		Iterator Insert(Value&& val);
		template<class Iter>
		Iterator Insert(Iter const& it) { return Insert(*it); }
		template<class Iter>
		void Insert(Iter const& first, Iter const& last);
		Iterator Erase(Iterator pos) { return Erase(pos.node); }
		Iterator Erase(Iterator first, Iterator last);
		void Erase(Value const& val);
		void Clear();
		Iterator Find(Value const& val);
	private:
		NodePtr CreateNode(Value const& val, size_t _level);
		void Init();
		void CopyFrom(SkipList const& copy);
		size_t RandomLevel();
		bool IsValueAfterNode(Value const& val, NodePtr node);
		Iterator FindGreaterOrEqual(Value const& val, NodePtr* prev);
		Iterator FindLess(Value const& val);
		template<class ...Args>
		Iterator EmplaceInsert(Args&&... args);
		Iterator Erase(NodePtr node);
	private:
		size_t level;
		size_t maxLevel;
		size_t nodeCount;
		Comparator valComparator;
		Random random;
		NodePtr head = nullptr;
	};

}
