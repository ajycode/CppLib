#pragma once

namespace Ajy
{

	template<class Value, class Comparator>
	SkipList<Value, Comparator>::SkipList()
		: SkipList(8)
	{
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>::SkipList(size_t maxlevel)
		: maxLevel(maxlevel)
	{
		Init();
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>::~SkipList()
	{
		if (head)
		{
			Clear();
			free(head);
			head = nullptr;
		}
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>::SkipList(SkipList const& copy)
		: SkipList(copy.GetMaxLevel())
	{
		CopyFrom(copy);
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>::SkipList(SkipList&& copy)
		: head(copy.head)
		, maxLevel(copy.maxLevel)
		, level(copy.level)
		, nodeCount(copy.nodeCount)
	{
		copy.head = nullptr;
		copy.Init();
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>& SkipList<Value, Comparator>::operator=(SkipList const& copy)
	{
		if (this != &copy)
		{
			Clear();
			free(head);
			maxLevel = copy.maxLevel;
			head = nullptr;
			Init();
			CopyFrom(copy);
		}
		return *this;
	}

	template<class Value, class Comparator>
	SkipList<Value, Comparator>& SkipList<Value, Comparator>::operator=(SkipList&& copy)
	{
		if (this != &copy)
		{
			head = copy.head;
			maxLevel = copy.maxLevel;
			level = copy.level;
			nodeCount = copy.nodeCount;
			copy.head = nullptr;
			copy.Init();
		}
		return *this;
	}

	template<class Value, class Comparator>
	void SkipList<Value, Comparator>::CopyFrom(SkipList const& copy)
	{
		level = copy.level;
		nodeCount = copy.nodeCount;
		NodePtr traverse = copy.head->next[0];
		NodePtr newNodePrev = head;
		while (traverse)
		{
			NodePtr newNode = CreateNode(traverse->val, traverse->nextCount - 1);
			traverse = traverse->next[0];
			newNodePrev->next[0] = newNode;
			newNodePrev = newNode;
		}
		traverse = copy.head;
		NodePtr traverse1 = head;
		while (traverse->next[0])
		{
			int len = traverse->nextCount > level ? level : traverse->nextCount;
			for (int i = 1; i < len; ++i)
			{
				if (traverse->next[i])
				{
					NodePtr findRes = FindGreaterOrEqual(traverse->next[i]->val, nullptr).node;
					traverse1->next[i] = findRes;
				}
				else
				{
					traverse1->next[i] = nullptr;
				}
			}
			traverse = traverse->next[0];
			traverse1 = traverse1->next[0];
		}
	}

	template<class Value, class Comparator>
	template<class Iter>
	void SkipList<Value, Comparator>::Insert(Iter const& first, Iter const& last)
	{
		for (Iter it = first; it != last; ++it)
		{
			Insert(*it);
		}
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::Erase(Iterator first, Iterator last)
	{
		if (first == Begin() && last == End())
			Clear();
		else
			while (first != last)
				Erase(first++);
		return last;
	}

	template<class Value, class Comparator>
	void SkipList<Value, Comparator>::Erase(Value const& val)
	{
		Iterator it = Find(val);
		Erase(it);
	}

	template<class Value, class Comparator>
	void SkipList<Value, Comparator>::Clear()
	{
		if (head)
		{
			NodePtr node = head->next[0];
			while (node)
			{
				NodePtr nodenext = node->next[0];
				free(node);
				node = nodenext;
			}
			for (int i = 0; i < head->nextCount; ++i)
			{
				head->next[i] = nullptr;
			}
			nodeCount = 0;
			level = 1;
		}
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::Find(Value const& val)
	{
		NodePtr node = FindGreaterOrEqual(val, nullptr).node;                // 查找大于或等于val的节点  
		if (node != nullptr && node->val == val)
		{
			return Iterator(node);
		}
		else
		{
			return End();
		}
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::NodePtr SkipList<Value, Comparator>::CreateNode(Value const& val, size_t _level)
	{
		NodePtr node = (NodePtr)malloc(sizeof(Node) + _level*sizeof(NodePtr));
		new (&node->val) Value(val);
		new (&node->nextCount) size_t(_level+1);
		for (size_t i = 0; i <= _level; ++i)
		{
			node->next[i] = nullptr;
		}
		return node;
	}

	template<class Value, class Comparator>
	void SkipList<Value, Comparator>::Init()
	{
		if (!head)
		{
			head = CreateNode(Value(), maxLevel - 1);
			new (&head->val) Value();
			nodeCount = 0;
			level = 1;
		}
	}

	template<class Value, class Comparator>
	size_t SkipList<Value, Comparator>::RandomLevel()
	{
		const size_t kBranching = 4;
		size_t height = 1;
		while (height < maxLevel && ((random.GetInt(0, 9) % kBranching) == 0))
		{
			++height;
		}
		return height;
	}

	template<class Value, class Comparator>
	bool SkipList<Value, Comparator>::IsValueAfterNode(Value const& val, NodePtr node)
	{
		return (node != nullptr) && valComparator(node->val, val);
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::FindGreaterOrEqual(Value const& val, NodePtr* prev)
	{
		NodePtr node = head;
		int height = level - 1;
		while (true)
		{
			NodePtr next = node->next[height];
			if (IsValueAfterNode(val, next))
			{
				node = next;
			}
			else
			{
				if (prev != nullptr)
				{
					prev[height] = node;
				}
				else
				{
					if (next && val == next->val)
					{
						return Iterator(next);
					}
				}
				if (height == 0)
				{
					return Iterator(next);
				}
				else
				{
					--height;
				}
			}
		}
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::FindLess(Value const& val)
	{
		NodePtr node = head;
		int height = level - 1;
		while (true)
		{
			assert(node == head || valComparator(node->val, val) < 0);
			NodePtr next = node->next[height];
			if (next == nullptr || valComparator(next->val, val) >= 0)
			{
				if (height == 0)
				{
					return node;
				}
				else
				{
					--height;
				}
			}
			else
			{
				node = next;
			}
		}
	}

	template<class Value, class Comparator>
	template<class ...Args>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::EmplaceInsert(Args&&... args)
	{
		Value val(std::forward<Args>(args)...);
		NodePtr* prev = new NodePtr[maxLevel];
		for (size_t i = 0; i < maxLevel; ++i)
		{
			prev[i] = nullptr;
		}
		NodePtr node = FindGreaterOrEqual(val, prev).node;                                // prev记录每个level上前一个节点  
		if (node && node->val == val)
		{
			return End();
		}
		size_t height = RandomLevel();
		if (height > level)
		{
			for (size_t i = level; i < height; ++i)
			{
				prev[i] = head;
			}
			level = height;
		}

		node = CreateNode(val, height-1);
		for (size_t i = 0; i < height; i++)
		{
			// 根据当前节点的level层数，设置每个level的指针   
			node->next[i] = prev[i]->next[i];
			prev[i]->next[i] = node;
		}
		++nodeCount;
		delete[] prev;
		return Iterator(node);
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::Insert(Value const& val)
	{
		return EmplaceInsert(val);
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::Insert(Value&& val)
	{
		return EmplaceInsert((Value&&)val);
	}

	template<class Value, class Comparator>
	typename SkipList<Value, Comparator>::Iterator SkipList<Value, Comparator>::Erase(NodePtr node)
	{
		if (!node) return End();
		NodePtr next = node->next[0];
		NodePtr* prev = new NodePtr[maxLevel];
		for (int i = 0; i < maxLevel; ++i)
		{
			prev[i] = nullptr;
		}
		NodePtr p = head, q = nullptr;
		for (int i = level - 1; i >= 0; --i)
		{
			while ((q = p->next[i]) && IsValueAfterNode(node->val, q))
			{
				p = q;
			}
			prev[i] = p;
		}
		//逐层删除，和普通列表删除一样  
		for (int i = 0; i < level; ++i)
		{
			if (prev[i]->next[i] == q)
			{
				prev[i]->next[i] = q->next[i];
			}
		}
		free(node);
		//如果删除的是最大层(层数大于1)的节点，那么需要重新维护跳表
		if (level>1)
		{
			for (int i = level - 1; i >= 0; --i)
			{
				if (head->next[i] == nullptr)
				{
					--level;
				}
			}
		}
		delete[] prev;
		--nodeCount;
		return Iterator(next);
	}

}

