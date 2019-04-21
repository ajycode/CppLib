#pragma once

namespace Ajy
{

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(size_t capacity)
	{
		Init(capacity);
	}

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap()
		: HashMap(32)
	{
	}

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(HashMap const& copy)
		: HashMap(copy.Size())
	{
		operator=(copy);
	}

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(HashMap&& copy)
	{
		buckets = std::move(copy.buckets);
		nodes = std::move(copy.nodes);
		pool = std::move(copy.pool);
		copy.Init(32);
	}

	template <class Key, class Value>
	HashMap<Key, Value>& HashMap<Key, Value>::operator=(HashMap const& copy)
	{
		if (this != &copy)
		{
			Clear();
			Reserve(copy.Size());
			for (size_t i = 0; i < copy.Size(); ++i)
			{
				Node* on = copy.nodes[i];
				size_t mod = on->hash % buckets.DataLen();
				Node* n = (Node*)pool.FastAlloc();
				n->next = buckets[mod];
				n->hash = on->hash;
				n->index = nodes.DataLen();
				new (&n->key) Key(on->key);
				new (&n->value) Value(on->value);
				buckets[mod] = n;
				nodes.Push(n);
			}
		}
		return *this;
	}

	template <class Key, class Value>
	HashMap<Key, Value>& HashMap<Key, Value>::operator=(HashMap&& copy)
	{
		if (this != &copy)
		{
			for (size_t i = 0; i < nodes.DataLen(); ++i)
			{
				Dispose(nodes[i]);
			}
			buckets = std::move(copy.buckets);
			nodes = std::move(copy.nodes);
			pool = std::move(copy.pool);
			copy.Init(32);
		}
		return *this;
	}

