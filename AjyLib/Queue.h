#pragma once

namespace Ajy
{

	// 3 situation:
	//...............FR...............		// Front == Rear
	//......Front+++++++++++Rear......		// DataLen = Rear - Front
	// ++++++Rear...........Front++++++		// DataLen = BufLen - Front + Rear

	template<class T>
	class Queue :MemoryMoveable
	{
	private:
		size_t front, rear;
		size_t bufLen;
		T* buf;
		template<class... Args>
		T& EmplacePush(Args&&... args);			// [ Rear++ ] = T( ps )
	public:
		Queue();								// Queue( 8 )
		explicit Queue(size_t capacity);
		~Queue();
		Queue(Queue const& copy);
		Queue(Queue&& copy);
		Queue& operator=(Queue const& copy);
		Queue& operator=(Queue&& copy);
		//T const& operator[](size_t idx) const;	// [0] = Front()
		//T& operator[](size_t idx);
		//T const& At(size_t idx) const;		// like []
		//T& At(size_t idx);
		size_t Size() const;
		bool Empty() const;
		void Clear();
		void Reserve(size_t capacity, bool isFull = false);
		template<typename ...Args>
		void Push(T const& v, Args&&... args);
		template<typename ...Args>
		void Push(T&& v, Args&&... args);
		void Push(T const& v);
		void Push(T&& v);
		T& Front();
		T const& Front() const;
		void Pop();								// ++Front
		bool Pop(T& outVal);
		size_t PopMulti(size_t count);				// Front += count
	};

}

