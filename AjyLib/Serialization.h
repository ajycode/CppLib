#pragma once

namespace Ajy
{

	/*			数值压缩存储方法Varint
		Varint 是一种紧凑的表示数字的方法。它用一个或多个字节来表示一个数字，值越小的数字使用越少的字节数。这能减少用来表示数字的字节数。
	比如对于 int32 类型的数字，一般需要 4 个 byte 来表示。但是采用 Varint，对于很小的 int32 类型的数字，则可以用 1 个 byte 来表示。
	当然凡事都有好的也有不好的一面，采用 Varint 表示法，大的数字则需要 5 个 byte 来表示。从统计的角度来说，一般不会所有的消息中的数字都是大数，
	因此大多数情况下，采用 Varint 后，可以用更少的字节数来表示数字信息。
		Varint 中的每个 byte 的最高位 bit 有特殊的含义，如果该位为 1，表示后续的 byte 也是该数字的一部分，如果该位为 0，则结束。其他的 7 个 bit 都用来表示数字。
	由于负数的高位为1,所以采用这种压缩处理的时候必须负数转成正数
	*/

	#define Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED 1

	template <class Key, class Value>
	class HashMap;

	class ByteBuffer : MemoryMoveable
	{
	public:
		char*       buf;
		size_t      bufLen;
		size_t      dataLen;
		size_t      offset;
		bool        isRef;

#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		HashMap<void*, size_t>* ptrStore = nullptr;                                    // for 序列化含指针的类实例
		HashMap<size_t, void*>* idxStore = nullptr;                                    // for 反序列化含指针的类实例
#endif

		ByteBuffer();																// ByteBuffer( 1024 )
		explicit ByteBuffer(size_t capacity);										// prepare
		ByteBuffer(char* _buf, size_t _bufLen, size_t _dataLen = 0, bool isRef = false);   // Copy or ref
		ByteBuffer(ByteBuffer const& copy);                                      // Copy
		ByteBuffer(ByteBuffer&& copy);                                           // move
		ByteBuffer& operator=(ByteBuffer const& copy);                           // Copy
		ByteBuffer& operator=(ByteBuffer&& copy);                                // move
		void Destroy();                                                             // delete[] buf if need
		~ByteBuffer();
		void Assign(char const* _buf, size_t _bufLen, size_t _dataLen = 0, bool isRef = false);  // Copy or ref
		void Reserve(size_t capacity);
		void Resize(size_t len);
		void Clear();                                                               // dataLen = offset = 0
		bool Empty() const;                                                         // dataLen == 0
		bool Full() const;                                                          // dataLen == offset
		char operator[] (size_t idx) const;                                       // buf[ idx ]
		char& operator[] (size_t idx);                                            // buf[ idx ]
		char IndexAt(size_t idx) const;                                           // buf[ idx ]
		char& IndexAt(size_t idx);                                                // buf[ idx ]

		bool operator==(ByteBuffer const& other) const;
		bool operator!=(ByteBuffer const& other) const;

#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		// ptrStore, idxStore 相关
		void PtrStoreInit();                                                        // RootWrite 时自动调用 
		void PtrStoreDestroy();

		void IdxStoreInit();                                                        // RootRead 时自动调用
		void IdxStoreDestroy();
#endif

		// 最基础的读写函数实现体

		// 基本读写系列
		template<class T>
		static void WriteCore(char* dest, T const& src);
		template<class T>
		static void ReadCore(T& dest, char const* src);

		// 变长读写系列
		static void Write7Core(char* buf, size_t& offset, uint32 v);                // need ensure 7 space
		static bool Read7Core(uint32& v, char* buf, size_t len, size_t& offset);
		static bool TryRead7Core(uint32& v, char* buf, size_t len, size_t offset);

		// 64 位长版
		static void Write7Core(char* buf, size_t& offset, uint64 v);                // need ensure 9 space
		static bool Read7Core(uint64& v, char* buf, size_t len, size_t& offset);

		// negative -> ZigZag positive
		// 效果：负数变正，正数 *= 2
		static uint32   ZigZagEncode(int32    i);
		static uint64 ZigZagEncode(int64  i);

		// ZigZag positive -> negative
		static int32    ZigZagDecode(uint32   i);
		static int64  ZigZagDecode(uint64 i);


		// 正常写入系列：会预申请内存
		void Write(char const* _buf, size_t _dataLen);
		template<class T>
		void Write(T const& v);
		void Write(std::string const& v);
		template<class K, class V>
		void Write(std::pair<K, V> const& v);
		// 多值变参版
		template<class... Args>
		void WriteMulti(Args const&... args);

#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		// 带指针写入系列
		template<class T>
		void Write(T* v);
		template<class T>
		void RootWrite(T const& v);

		// 带指针读出系列
		template<class T>
		bool Read(T*& v);
		template<class T>
		bool RootRead(T& v);
#endif
		// 正常读取系列：返回 true 表示读出成功
		template<class T>
		bool Read(T& v);
		bool Read(std::string& v);
		template<class K, class V>
		bool Read(std::pair<K, V>& v);
		template<class T>
		bool Read(T& v, size_t minLen, size_t maxLen);       // 限长版
		bool Read(char* _buf, size_t _dataLen);
		template<class... Args>
		bool ReadMulti(Args&... args);

		// 变长写入系列：会预申请内存
		//varint紧凑数字表示法,对整数进行二进制的变长编码
		void VarWrite(int32 v);
		void VarWrite(uint32 v);
		void VarWrite(int64 v);
		void VarWrite(uint64 v);
		void VarWrite(long v);
		void VarWrite(ulong v);
		// todo: float, double
		template<class T>
		void VarWrite(T const& v);
		void VarWrite(std::string const& v);
		template<class K, class V>
		void VarWrite(std::pair<K, V> const& v);
		//// 多值变参版
		template<class... Args>
		void VarWriteMulti(Args const&... args);
		//变长读取系列
		bool VarRead(int32& v);
		bool VarRead(uint32& v);
		bool VarRead(int64& v);
		bool VarRead(uint64& v);
		bool VarRead(long& v);
		bool VarRead(ulong& v);
		bool TryVarRead(uint32& v);	// 不会更改偏移
		template<class T>
		bool VarRead(T& v);
		bool VarRead(std::string& v);
		template<class K, class V>
		bool VarRead(std::pair<K, V>& v);
		template<class T>
		bool VarRead(T& v, size_t minLen, size_t maxLen);       // 限长版
		template<class... Args>
		bool VarReadMulti(Args&... args);





		// interface for ByteBuffer
		void WriteTo(ByteBuffer& bb) const;
		bool ReadFrom(ByteBuffer& bb);
		bool ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& bb) const;
		bool VarReadFrom(ByteBuffer& bb);
		bool VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen);

	protected:

		template<class T>
		void WriteMultiCore(T const& v);
		template<class T, class... Args>
		void WriteMultiCore(T const& v, Args const&... args);

		template<class T>
		bool ReadMultiCore(T& v);
		template<class T, class... Args>
		bool ReadMultiCore(T& v, Args&... args);

		template<class T>
		void VarWriteMultiCore(T const& v);
		template<class T, class... Args>
		void VarWriteMultiCore(T const& v, Args const&... args);

		template<class T>
		bool VarReadMultiCore(T& v);
		template<class T, class... Args>
		bool VarReadMultiCore(T& v, Args&... args);
	};

}