	template <class Key, class Value>
	HashMap<Key, Value>::~HashMap()
	{
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			Dispose(nodes[i]);
		}
		nodes.Clear();
		buckets.Clear();
	}

	template<class Key, class Value>
	void HashMap<Key, Value>::Init(size_t capacity)
	{
		if (capacity <= 0) capacity = 8;
		else capacity = Round2n(capacity);
		pool.Init<Node>(capacity);
		size_t prime = Ajy::GetPrime(capacity);
		nodes.Reserve(prime);
		buckets.Resize(prime);
	}

	template <class Key, class Value>
	std::pair<typename HashMap<Key, Value>::Node*, bool> HashMap<Key, Value>::Add(Key const& k, Value&& v, bool replace /*= false */)
	{
		return EmplaceAdd(replace, k, (Value&&)v);
	}
	template <class Key, class Value>
	std::pair<typename HashMap<Key, Value>::Node*, bool> HashMap<Key, Value>::Add(Key const& k, Value const& v, bool replace /*= false */)
	{
		return EmplaceAdd(replace, k, v);
	}
	template <class Key, class Value>
	std::pair<typename HashMap<Key, Value>::Node*, bool> HashMap<Key, Value>::Add(Key&& k, Value&& v, bool replace /*= false */)
	{
		return EmplaceAdd(replace, (Key&&)k, (Value&&)v);
	}
	template <class Key, class Value>
	std::pair<typename HashMap<Key, Value>::Node*, bool> HashMap<Key, Value>::Add(Key&& k, Value const& v, bool replace /*= false */)
	{
		return EmplaceAdd(replace, (Key&&)k, v);
	}

	template <class Key, class Value>
	template<class TK, class... Args>
	std::pair<typename HashMap<Key, Value>::Node*, bool> HashMap<Key, Value>::EmplaceAdd(bool replace, TK&& k, Args&&... args)
	{
		std::pair<typename HashMap<Key, Value>::Node*, bool> rtv;
		size_t hashCode = GetHashCode(k);
		size_t mod = hashCode % buckets.DataLen();
		Node* node = buckets[mod];
		while (node)
		{
			if (node->hash == hashCode && node->key == k)
			{
				if (replace) node->value = Value(std::forward<Args>(args)...);
				rtv.first = node;
				rtv.second = false;
				return rtv;
			}
			node = node->next;
		};
		Node* n = (Node*)pool.FastAlloc();
		n->next = buckets[mod];
		n->hash = hashCode;
		n->index = nodes.DataLen();
		new (&n->key) Key(std::forward<TK>(k));
		new (&n->value) Value(std::forward<Args>(args)...);
		buckets[mod] = n;
		nodes.Push(n);
		if (nodes.DataLen() == buckets.DataLen())
		{
			Reserve(nodes.DataLen() * 2);
		}
		rtv.first = n;
		rtv.second = true;
		return rtv;
	}

	template <class Key, class Value>
	typename HashMap<Key, Value>::Node* HashMap<Key, Value>::Find(Key const& k)
	{
		size_t hashCode = GetHashCode(k);
		size_t mod = hashCode % buckets.DataLen();
		Node* node = buckets[mod];
		while (node)
		{
			if (node->hash == hashCode && node->key == k) return node;
			node = node->next;
		}
		return nullptr;
	}

	template <class Key, class Value>
	typename HashMap<Key, Value>::Node const* HashMap<Key, Value>::Find(Key const& k) const
	{
		return const_cast<HashMap<Key, Value>*>(this)->Find(k);
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::Remove(Key const& k)
	{
		Node* n = Find(k);
		if (!n) return;
		Remove(n);
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::Remove(Node* n)
	{
		size_t mod = n->hash % buckets.DataLen();
		Node* node = buckets[mod];
		if (node == n) buckets[mod] = node->next;
		else
		{
			Node* parent = node;
			while ((node = node->next))
			{
				if (node == n)
				{
					parent->next = node->next;
					break;
				}
				parent = node;
			}
		}
		Node* last = nodes.Top();
		nodes.Pop();
		if (n != last)
		{
			nodes[n->index] = last;
			last->index = n->index;
		}
		Dispose(n);
	}

	template <class Key, class Value>
	Value& HashMap<Key, Value>::operator[](Key const& k)
	{
		size_t hashCode = GetHashCode(k);
		size_t mod = hashCode % buckets.DataLen();
		Node* node = buckets[mod];
		while (node)
		{
			if (node->hash == hashCode && node->key == k) return node->value;
			node = node->next;
		};
		Node* n = (Node*)pool.FastAlloc();                       // new & Init
		n->next = buckets[mod];
		n->hash = hashCode;
		n->index = nodes.DataLen();
		new (&n->key) Key(k);
		new (&n->value) Value();
		buckets[mod] = n;
		nodes.Push(n);
		if (nodes.DataLen() == buckets.DataLen())
			Reserve(nodes.DataLen() * 2);
		return n->value;
	}

	template <class Key, class Value>
	Value& HashMap<Key, Value>::At(Key const& k)
	{
		return operator[](k);
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::Clear()
	{
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			Dispose(nodes[i]);
		}
		nodes.Clear();
		memset(buckets.Buf(), 0, buckets.DataByteLen());
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::Reserve(size_t capacity)
	{
		if (capacity <= buckets.DataLen()) return;
		size_t prime = GetPrime(Round2n(capacity));
		if (pool.allPageItemCounter < prime)
		{
			pool.SetPageSizeByItemCount(prime - pool.allPageItemCounter);
			pool.NewPage();
		}
		nodes.Reserve(prime);
		buckets.Resize(prime, false);
		memset(buckets.Buf(), 0, buckets.DataByteLen());      // clean up
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			Node*& o = nodes[i];
			size_t mod = o->hash % prime;
			o->next = buckets[mod];
			buckets[mod] = o;
		}
	}

	template <class Key, class Value>
	typename HashMap<Key, Value>::Node const* HashMap<Key, Value>::IndexAt(size_t idx) const
	{
		
		return nodes[idx];
	}

	template <class Key, class Value>
	typename HashMap<Key, Value>::Node* HashMap<Key, Value>::IndexAt(size_t idx)
	{
		return nodes[idx];
	}

	template <class Key, class Value>
	size_t HashMap<Key, Value>::Size() const
	{
		return nodes.DataLen();
	}

	template<class Key, class Value>
	Vector<typename HashMap<Key, Value>::Node*> const& HashMap<Key, Value>::Data() const
	{
		return nodes;
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::Dispose(Node* n)
	{
		n->key.~Key();
		n->value.~Value();
		n->next = nullptr;
		pool.Free(n);
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Empty() const
	{
		return nodes.DataLen() == 0;
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(nodes.DataLen());
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			bb.Write(nodes[i]->key);
			bb.Write(nodes[i]->value);
		}
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		Clear();
		Reserve(len);
		for (size_t i = 0; i < len; ++i)
		{
			Node* n = (Node*)pool.FastAlloc();							// malloc
			if (!std::is_pod<Key>::value) new (&n->key) Key();         // new key
			if (!bb.Read(n->key))
			{
				if (!std::is_pod<Key>::value) n->key.~Key();             // delete key
				pool.Free(n);                                         // free
				return false;
			}
			if (!std::is_pod<Value>::value) new (&n->value) Value();       // new value
			if (!bb.Read(n->value))
			{
				if (!std::is_pod<Value>::value) n->value.~Value();           // delete value
				pool.Free(n);                                         // free
				return false;
			}
			n->hash = GetHashCode(n->key);
			size_t mod = n->hash % buckets.DataLen();
			n->index = i;
			n->next = buckets[mod];
			buckets[mod] = n;
			nodes.Push(n);
		}
		return true;
	}

	template <class Key, class Value>
	void HashMap<Key, Value>::VarWriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(nodes.DataLen());
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			bb.VarWrite(nodes[i]->key);
			bb.VarWrite(nodes[i]->value);
		}
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::VarReadFrom(ByteBuffer& bb)
	{
		return VarReadFrom(bb, 0, 0);
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		Clear();
		Reserve(len);
		for (size_t i = 0; i < len; ++i)
		{
			Node* n = (Node*)pool.FastAlloc();							// malloc
			if (!std::is_pod<Key>::value) new (&n->key) Key();         // new key
			if (!bb.VarRead(n->key))
			{
				if (!std::is_pod<Key>::value) n->key.~Key();             // delete key
				pool.Free(n);                                         // free
				return false;
			}
			if (!std::is_pod<Value>::value) new (&n->value) Value();       // new value
			if (!bb.VarRead(n->value))
			{
				if (!std::is_pod<Value>::value) n->value.~Value();           // delete value
				pool.Free(n);                                         // free
				return false;
			}
			n->hash = GetHashCode(n->key);
			size_t mod = n->hash % buckets.DataLen();
			n->index = i;
			n->next = buckets[mod];
			buckets[mod] = n;
			nodes.Push(n);
		}
		return true;
	}

}
