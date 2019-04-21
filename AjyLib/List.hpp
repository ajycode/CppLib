#pragma once

namespace Ajy
{

	template<class T>
	List<T>::List()
		:List(32)
	{
	}

	template<class T>
	List<T>::List(size_t capacity)
	{
		Init(capacity);
	}

	template<class T>
	List<T>::List(List const& copy)
		:List(copy.Size())
	{
		Insert(Begin(), copy.Begin(), copy.End());
	}

	template<class T>
	List<T>::List(List&& copy)
		: head(copy.head)
		, pool(std::move(copy.pool))
		, nodeCount(copy.nodeCount)
	{
		copy.Init(32);
	}

	template<class T>
	List<T>& List<T>::operator=(List const& copy)
	{
		if (this != &copy)
		{
			Assign(copy.Begin(), copy.End());
		}
		return *this;
	}

	template<class T>
	List<T>& List<T>::operator=(List&& copy)
	{
		if (this != &copy)
		{
			Clear();
			Dispose(head);
			head = copy.head;
			pool = std::move(copy.pool);
			nodeCount = copy.nodeCount;
			copy.Init(32);
		}
		return *this;
	}

	template<class T>
	List<T>::~List()
	{
		if (head)
		{
			Clear();
			Dispose(head);
			head = nullptr;
		}
	}

	template<class T>
	void List<T>::Init(size_t capacity)
	{
		if (capacity <= 0) capacity = 32;
		else capacity = Round2n(capacity);
		pool.Init<Node>(capacity);
		head = (NodePtr)pool.FastAlloc();
		new (&head->value) T();
		head->prev = head;
		head->next = head;
		nodeCount = 0;
	}

	template<class T>
	void List<T>::Assign(Iterator first, Iterator last)
	{
		Clear();
		Insert(Begin(), first, last);
	}

	template<class T>
	void List<T>::Clear()
	{
		if (head)
		{
			NodePtr cur = head->next;
			while (cur != head)
			{
				NodePtr curNext = cur->next;
				Dispose(cur);
				cur = curNext;
			}
		}
	}

	template<class T>
	template<class... Args>
	typename List<T>::Iterator List<T>::EmplaceInsert(Iterator pos, Args&&... args)
	{
		NodePtr insert = (NodePtr)pool.Alloc();
		new (&insert->value) T(std::forward<Args>(args)...);
		NodePtr insertNext = pos.node;
		insert->next = insertNext;
		insert->prev = insertNext->prev;
		insertNext->prev->next = insert;
		insertNext->prev = insert;
		++nodeCount;
		return Iterator(insert);
	}

	template<class T>
	void List<T>::Insert(Iterator pos, Iterator first, Iterator last)
	{
		while (first != last)
		{
			EmplaceInsert(pos, *first);
			++first;
		}
	}

	template<class T>
	void List<T>::Insert(Iterator pos, ConstIterator first, ConstIterator last)
	{
		while (first != last)
		{
			EmplaceInsert(pos, *first);
			++first;
		}
	}

	template<class T>
	typename List<T>::Iterator List<T>::Erase(Iterator pos)
	{
		if (pos == End()) return End();
		NodePtr freenodeNext = pos.node->next;
		NodePtr freenode = UnLink(pos);
		Dispose(freenode);
		pool.Free(freenode);
		--nodeCount;
		return Iterator(freenodeNext);
	}

	template<class T>
	typename List<T>::Iterator List<T>::Erase(Iterator first, Iterator last)
	{
		if (first == Begin() && last == End())
			Clear();
		else
			while (first != last)
				Erase(first++);
		return last;
	}

	template<class T>
	void List<T>::Remove(T const& t)
	{
		for (Iterator it = Begin(); it != End();)
		{
			if (*it == t) it = Erase(it);
			else ++it;
		}
	}

	template<class T>
	void List<T>::RemoveIf(Ajy::function<bool(T)> func)
	{
		for (Iterator it = Begin(); it != End();)
		{
			if (func(*it)) it = Erase(it);
			else ++it;
		}
	}

	template<class T>
	void List<T>::Resize(size_t size)
	{
		assert(size >= 0);
		if (!size) Clear();
		else if (size >= Size()) return;
		else
		{
			NodePtr search = head;
			while (size--) search = search->next;
			Erase(Iterator(search), End());
		}
	}

	template<class T>
	void List<T>::Reverse()
	{
		if (head == head->next || head == head->next->next) return;
		NodePtr tmp = head;
		NodePtr cur = head->next;
		while (cur != head)
		{
			NodePtr curNext = cur->next;
			cur->next = tmp;
			tmp->prev = cur;
			tmp = cur;
			cur = curNext;
		}
		head->next = tmp;
		tmp->prev = head;
	}

	template<class T>
	void List<T>::Sort(bool asc /* = true */)
	{
		if (head == head->next || head->next->next == head) return;
		NodePtr pPrev, p, pNext;
		size_t times = 0;		//外围循环次数 ，外围循环完成一次 值加 1. 
		size_t listLen = Size();
		while (head->next != head && times < listLen)
		{
			int compareTimes = 0; 		//比较的次数  
			pPrev = head;
			p = head->next;
			pNext = head->next->next;
			bool swaped = false;
			while (compareTimes != listLen - times - 1)	//外围循环完成一次 ,就冒泡一次(冒泡的值为链表未排序节点的最大值),相应的内围循环要比较的次数减 1. 
			{
				bool needSwap = false;
				if (asc)
				{
					if (p->value > pNext->value) needSwap = true;
				}
				else
				{
					if (p->value < pNext->value) needSwap = true;
				}
				if (needSwap)
				{
					swaped = true;
					p->next = pNext->next;
					pNext->next->prev = p;
					p->prev = pNext;
					pNext->next = p;
					pNext->prev = pPrev;
					pPrev->next = pNext;
					pPrev = pNext;
					pNext = p->next;
				}
				else
				{
					pPrev = p;
					p = p->next;
					pNext = p->next;
				}
				compareTimes++;
			}
			if (swaped == false)
			{
				break;
			}
			times++;
		}
	}


	template<class T>
	typename List<T>::NodePtr List<T>::UnLink(Iterator pos)
	{
		assert(pos.node != nullptr);
		NodePtr unlinknode = pos.node;
		unlinknode->prev->next = unlinknode->next;
		unlinknode->next->prev = unlinknode->prev;
		return unlinknode;
	}

	template<class T>
	void List<T>::Dispose(NodePtr dispose)
	{
		dispose->value.~T();
		dispose->prev = nullptr;
		dispose->next = nullptr;
	}

	template<class T>
	void List<T>::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(nodeCount);
		NodePtr search = head->next;
		while (search != head)
		{
			bb.Write(search->value);
			search = search->next;
		}
	}

	template<class T>
	bool List<T>::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	template<class T>
	bool List<T>::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
			T t;
			if (!bb.Read(t))
			{
				if (!std::is_pod<T>::value) t.~T();           // delete value
				return false;
			}
			PushBack(t);
		}
		return true;
	}

	template<class T>
	void List<T>::VarWriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite(nodeCount);
		NodePtr search = head->next;
		while (search != head)
		{
			bb.VarWrite(search->value);
			search = search->next;
		}
	}

	template<class T>
	bool List<T>::VarReadFrom(ByteBuffer& bb)
	{
		return VarReadFrom(bb, 0, 0);
	}

	template<class T>
	bool List<T>::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
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
			T t;
			if (!bb.VarRead(t))
			{
				if (!std::is_pod<T>::value) t.~T();           // delete value
				return false;
			}
			PushBack(t);
		}
		return true;
	}

}

