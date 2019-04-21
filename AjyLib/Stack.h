#pragma once

namespace Ajy
{

	template<class T>
	class Stack :MemoryMoveable
	{
	private:
		T* buf = nullptr;
		size_t bufLen = 0;
		size_t dataLen = 0;
	public:
		Stack();
		explicit Stack(size_t capacity);
		Stack(Stack const& copy);
		Stack(Stack&& copy);
		Stack& operator=(Stack const& copy);
		Stack& operator=(Stack&& copy);
		~Stack();
		void Clear();
		void Reserve(size_t capacity);
		bool IsEmpty() const;
		size_t BufLen() const;
		size_t DataLen() const;
		size_t BufByteLen() const;
		size_t DataByteLen() const;
		void Push(T const& v);
		void Push(T&& v);
		template<class ...Args>
		void Push(T const& v, Args&&... args);
		template<class ...Args>
		void Push(T&& v, Args&&... args);
		T& Top();
		T const& Top() const;
		void Pop();
		bool Pop(T& output);
		/*T& At(size_t idx);
		T const& At(size_t idx) const;*/
	};


}

