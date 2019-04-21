#pragma once

namespace Ajy
{

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>::Map()
		:Map(32)
	{
	}

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>::Map(size_t capacity)
		: rbtree(capacity)
	{
	}

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>::Map(Map const& copy)
		: rbtree(copy.rbtree)
	{
	}

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>::Map(Map&& copy)
		: rbtree(std::move(copy.rbtree))
	{
	}

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>& Map<Key, Value, Compare>::operator=(Map const& copy)
	{
		if (this != &copy)
			rbtree = copy.rbtree;
		return *this;
	}

	template<class Key, class Value, class Compare>
	Map<Key, Value, Compare>& Map<Key, Value, Compare>::operator=(Map&& copy)
	{
		if (this != &copy)
			rbtree = std::move(copy.rbtree);
		return *this;
	}

	template<class Key, class Value, class Compare>
	void Map<Key, Value, Compare>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
		{
			bb.Write(*it);
		}
	}

	template<class Key, class Value, class Compare>
	bool Map<Key, Value, Compare>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	template<class Key, class Value, class Compare>
	bool Map<Key, Value, Compare>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
			std::pair<Key, Value> data;		//不能使用RBTreeData data;须去掉const
			if (!bb.Read(data))
			{
				data.~pair();
				return false;
			}
			Insert(data);
		}
		return true;
	}

	template<class Key, class Value, class Compare>
	void Map<Key, Value, Compare>::VarWriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(Size());
		for (ConstIterator it = Begin(); it != End(); ++it)
		{
			bb.VarWrite(*it);
		}
	}

	template<class Key, class Value, class Compare>
	bool Map<Key, Value, Compare>::VarReadFrom(ByteBuffer& bb)
	{
		return VarReadFrom(bb, 0, 0);
	}

	template<class Key, class Value, class Compare>
	bool Map<Key, Value, Compare>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
			std::pair<Key, Value> data;		//不能使用RBTreeData data;须去掉const
			if (!bb.VarRead(data))
			{
				data.~pair();
				return false;
			}
			Insert(data);
		}
		return true;
	}






	/*-------------------------------------------------------------------------------------------------------------------------*/



	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>::MultiMap()
		:MultiMap(32)
	{
	}

	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>::MultiMap(size_t capacity)
		: rbtree(capacity)
	{
	}

	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>::MultiMap(MultiMap const& copy)
		: rbtree(copy.rbtree)
	{
	}

	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>::MultiMap(MultiMap&& copy)
		: rbtree(std::move(copy.rbtree))
	{
	}

	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>& MultiMap<Key, Value, Compare>::operator=(MultiMap const& copy)
	{
		if (this != &copy)
			rbtree = copy.rbtree;
		return *this;
	}

	template<class Key, class Value, class Compare>
	MultiMap<Key, Value, Compare>& MultiMap<Key, Value, Compare>::operator=(MultiMap&& copy)
	{
		if (this != &copy)
			rbtree = std::move(copy.rbtree);
		return *this;
	}

}
