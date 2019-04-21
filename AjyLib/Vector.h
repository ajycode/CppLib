#pragma once

namespace Ajy
{

	class ByteBuffer;

	template <class T>
	class Vector :MemoryMoveable
	{
	private:
		T* buf = nullptr;
		size_t bufLen = 0;
		size_t dataLen = 0;
		template<class ...Args>
		void EmplaceInsert(size_t idx, Args&&... args);
	public:
		Vector();
		explicit Vector(size_t capacity);
		Vector(Vector const& copy);
		Vector(Vector&& copy);
		Vector& operator=(Vector const& copy);
		Vector& operator=(Vector&& copy);
		~Vector();
		bool operator==(Vector const& vec) const;
		bool operator!=(Vector const& vec) const;
		T* Buf() const;
		bool Empty() const;
		size_t DataLen() const;
		size_t BufLen() const;
		size_t DataByteLen() const;
		size_t BufByteLen() const;
		void Clear();
		void Reserve(size_t capacity);
		void Resize(size_t capacity, bool init = true);
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
		size_t Find(T const& v) const;
		T const& operator[](size_t idx) const;
		T& operator[](size_t idx);
		T const& At(size_t idx) const;
		T& At(size_t idx);
		void Set(size_t idx, T&& v);
		void Set(size_t idx, T const& v);
		void Insert(size_t idx, T const& v);
		void Insert(size_t idx, T&& v);
		void Remove(size_t idx);
		void FastRemove(size_t idx);			// swap with last item + Pop()
		void FastPush(T&& v);				// unsafe: do not check space enough
		void FastPush(T const& v);			// unsafe: do not check space enough

		// for Serialization
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
	};

	class BitArrayHelper;

	//Vector<T> specializated as a bit array when T is bool
	template<>
	class Vector<bool> :MemoryMoveable
	{
	private:
		char* buf = nullptr;
		size_t bufLen = 0;
		size_t dataLen = 0;
	public:
		Vector();							// Vector( 8 )
		explicit Vector(size_t capacity);
		~Vector();
		Vector(Vector<bool>&& copy);
		Vector(Vector<bool> const& copy);
		Vector& operator=(Vector<bool>&& copy);
		Vector& operator=(Vector<bool> const& copy);
		void Push(bool bit);
		template<class ...Args>
		void Push(bool bit, Args&&... args);
		void Pop();
		bool Top() const;
		void Clear();
		void Reserve(size_t capacity);
		bool Empty() const;
		size_t DataLen() const;
		size_t BufLen() const;
		size_t DataByteLen() const;
		size_t BufByteLen() const;
		BitArrayHelper operator[](size_t idx);
		bool operator[](size_t idx) const;
		bool At(size_t idx) const;
		void Set(size_t idx, bool bit);
		void SetRange(size_t idx, bool bit);
		template<class ...Args>
		void SetRange(size_t idx, bool bit, Args&&... args);
		void SetTrue(size_t idx);
		void SetFalse(size_t idx);
		void FillTrue();
		void FillFalse();
		void Fill(bool bit, size_t idxFrom, size_t idxTo);
		void Resize(size_t capacity, bool init = true);
		void CleanUp() const;   // clear last byte's unused bits

		// for Serialization
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
	};

	using BitArray = Vector<bool>;
	using BitArray_Ptr = Vector<bool>*;
	//typedef Vector<bool> BitArray;
	//typedef Vector<bool>* BitArray_Ptr;

	class BitArrayHelper
	{
	public:
		BitArrayHelper() = delete;
		BitArrayHelper(BitArray_Ptr _bitarr, size_t _idx);
		BitArrayHelper(BitArrayHelper const& copy);
		BitArrayHelper(BitArrayHelper&& copy);
		BitArrayHelper& operator=(BitArrayHelper const& copy);
		BitArrayHelper& operator=(BitArrayHelper&& copy);
		BitArrayHelper& operator=(bool _flag);
		~BitArrayHelper();
		operator bool();
		BitArray_Ptr bitarr = nullptr;
		size_t index;
	};

}
