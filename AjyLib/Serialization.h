#pragma once

namespace Ajy
{

	/*			��ֵѹ���洢����Varint
		Varint ��һ�ֽ��յı�ʾ���ֵķ���������һ�������ֽ�����ʾһ�����֣�ֵԽС������ʹ��Խ�ٵ��ֽ��������ܼ���������ʾ���ֵ��ֽ�����
	������� int32 ���͵����֣�һ����Ҫ 4 �� byte ����ʾ�����ǲ��� Varint�����ں�С�� int32 ���͵����֣�������� 1 �� byte ����ʾ��
	��Ȼ���¶��кõ�Ҳ�в��õ�һ�棬���� Varint ��ʾ���������������Ҫ 5 �� byte ����ʾ����ͳ�ƵĽǶ���˵��һ�㲻�����е���Ϣ�е����ֶ��Ǵ�����
	��˴��������£����� Varint �󣬿����ø��ٵ��ֽ�������ʾ������Ϣ��
		Varint �е�ÿ�� byte �����λ bit ������ĺ��壬�����λΪ 1����ʾ������ byte Ҳ�Ǹ����ֵ�һ���֣������λΪ 0��������������� 7 �� bit ��������ʾ���֡�
	���ڸ����ĸ�λΪ1,���Բ�������ѹ�������ʱ����븺��ת������
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
		HashMap<void*, size_t>* ptrStore = nullptr;                                    // for ���л���ָ�����ʵ��
		HashMap<size_t, void*>* idxStore = nullptr;                                    // for �����л���ָ�����ʵ��
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
		// ptrStore, idxStore ���
		void PtrStoreInit();                                                        // RootWrite ʱ�Զ����� 
		void PtrStoreDestroy();

		void IdxStoreInit();                                                        // RootRead ʱ�Զ�����
		void IdxStoreDestroy();
#endif

		// ������Ķ�д����ʵ����

		// ������дϵ��
		template<class T>
		static void WriteCore(char* dest, T const& src);
		template<class T>
		static void ReadCore(T& dest, char const* src);

		// �䳤��дϵ��
		static void Write7Core(char* buf, size_t& offset, uint32 v);                // need ensure 7 space
		static bool Read7Core(uint32& v, char* buf, size_t len, size_t& offset);
		static bool TryRead7Core(uint32& v, char* buf, size_t len, size_t offset);

		// 64 λ����
		static void Write7Core(char* buf, size_t& offset, uint64 v);                // need ensure 9 space
		static bool Read7Core(uint64& v, char* buf, size_t len, size_t& offset);

		// negative -> ZigZag positive
		// Ч������������������ *= 2
		static uint32   ZigZagEncode(int32    i);
		static uint64 ZigZagEncode(int64  i);

		// ZigZag positive -> negative
		static int32    ZigZagDecode(uint32   i);
		static int64  ZigZagDecode(uint64 i);


		// ����д��ϵ�У���Ԥ�����ڴ�
		void Write(char const* _buf, size_t _dataLen);
		template<class T>
		void Write(T const& v);
		void Write(std::string const& v);
		template<class K, class V>
		void Write(std::pair<K, V> const& v);
		// ��ֵ��ΰ�
		template<class... Args>
		void WriteMulti(Args const&... args);

#if Ajy_BYTEBUFFER_PTR_SUPPORT_ENABLED
		// ��ָ��д��ϵ��
		template<class T>
		void Write(T* v);
		template<class T>
		void RootWrite(T const& v);

		// ��ָ�����ϵ��
		template<class T>
		bool Read(T*& v);
		template<class T>
		bool RootRead(T& v);
#endif
		// ������ȡϵ�У����� true ��ʾ�����ɹ�
		template<class T>
		bool Read(T& v);
		bool Read(std::string& v);
		template<class K, class V>
		bool Read(std::pair<K, V>& v);
		template<class T>
		bool Read(T& v, size_t minLen, size_t maxLen);       // �޳���
		bool Read(char* _buf, size_t _dataLen);
		template<class... Args>
		bool ReadMulti(Args&... args);

		// �䳤д��ϵ�У���Ԥ�����ڴ�
		//varint�������ֱ�ʾ��,���������ж����Ƶı䳤����
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
		//// ��ֵ��ΰ�
		template<class... Args>
		void VarWriteMulti(Args const&... args);
		//�䳤��ȡϵ��
		bool VarRead(int32& v);
		bool VarRead(uint32& v);
		bool VarRead(int64& v);
		bool VarRead(uint64& v);
		bool VarRead(long& v);
		bool VarRead(ulong& v);
		bool TryVarRead(uint32& v);	// �������ƫ��
		template<class T>
		bool VarRead(T& v);
		bool VarRead(std::string& v);
		template<class K, class V>
		bool VarRead(std::pair<K, V>& v);
		template<class T>
		bool VarRead(T& v, size_t minLen, size_t maxLen);       // �޳���
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
