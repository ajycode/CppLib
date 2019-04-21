#pragma once

namespace Ajy
{

	class ByteBuffer;

	struct StringFormatHelper
	{
		size_t idx, len;
	};

	class String :MemoryMoveable
	{
	public:
		String();													// String( 64 )
		explicit String(size_t capacity);
		String(char const* _buf, size_t _bufLen, size_t _dataLen, bool isRef);    // ref or Copy
		String(char const* s, bool isRef = false);                // Copy or ref
		String(size_t begin, size_t _dataLen, char const* s, bool isRef = false);	// Copy
		String(String const& copy);                                  // Copy
		String(String&& copy);                                       // move
		String& operator=(String const& copy);                       // Copy
		String& operator=(String&& copy);                            // move
		~String();                                                  // Dispose _buf
		void Assign(char const* _buf, size_t _bufLen, size_t _dataLen, bool isRef);     // ref or Copy
		void Assign(char const* s, size_t sLen = 0, bool isRef = false);      // Copy or ref
		void Reserve(size_t capacity);
		void Resize(size_t len, bool fillZero = true);									// cut( maybe damage utf8 ) or grow up ( does not set 0 )
		void Clear();                                               // Set dataLen to 0
		char const* C_str() const;												// return buf
		std::string Std_str() const;
		size_t Size() const;
		char& Front();
		char const& Front() const;
		char& Back();
		char const& Back() const;
		char const& operator[] (size_t idx) const;                // return buf[ idx ]
		char& operator[] (size_t idx);                            // return buf[ idx ]
		char const& At(size_t idx) const;
		char& At(size_t idx);
		void Push(char c);
		void Pop();
		char& Top();
		char const& Top() const;
		bool Empty() const;
		void Insert(size_t idx, char const* s, size_t sLen = 0);
		void Insert(size_t idx, String const& s);
		void Erase(size_t idx);
		String Substr(size_t begin, size_t len) const;
		std::pair<size_t,Vector<size_t>> FindAll(char const* match) const;
		std::pair<size_t, Vector<size_t>> FindAll(String const& match) const;
		std::pair<size_t, Vector<size_t>> FindAllByKMP(char const* match, size_t begin, size_t end) const;
		std::pair<size_t, Vector<size_t>> FindAllByBM(char const* match, size_t begin, size_t end) const;
		std::pair<size_t, Vector<size_t>> FindAllBySunday(char const* match, size_t begin, size_t end) const;
		std::pair<size_t, Vector<size_t>> FindAllByHorspool(char const* match, size_t begin, size_t end) const;
		Vector<String> Split(char delimiter) const;
		Vector<String> Split(char const* delimiter) const;
		Vector<String> Split(String const& delimiter) const;
		bool operator==(String const& o) const;
		bool operator!=(String const& o) const;
		bool operator<(String const& o) const;
		bool operator>(String const& o) const;
		bool operator<=(String const& o) const;
		bool operator>=(String const& o) const;
		bool operator==(char const* o) const;
		template<size_t sLen>
		bool operator==(char const(&s)[sLen]) const;                         //规定长度的char数组形参
		size_t GetHashCode() const;

		template<class... Args>
		void Append(Args const&... args);
		template<class... Args>
		void AppendFormat(char const* format, Args const&... args);
		template<class T>
		void AppendHex(T const& v);
		template<class T>
		String& operator<<(T const& v);
		template<class... Args>
		static String MakeString(Args const&... args);
		template<class... Args, size_t _bufLen>
		static String MakeString(char(&_buf)[_bufLen], Args const&... args);      //规定长度的char数组形参
		template<class... Args>
		static String MakeFormatString(char const* format, Args const&... args);
		template<class T>
		static String ToString(T const& v);
		template<class T>
		static String ToHexString(T const& v);

		// for dump binary data
		void DumpCore(char const* buf, size_t len);
		void Dump(char const* buf, size_t len);

		// for Serialization
		void WriteTo(ByteBuffer& fb) const;
		bool ReadFrom(ByteBuffer& fb);
		bool ReadFrom(ByteBuffer& fb, size_t minLen, size_t maxLen);
		void VarWriteTo(ByteBuffer& fb) const;
		bool VarReadFrom(ByteBuffer& fb);
		bool VarReadFrom(ByteBuffer& fb, size_t minLen, size_t maxLen);
	private:
		static void GetNextArrayOfKMP(char const* pattern, size_t len, int next[]);
		static int* MakeSkipOfBM(char const* pattern, size_t len);
		static int* MakeShiftOfBM(char const* pattern, size_t len);
		template<class... Args>
		static size_t Fill(char* _buf, Args const&... args);
		template<class... Args>
		static size_t FillHex(char * _buf, Args const&... args);
		template<class T>
		static void FillCore(char*& _buf, size_t& offset, T const& v);
		template<class T, class... Args>
		static void FillCore(char*& _buf, size_t& offset, T const& v, Args const&... args);
		template<class T>
		static void FillHexCore(char*& _buf, size_t& offset, T const& v);
		template<class T, class... Args>
		static void FillHexCore(char*& _buf, size_t& offset, T const& v, Args const&... args);
		template<class T>
		static void GetFillMaxLengthCore(size_t& len, T const& v);
		template<class T, class... Args>
		static void GetFillMaxLengthCore(size_t& len, T const& v, Args const&... args);
		template<class... Args>
		static size_t GetFillMaxLength(Args const&... args);
		template<class T>
		void AppendFormatCore(String& s, size_t& n, T const& v);
		template<class T, class... Args>
		void AppendFormatCore(String& s, size_t& n, T const& v, Args const&... args);
		void Init(size_t capacity);
		void Dispose();
	private:
		char*       buf = nullptr;
		size_t      bufLen = 0;
		size_t      dataLen = 0;
		bool		isRef = false;
	} const emptyString(nullptr, 0, 0, true);

}
