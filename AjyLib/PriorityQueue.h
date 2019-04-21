#pragma once

namespace Ajy
{
	template<class T, class Compare>
	void PushHeap(Vector<T>& vec, size_t childIndex, T val);

	template<class T, class Compare>
	void PopHeap(Vector<T>& vec);

	template<class T, class Compare = std::less<T>>
	class PriorityQueue :MemoryMoveable
	{
	public:
		PriorityQueue();
		PriorityQueue(size_t capacity);
		PriorityQueue(PriorityQueue const& copy);
		PriorityQueue(PriorityQueue&& copy);
		PriorityQueue& operator=(PriorityQueue const& copy);
		PriorityQueue& operator=(PriorityQueue&& copy);
		~PriorityQueue() {}
		bool Empty() const;
		size_t Size() const;
		void Push(T const& t);
		void Push(T&& t);
		T& Top();
		T const& Top() const;
		void Pop();
		//private:
		Vector<T> vec;
	};

}
