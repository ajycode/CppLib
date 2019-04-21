#pragma once

namespace Ajy
{

	class ByteBuffer;

	template <class Key, class Value>
	class HashMap : MemoryMoveable
	{
	public:
		struct Node
		{
			Node*           next;
			size_t            hash;                               // key's hashCode
			size_t          index;                              // nodes[ index ]
			Key              key;
			Value              value;
		};
	public:
		HashMap();
		explicit HashMap(size_t capacity);
		HashMap(HashMap&& copy);
		HashMap(HashMap const& copy);
		HashMap& operator=(HashMap&& copy);
		HashMap& operator=(HashMap const& copy);
		~HashMap();
		template<class TK, class... Args>
		std::pair<Node*, bool> EmplaceAdd(bool replace, TK&& k, Args&&... args);		// same as Add but need value type's construct parameters
		std::pair<Node*, bool> Add(Key const& k, Value const& v, bool replace = false);	// if exists, return Node* + false. else add and return Node* + true
		std::pair<Node*, bool> Add(Key const& k, Value&& v, bool replace = false);
		std::pair<Node*, bool> Add(Key&& k, Value const& v, bool replace = false);
		std::pair<Node*, bool> Add(Key&& k, Value&& v, bool replace = false);
		Node const* Find(Key const& k) const;                  // if exists, return Node*. else return nullptr
		Node* Find(Key const& k);
		Value& At(Key const& k);                                  // Find or Add default value( if not found ) and return
		Value& operator[](Key const& k);                          // return at( k )
		void Remove(Key const& k);
		void Remove(Node* n);
		void Reserve(size_t capacity);
		void Clear();
		bool Empty() const;
		size_t Size() const;
		Node const* IndexAt(size_t idx) const;                // return nodes[ idx ]
		Node* IndexAt(size_t idx);
		Vector<Node*> const& Data() const;

		// for Serialization
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
	private:
		void Init(size_t capacity);
		void Dispose(Node* n);
		Vector<Node*>         nodes;
		Vector<Node*>         buckets;
		MemoryPool            pool;
	};

}
