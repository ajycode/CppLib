#pragma once

namespace Ajy
{

	template <class Key>
	HashSet<Key>::HashSet(size_t capacity)
	{
		Init(capacity);
	}
	template <class Key>
	HashSet<Key>::HashSet()
		: HashSet(32)
	{
	}

	template <class Key>
	HashSet<Key>::HashSet(HashSet const& copy)
		: HashSet(copy.Size())
	{
		operator=(copy);
	}

	template <class Key>
	HashSet<Key>::HashSet(HashSet&& copy)
	{
		buckets = std::move(copy.buckets);
		nodes = std::move(copy.nodes);
		pool = std::move(copy.pool);
		copy.Init(32);
	}

	template <class Key>
	HashSet<Key>& HashSet<Key>::operator=(HashSet const& copy)
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
				buckets[mod] = n;
				nodes.Push(n);
			}
		}
		return *this;
	}

	template <class Key>
	HashSet<Key>& HashSet<Key>::operator=(HashSet&& copy)
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

	template <class Key>
	HashSet<Key>::~HashSet()
	{
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			Dispose(nodes[i]);
		}
		nodes.Clear();
		buckets.Clear();
	}

	template<class Key>
	void HashSet<Key>::Init(size_t capacity)
	{
		if (capacity <= 0) capacity = 8;
		else capacity = Round2n(capacity);
		pool.Init<Node>(capacity);
		size_t prime = GetPrime(capacity);
		nodes.Reserve(prime);
		buckets.Resize(prime);
	}

	template <class Key>
	template<class... KT>
	std::pair<typename HashSet<Key>::Node*, bool> HashSet<Key>::EmplaceAdd(KT&&... key)
	{
		std::pair<typename HashSet<Key>::Node*, bool> rtv;
		Key k = Key(std::forward<KT>(key)...);
		size_t hashCode = GetHashCode(k);
		size_t mod = hashCode % buckets.DataLen();
		Node* node = buckets[mod];
		while (node)
		{
			if (node->hash == hashCode && node->key == k)
			{
				rtv.first = node;
				rtv.second = false;
				return rtv;
			}
			node = node->next;
		};
		Node* n = (Node*)pool.Alloc();
		new (&n->key) Key(k);
		n->hash = hashCode;
		n->next = buckets[mod];
		n->index = nodes.DataLen();
		buckets[mod] = n;
		nodes.Push(n);
		if (nodes.DataLen() == buckets.DataLen()) Resize();
		rtv.first = n;
		rtv.second = true;
		return rtv;
	}

	template <class Key>
	std::pair<typename HashSet<Key>::Node*, bool> HashSet<Key>::Add(Key&& key)
	{
		return EmplaceAdd((Key&&)key);
	}
	template <class Key>
	std::pair<typename HashSet<Key>::Node*, bool> HashSet<Key>::Add(Key const& key)
	{
		return EmplaceAdd(key);
	}

	template <class Key>
	typename HashSet<Key>::Node* HashSet<Key>::Find(Key const& key)
	{
		size_t hashCode = GetHashCode(key);
		size_t mod = hashCode % buckets.DataLen();
		Node* node = buckets[mod];
		while (node)
		{
			if (node->hash == hashCode && node->key == key) return node;
			node = node->next;
		}
		return nullptr;
	}

	template <class Key>
	void HashSet<Key>::Remove(Key const& key)
	{
		Node* del = Find(key);
		if (!del) return;
		Remove(del);
	}

	template <class Key>
	void HashSet<Key>::Remove(Node* del)
	{
		size_t mod = del->hash % (uint)buckets.DataLen();
		Node* node = buckets[mod];
		if (node == del) buckets[mod] = node->next;
		else
		{
			Node* parent = node;
			while ((node = node->next))
			{
				if (node == del)
				{
					parent->next = node->next;
					break;
				}
				parent = node;
			}
		}
		Node* last = nodes.Top();
		nodes.Pop();
		if (del != last)
		{
			nodes[del->index] = last;
			last->index = del->index;
		}
		Dispose(del);
		pool.Free(del);
	}

	template <class Key>
	void HashSet<Key>::Clear()
	{
		for (size_t i = 0; i < nodes.DataLen(); ++i)
		{
			Dispose(nodes[i]);
			pool.Free(nodes[i]);
		}
		nodes.Clear();
		memset(buckets.Buf(), 0, buckets.DataByteLen());
	}


	template <class Key>
	void HashSet<Key>::Reserve(size_t capacity)
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

	template <class Key>
	size_t HashSet<Key>::Size() const
	{
		return nodes.DataLen();
	}

	template<class Key>
	Vector<typename HashSet<Key>::Node*> const& HashSet<Key>::Data() const
	{
		return nodes;
	}

	template <class Key>
	void HashSet<Key>::Dispose(Node* n)
	{
		n->key.~Key();
	}

	template <class Key>
	bool HashSet<Key>::Empty() const
	{
		return nodes.DataLen() == 0;
	}

	template <class Key>
	void HashSet<Key>::Resize()
	{
		size_t prime = GetPrime(nodes.DataLen() * 3);
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

	template <class Key>
	typename HashSet<Key>::Node const* HashSet<Key>::operator[](size_t idx) const
	{
		assert(idx < nodes.DataLen());
		return nodes[idx];
	}
	template <class Key>
	typename HashSet<Key>::Node* HashSet<Key>::operator[](size_t idx)
	{
		assert(idx < nodes.DataLen());
		return nodes[idx];
	}
	template <class Key>
	typename HashSet<Key>::Node const* HashSet<Key>::IndexAt(size_t idx) const
	{
		assert(idx < nodes.DataLen());
		return nodes[idx];
	}
	template <class Key>
	typename HashSet<Key>::Node* HashSet<Key>::IndexAt(size_t idx)
	{
		assert(idx < nodes.DataLen());
		return nodes[idx];
	}


}