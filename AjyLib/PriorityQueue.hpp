#pragma once

namespace Ajy
{

	template<class T, class Compare>
	void PushHeap(Vector<T>& vec, size_t childIndex, T val)
	{
		if (vec.DataLen() == 1) return;
		Compare comp;
		size_t origin_childIndex = childIndex;
		size_t parentIndex = (childIndex - 1) / 2;
		while (childIndex > 0 && comp(vec[parentIndex], val))
		{
			vec[childIndex] = vec[parentIndex];
			childIndex = parentIndex;
			parentIndex = (childIndex - 1) / 2;
		}
		if (origin_childIndex != childIndex)
			vec[childIndex] = val;
	}

	template<class T, class Compare>
	void PopHeap(Vector<T>& vec)
	{
		if (vec.Empty() || vec.DataLen() == 1) return;
		Compare comp;
		size_t parentIndex = 0;
		size_t rightChildIndex = 2 * parentIndex + 2;
		while (rightChildIndex < vec.DataLen())
		{
			if (comp(vec[rightChildIndex], vec[rightChildIndex - 1]))
				--rightChildIndex;
			vec[parentIndex] = vec[rightChildIndex];
			parentIndex = rightChildIndex;
			rightChildIndex = 2 * parentIndex + 2;
		}
		if (rightChildIndex == vec.DataLen())		//只有左孩子,此时左孩子即Top,退出操作
			vec[parentIndex] = vec[rightChildIndex - 1];
		else
		{
			if (parentIndex == vec.DataLen() - 1)		//自身即Top,退出操作
			{
			}
			else									//位于最底层（非TOP）或倒数第二层
			{
				T val = vec.Top();
				vec[parentIndex] = val;
				PushHeap<T, Compare>(vec, parentIndex, val);
			}
		}
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>::PriorityQueue()
		:PriorityQueue(8)
	{
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>::PriorityQueue(size_t capacity)
		: vec(capacity)
	{
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>::PriorityQueue(PriorityQueue const& copy)
		: vec(copy.vec)
	{
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>::PriorityQueue(PriorityQueue&& copy)
		: vec(std::move(copy.vec))
	{
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(PriorityQueue const& copy)
	{
		if (this != &copy)
		{
			vec = copy.vec;
		}
		return *this;
	}

	template<class T, class Compare>
	PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(PriorityQueue&& copy)
	{
		if (this != &copy)
		{
			vec = std::move(copy.vec);
		}
		return *this;
	}

	template<class T, class Compare>
	bool PriorityQueue<T, Compare>::Empty() const
	{
		return vec.Empty();
	}

	template<class T, class Compare>
	size_t PriorityQueue<T, Compare>::Size() const
	{
		return vec.DataLen();
	}

	template<class T, class Compare>
	void PriorityQueue<T, Compare>::Push(T const& t)
	{
		vec.Push(t);
		PushHeap<T, Compare>(vec, vec.DataLen() - 1, vec.Top());
	}

	template<class T, class Compare>
	void PriorityQueue<T, Compare>::Push(T&& t)
	{
		vec.Push((T&&)t);
		PushHeap<T, Compare>(vec, vec.DataLen() - 1, vec.Top());
	}

	template<class T, class Compare>
	T& PriorityQueue<T, Compare>::Top()
	{
		return vec[0];
	}

	template<class T, class Compare>
	T const& PriorityQueue<T, Compare>::Top() const
	{
		return vec[0];
	}

	template<class T, class Compare>
	void PriorityQueue<T, Compare>::Pop()
	{
		PopHeap<T, Compare>(vec);
		vec.Pop();
	}

}
