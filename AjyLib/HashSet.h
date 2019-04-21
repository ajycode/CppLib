#pragma once

namespace Ajy
{
	// 直接由 HashMap 去 value 简化而来
	template <class Key>
	struct HashSet : MemoryMoveable
	{
		struct Node
		{
			Node*           next;
			size_t            hash;                               // key's hashCode
			size_t          index;                              // nodes[ index ]
			Key              key;
		};
	public:
		HashSet();
		explicit HashSet(size_t capacity);
		HashSet(HashSet&& copy);
		HashSet(HashSet const& copy);
		HashSet& operator=(HashSet&& copy);
		HashSet& operator=(HashSet const& copy);
		~HashSet();
		template<class... KT>
		std::pair<Node*, bool> EmplaceAdd(KT&&... key);		// same as Add but need value type's construct parameters
		std::pair<Node*, bool> Add(Key&& key);					// if exists, return Node* + false. else add and return Node* + true
		std::pair<Node*, bool> Add(Key const& key);
		Node* Find(Key const& key);                              // if exists, return Node*. else return nullptr
		void Remove(Key const& key);
		void Remove(Node* del);
		void Clear();
		bool Empty() const;
		void Reserve(size_t capacity);
		size_t Size() const;
		Node const* operator[](size_t idx) const;
		Node* operator[](size_t idx);
		Node const* IndexAt(size_t idx) const;
		Node* IndexAt(size_t idx);
		Vector<Node*> const& Data() const;
	protected:
		void Init(size_t capacity);
		void Dispose(Node* n);
		void Resize();
		Vector<Node*>         nodes;
		Vector<Node*>         buckets;
		MemoryPool            pool;
	};

}