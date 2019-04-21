#pragma once

namespace Ajy
{

	template<class Key, class Compare>
	Set<Key, Compare>::Set()
		:Set(32)
	{
	}

	template<class Key, class Compare>
	Set<Key, Compare>::Set(size_t capacity)
		: rbtree(capacity)
	{
	}

	template<class Key, class Compare>
	Set<Key, Compare>::Set(Set const& copy)
		: rbtree(copy.rbtree)
	{
	}

	template<class Key, class Compare>
	Set<Key, Compare>::Set(Set&& copy)
		: rbtree(std::move(copy.rbtree))
	{
	}

	template<class Key, class Compare>
	Set<Key, Compare>& Set<Key, Compare>::operator=(Set const& copy)
	{
		if (this != &copy)
			rbtree = copy.rbtree;
		return *this;
	}

	template<class Key, class Compare>
	Set<Key, Compare>& Set<Key, Compare>::operator=(Set&& copy)
	{
		if (this != &copy)
			rbtree = std::move(copy.rbtree);
		return *this;
	}



	/*-------------------------------------------------------------------------------------------------------------------------*/



	template<class Key, class Compare>
	MultiSet<Key, Compare>::MultiSet()
		: MultiSet(32)
	{
	}

	template<class Key, class Compare>
	MultiSet<Key, Compare>::MultiSet(size_t capacity)
		: rbtree(capacity)
	{
	}

	template<class Key, class Compare>
	MultiSet<Key, Compare>::MultiSet(MultiSet const& copy)
		: rbtree(copy.rbtree)
	{
	}

	template<class Key, class Compare>
	MultiSet<Key, Compare>::MultiSet(MultiSet&& copy)
		: rbtree(std::move(copy.rbtree))
	{
	}

	template<class Key, class Compare>
	MultiSet<Key, Compare>& MultiSet<Key, Compare>::operator=(MultiSet const& copy)
	{
		if (this != &copy)
			rbtree = copy.rbtree;
		return *this;
	}

	template<class Key, class Compare>
	MultiSet<Key, Compare>& MultiSet<Key, Compare>::operator=(MultiSet&& copy)
	{
		if (this != &copy)
			rbtree = std::move(copy.rbtree);
		return *this;
	}

	template<class Key, class Compare>
	void MultiSet<Key, Compare>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
		{
			bb.Write(*it);
		}
	}

	template<class Key, class Compare>
	bool MultiSet<Key, Compare>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	template<class Key, class Compare>
	bool MultiSet<Key, Compare>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
		for (size_t i = 0; i < len; ++i)
		{
			Key k;
			if (!bb.Read(k))
			{
				if (std::is_pod<Key>::value) k.~Key();
				return false;
			}
			Insert(k);
		}
		return true;
	}

	template<class Key, class Compare>
	void MultiSet<Key, Compare>::VarWriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
		{
			bb.VarWrite(*it);
		}
	}

	template<class Key, class Compare>
	bool MultiSet<Key, Compare>::VarReadFrom(ByteBuffer& bb)
	{
		return VarReadFrom(bb, 0, 0);
	}

	template<class Key, class Compare>
	bool MultiSet<Key, Compare>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
		for (size_t i = 0; i < len; ++i)
		{
			Key k;
			if (!bb.VarRead(k))
			{
				if (std::is_pod<Key>::value) k.~Key();
				return false;
			}
			Insert(k);
		}
		return true;
	}

}
