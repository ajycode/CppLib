#pragma once

namespace Ajy
{

	template<class T>
	struct ListNode
	{
		ListNode<T>* prev;
		ListNode<T>* next;
		T value;
	};

	template<class T, class Ref, class Ptr>
	struct ListIterator
	{
		ListNode<T>* node;
		ListIterator() : node(nullptr) {}
		explicit ListIterator(ListNode<T>* p) : node(p) {}
		ListIterator(ListIterator<T, T&, T*> const& it) : node(it.node) {}
		bool operator==(ListIterator const& r) const { return node == r.node; }
		bool operator!=(ListIterator const& r) const { return !(node == r.node); }
		ListIterator& operator++() { node = node->next; return *this; }
		ListIterator operator++(int) { ListIterator iter = *this; ++(*this); return iter; }
		ListIterator& operator--() { node = node->prev; return *this; }
		ListIterator operator--(int) { ListIterator iter = *this; --(*this); return iter; }
		Ref operator*() { return node->value; }
		Ptr operator->() { return &node->value; }
	};

	template<class T, class Ref, class Ptr>
	struct ListReverseIterator
	{
		ListNode<T>* node;
		ListReverseIterator() : node(nullptr) {}
		explicit ListReverseIterator(ListNode<T>* p) : node(p) {}
		ListReverseIterator(ListReverseIterator<T, T&, T*> const& it) : node(it.node) {}
		bool operator==(ListReverseIterator const& r) const { return node == r.node; }
		bool operator!=(ListReverseIterator const& r) const { return !(node == r.node); }
		ListReverseIterator& operator++() { node = node->prev; return *this; }
		ListReverseIterator operator++(int) { ListReverseIterator iter = *this; ++(*this); return iter; }
		ListReverseIterator& operator--() { node = node->next; return *this; }
		ListReverseIterator operator--(int) { ListReverseIterator iter = *this; --(*this); return iter; }
		Ref operator*() { return node->value; }
		Ptr operator->() { return &node->value; }
	};

	template<class T>
	class List :MemoryMoveable
	{
	public:
		typedef ListNode<T> Node;
		typedef ListNode<T>* NodePtr;
		typedef ListIterator<T, T&, T*> Iterator;
		typedef ListIterator<T, T const&, T const*> ConstIterator;
		typedef ListReverseIterator<T, T&, T*> ReverseIterator;
		typedef ListReverseIterator<T, T const&, T const*> ConstReverseIterator;
	private:
		Node* head = nullptr;
		MemoryPool pool;
		size_t nodeCount;
	public:
		List();
		explicit List(size_t capacity);
		List(List const& copy);
		List(List&& copy);
		List& operator=(List const& copy);
		List& operator=(List&& copy);
		~List();
		bool Empty() const { return !nodeCount; }
		size_t Size() const { return nodeCount; }
		//若要支持C++11 range-based for循环，Begin和End首字母小写即可
		Iterator Begin() { return Iterator(head->next); }
		ConstIterator Begin() const { return ConstIterator(head->next); }
		Iterator End() { return Iterator(head); }
		ConstIterator End() const { return ConstIterator(head); }
		ReverseIterator RBegin() { return ReverseIterator(head->prev); }
		ConstReverseIterator RBegin() const { return ConstReverseIterator(head->prev); }
		ReverseIterator REnd() { return ReverseIterator(head); }
		ConstReverseIterator REnd() const { return ConstReverseIterator(head); }
		T& Front() { return *Begin(); }
		T const& Front() const { return *Begin(); }
		T& Back() { return *(--End()); }
		T const& Back() const { return *(--End()); }
		void Assign(Iterator first, Iterator last);
		void Clear();
		void PushFront(T const& t) { Insert(Begin(), t); }
		void PushFront(T&& t) { Insert(Begin(), (T&&)t); }
		void PushBack(T const& t) { Insert(End(), t); }
		void PushBack(T&& t) { Insert(End(), (T&&)t); }
		Iterator Insert(Iterator pos, T const& t) { return EmplaceInsert(pos, t); }
		Iterator Insert(Iterator pos, T&& t) { return EmplaceInsert(pos, (T&&)t); }
		void Insert(Iterator pos, Iterator first, Iterator last);
		void Insert(Iterator pos, ConstIterator first, ConstIterator last);
		void PopFront() { Erase(Begin()); }
		void PopBack() { Erase(--End()); }
		Iterator Erase(Iterator pos);
		Iterator Erase(Iterator first, Iterator last);
		void Remove(T const& t);
		void RemoveIf(Ajy::function<bool(T)> func);
		void Resize(size_t size);
		void Reverse();
		void Sort(bool asc = true);

		// for Serialization
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
	private:
		void Init(size_t capacity);
		NodePtr UnLink(Iterator pos);
		void Dispose(NodePtr dispose);
		template<class... Args>
		Iterator EmplaceInsert(Iterator pos, Args&&... args);
	};

}

