#include "AjyLib.h"

namespace Ajy
{
	
	String::String(size_t capacity)
	{
		Init(capacity);
	}


	String::String()
		: String(64)
	{
	}

	String::String(char const* _buf, size_t _bufLen, size_t _dataLen, bool isRef)
	{
		this->isRef = isRef;
		dataLen = _dataLen;
		if (isRef)
		{
			buf = (char*)_buf;
			bufLen = _bufLen;
		}
		else
		{
			bufLen = Round2n(_dataLen);
			buf = new char[bufLen];
			memcpy(buf, _buf, _dataLen);
			buf[dataLen] = '\0';
		}
	}

	String::String(char const* s, bool isRef)
	{
		this->isRef = isRef;
		dataLen = strlen(s);
		if (isRef)
		{
			buf = (char*)s;
			bufLen = dataLen + 1;
		}
		else
		{
			bufLen = Round2n(dataLen + 1);
			buf = new char[bufLen];
			memcpy(buf, s, dataLen + 1);
		}
	}

	String::String(size_t begin, size_t _dataLen, char const* s, bool isRef /* = false */)
	{
		if (begin < 0)
		{
			begin = 0;
		}
		size_t slen = std::strlen(s);
		if (_dataLen > slen)
		{
			_dataLen = slen;
		}
		this->isRef = isRef;
		dataLen = _dataLen;
		if (isRef)
		{
			buf = (char*)s;
			bufLen = dataLen + 1;
		}
		else
		{
			bufLen = Round2n(dataLen + 1);
			buf = new char[bufLen];
			memcpy(buf, s+begin, dataLen);
			buf[dataLen] = '\0';
		}
	}

	String::String(String const& copy)
		: String(copy.dataLen + 1)
	{
		memcpy(buf, copy.buf, copy.dataLen + 1);
		dataLen = copy.dataLen;
	}

	String::String(String&& copy)
		: buf(copy.buf)
		, bufLen(copy.bufLen)
		, dataLen(copy.dataLen)
	{
		copy.buf = nullptr;
		copy.bufLen = 0;
		copy.dataLen = 0;
		copy.Init(64);
	}


	String& String::operator=(String const& copy)
	{
		if (this != &copy)
		{
			dataLen = copy.dataLen;
			if (bufLen >= copy.dataLen)
			{
				memcpy(buf, copy.buf, copy.dataLen + 1);
			}
			else
			{
				Dispose();
				bufLen = Round2n(copy.dataLen);
				buf = new char[bufLen];
				memcpy(buf, copy.buf, copy.dataLen + 1);
			}
		}
		return *this;
	}

	String& String::operator=(String&& copy)
	{
		if (this != &copy)
		{
			Dispose();
			buf = copy.buf;
			bufLen = copy.bufLen;
			dataLen = copy.dataLen;
			copy.buf = nullptr;
			copy.bufLen = 0;
			copy.dataLen = 0;
			copy.Init(32);
		}
		return *this;
	}


	String::~String()
	{
		Dispose();
		buf = nullptr;
	}

	void String::Assign(char const* _buf, size_t _bufLen, size_t _dataLen, bool isRef)
	{
		assert(buf != _buf);
		this->isRef = isRef;
		if (isRef)
		{
			Dispose();
			buf = (char*)_buf;
			bufLen = _bufLen;
			dataLen = _dataLen;
			return;
		}
		if (bufLen <= _dataLen)
		{
			Dispose();
			bufLen = Round2n(_dataLen);
			buf = new char[bufLen];
		}
		dataLen = _dataLen;
		memcpy(buf, _buf, _dataLen);
		buf[dataLen] = '\0';
	}

	void String::Assign(char const* s, size_t sLen /* = 0 */, bool isRef /* = false */)
	{
		if (!sLen) 
			sLen = strlen(s);
		Assign(s, sLen, sLen, isRef);
	}

	void String::Reserve(size_t capacity)
	{
		if (capacity + 1 <= bufLen) return;
		bufLen = Round2n(capacity + 1);
		char* newBuf = new char[bufLen];
		memcpy(newBuf, buf, dataLen + 1);
		Dispose();
		buf = newBuf;
	}

	void String::Resize(size_t len, bool fillZero /*= true*/)
	{
		if (len == dataLen) return;
		else if (len < dataLen)
		{
			dataLen = len;
			buf[dataLen] = '\0';
		}
		else
		{
			Reserve(len);
			if (fillZero) memset(buf + dataLen, 0, len - dataLen + 1);
			dataLen = len;
		}
	}

	void String::Clear()
	{
		dataLen = 0;
		buf[0] = '\0';
	}

	char const* String::C_str() const
	{
		return buf;
	}

	std::string String::Std_str() const
	{
		return std::string(buf);
	}

	size_t String::Size() const
	{
		return dataLen;
	}

	char& String::Front()
	{
		return buf[0];
	}
	char const& String::Front() const
	{
		return buf[0];
	}
	char& String::Back()
	{
		return buf[dataLen-1];
	}
	char const& String::Back() const
	{
		return buf[dataLen-1];
	}

	char const& String::operator[](size_t idx) const
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}
	char& String::operator[](size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	char const& String::At(size_t idx) const
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	char& String::At(size_t idx)
	{
		assert(idx >= 0 && idx < dataLen);
		return buf[idx];
	}

	void String::Push(char c)
	{
		if (dataLen + 1 == bufLen)
		{
			Reserve(dataLen + 1);
		}
		buf[dataLen] = c;
		buf[dataLen + 1] = '\0';
		++dataLen;
	}

	void String::Init(size_t capacity)
	{
		if (capacity < 64) bufLen = 64;
		else bufLen = Round2n(capacity);
		dataLen = 0;
		buf = new char[bufLen];
		buf[0] = '\0';
		isRef = false;
	}

	void String::Dispose()
	{
		if (buf && !isRef)
		{
			delete[] buf;
		}
	}

	void String::Pop()
	{
		assert(dataLen > 0);
		buf[--dataLen] = '\0';
	}

	char& String::Top()
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}

	char const& String::Top() const
	{
		assert(dataLen > 0);
		return buf[dataLen - 1];
	}

	bool String::Empty() const
	{
		return dataLen == 0;
	}

	void String::Insert(size_t idx, char const* s, size_t sLen)
	{
		if (idx < 0)
		{
			idx = 0;
		}
		if (idx >= dataLen)
		{
			Append(String(s, sLen, sLen, true));
			return;
		}
		if (!sLen)
		{
			sLen = strlen(s);
			if (!sLen) return;
		}
		Reserve(dataLen + sLen);
		memmove(buf + idx + sLen, buf + idx, dataLen - idx);
		memcpy(buf + idx, s, sLen);
		dataLen += sLen;
		buf[dataLen] = '\0';
	}

	void String::Insert(size_t idx, String const& s)
	{
		Insert(idx, s.buf, s.dataLen);
	}

	void String::Erase(size_t idx)
	{
		if (idx < 0)
		{
			idx = 0;
		}
		if (idx >= dataLen)
		{
			idx = dataLen - 1;
		}
		memmove(buf + idx, buf + idx + 1, dataLen - idx);
		--dataLen;
		buf[dataLen] = '\0';
	}

	String String::Substr(size_t begin, size_t len) const
	{
		return String(begin, len, buf);
	}

	std::pair<size_t, Vector<size_t>> String::FindAll(char const* match) const
	{
		return FindAllBySunday(match, 0, std::strlen(match));
		//return FindAllByBM(match, 0, std::strlen(match));
		//return FindAllByHorspool(match, 0, std::strlen(match));
		//return FindAllByKMP(match, 0, std::strlen(match));
	}

	std::pair<size_t, Vector<size_t>> String::FindAll(String const& match) const
	{
		return FindAll(match.C_str());
	}

	std::pair<size_t, Vector<size_t>> String::FindAllByKMP(char const* match, size_t begin, size_t end) const
	{
		Vector<size_t> beginIdxResults;
		if (match == "")
		{
			return std::make_pair(0, beginIdxResults);
		}
		size_t matchLen = std::strlen(match);
		if (begin < 0)
		{
			begin = 0;
		}
		if (end > matchLen -1)
		{
			end = matchLen -1;
		}
		matchLen = end + 1 - begin;
		int* next = new int[matchLen];
		char const* actualMatch = match + begin;
		//  得到模式串的next数组   
		GetNextArrayOfKMP(actualMatch, matchLen, next);
		for (int i = 0, j = 0; i < dataLen; )
		{  
			if (-1 == j ||  buf[i] == actualMatch[j])  // 如果模式串游标已经回退到第一个字符或者匹配成功    
			{
				// 满足以上两种情况时两个游标都要向前进一步   
				++i;
				++j;
				if ( j == matchLen )
				{
					j = 0;
					beginIdxResults.Push(i - matchLen);
				}
			}
			else                          // 匹配不成功,模式串游标回退到当前字符的next值   
			{
				j = next[j];
			}
		}
		delete next;
		return std::make_pair(matchLen,beginIdxResults);
	}

	std::pair<size_t, Vector<size_t>> String::FindAllByBM(char const* match, size_t begin, size_t end) const
	{
		Vector<size_t> beginIdxResults;
		if (match == "")
		{
			return std::make_pair(0, beginIdxResults);
		}
		size_t matchLen = std::strlen(match);
		if (begin < 0)
		{
			begin = 0;
		}
		if (end > matchLen -1)
		{
			end = matchLen -1;
		}
		matchLen = end + 1 - begin;
		char const* actualMatch = match + begin;
		int* skip = MakeSkipOfBM(actualMatch, matchLen);
		int* shift = MakeShiftOfBM(actualMatch, matchLen);
		int b_idx = matchLen;
		while (b_idx <= dataLen)		// 计算字符串是否匹配到了尽头  
		{
			int p_idx = matchLen, skip_stride, shift_stride;
			while (buf[--b_idx] == actualMatch[--p_idx])		// 开始匹配  
			{
				if (b_idx < 0)
					return std::make_pair(matchLen, beginIdxResults);
				if (p_idx == 0)
				{
					beginIdxResults.Push(b_idx);
					b_idx += (matchLen+1);
				}
			}
			skip_stride = skip[(unsigned char)buf[b_idx]];		// 根据坏字符规则计算跳跃的距离
			shift_stride = shift[p_idx];						// 根据好后缀规则计算跳跃的距离  
			b_idx += (skip_stride > shift_stride) ? skip_stride : shift_stride;		// 取大者  
		}

		return std::make_pair(matchLen, beginIdxResults);
	}

	std::pair<size_t, Vector<size_t>> String::FindAllBySunday(char const* match, size_t begin, size_t end) const
	{
		Vector<size_t> beginIdxResults;
		if (match == "")
		{
			return std::make_pair(0, beginIdxResults);
		}
		size_t matchLen = std::strlen(match);
		if (begin < 0)
		{
			begin = 0;
		}
		if (end > matchLen - 1)
		{
			end = matchLen - 1;
		}
		matchLen = end + 1 - begin;
		char const* actualMatch = match + begin;
		int next[128];
		for (int j = 0; j < 128; ++j)
		{
			next[j] = matchLen + 1;
		}
		for (int j = 0; j < matchLen; ++j)
		{
			next[actualMatch[j]] = matchLen - j;			// 记录字符到最右段的最短距离+1 
		}
		int pos = 0;
		while (pos < (dataLen - matchLen + 1))		// 末端对齐  
		{
			int i = pos;
			int j;
			for (j = 0; j < matchLen; ++j, ++i)
			{
				if (buf[i] != actualMatch[j])
				{
					pos += next[buf[pos + matchLen]];
					// 不等于就跳跃,跳跃是核心  
					break;
				}
			}
			if (j == matchLen)
			{
				beginIdxResults.Push(pos);
				++pos;
			}
		}
		return std::make_pair(matchLen, beginIdxResults);
	}

	std::pair<size_t, Vector<size_t>> String::FindAllByHorspool(char const* match, size_t begin, size_t end) const
	{
		Vector<size_t> beginIdxResults;
		if (match == "")
		{
			return std::make_pair(0, beginIdxResults);
		}
		size_t matchLen = std::strlen(match);
		if (begin < 0)
		{
			begin = 0;
		}
		if (end > matchLen - 1)
		{
			end = matchLen - 1;
		}
		matchLen = end + 1 - begin;
		char const* actualMatch = match + begin;
		int next[128];
		int i, pos, j;
		for (i = 0; i < 128; ++i)
		{
			next[i] = matchLen;
		}
		for (i = 0; i < matchLen; ++i)
		{
			next[actualMatch[i]] = matchLen - i - 1;
		}
		pos = 0;
		while (pos < (dataLen - matchLen)) 
		{
			j = matchLen - 1;
			while (j >= 0 && buf[pos + j] == actualMatch[j])  // matching  
			{
				--j;
			}
			if (j == -1)
			{
				beginIdxResults.Push(pos);
				++pos;
			}
			else		// not matched  
			{
				pos += next[actualMatch[pos + matchLen - 1]];
			}
		}
		return std::make_pair(matchLen, beginIdxResults);
	}

	Vector<String> String::Split(char delimiter) const
	{
		Vector<String> res;
		size_t splitHelper = 0;
		for (size_t i = 0; i < dataLen; ++i)
		{
			if (delimiter == buf[i] || i == dataLen - 1)
			{
				if (delimiter == buf[i])
				{
					String str = String(splitHelper, i - splitHelper, buf);
					splitHelper = i + 1;
					if (str != emptyString)
					{
						res.Push(str);
					}
				}
				else
				{
					String str = String(splitHelper, dataLen - splitHelper, buf);
					splitHelper = i + 1;
					if (str != emptyString)
					{
						res.Push(str);
					}
				}
			}
		}
		return res;
	}

	Vector<String> String::Split(char const* delimiter) const
	{
		Vector<String> res;
		std::pair<size_t, Vector<size_t>> matchs = FindAll(delimiter);
		size_t strLen = matchs.first;
		for (size_t i = 0; i < matchs.second.DataLen(); ++i)
		{
			size_t strBegin = matchs.second[i];
			String str = String(strBegin, strLen, buf);
			res.Push(str);
		}
		return res;
	}

	Vector<String> String::Split(String const& delimiter) const
	{
		return Split(delimiter.C_str());
	}

	void String::GetNextArrayOfKMP(char const* pattern, size_t len, int next[])
	{
		//  第一个字符的next值是-1,因为C中的数组是从0开始的   
		next[0] = -1;
		for (int i = 0, j = -1; i < len - 1; )
		{  
			if (-1 == j || pattern[i] == pattern[j])     // 如果模式串游标已经回退到第一个字符或者匹配成功   
			{
				// 两个游标都向前走一步   
				++i;
				++j;
				// 存放当前的next值为此时模式串的游标值   
				next[i] = j;
			}
			else                                  // 匹配不成功j就回退到上一个next值   
			{
				j = next[j];
			}
		}
	}

	/*
	目的：根据坏字符规则做预处理，建立一张坏字符表
	参数：
	pattern => 模式串P
	len => 模式串P长度
	返回：
	int* => 坏字符表
	*/
	int* String::MakeSkipOfBM(char const* pattern, size_t len)
	{
		int i;
		//为建立坏字符表，申请256个int的空间  
		/*PS:之所以要申请256个，是因为一个字符是8位，
		所以字符可能有2的8次方即256种不同情况*/
		int *skip = new int[256];

		//初始化坏字符表，256个单元全部初始化为pLen，没有在模式串出现的字符距离为pLen。  
		for (i = 0; i < 256; i++)
		{
			*(skip + i) = len;
		}

		//给表中需要赋值的单元赋值，不在模式串中出现的字符就不用再赋值了  
		while (len != 0)
		{
			*(skip + (unsigned char)*pattern++) = len--;
		}

		return skip;
	}

	/*
	目的：根据好后缀规则做预处理，建立一张好后缀表
	参数：
	pattern => 模式串P
	len => 模式串P长度
	返回：
	int* => 好后缀表
	*/
	int* String::MakeShiftOfBM(char const* pattern, size_t len)
	{
		int *shift = new int[len];				// 为好后缀表申请pLen个int的空间 
		int *sptr = shift + len - 1;			// 方便给好后缀表进行赋值的指标  
		char const* pptr = pattern + len - 1;	// 记录好后缀表边界位置的指标  
		char c = *(pattern + len - 1);			// 保存模式串中最后一个字符，因为要反复用到它 
		*sptr = 1;								// 以最后一个字符为边界时，确定移动1的距离  
		pptr--;		// 边界移动到倒数第二个字符（这句是我自己加上去的，因为我总觉得不加上去会有BUG，大家试试“abcdd”的情况，即末尾两位重复的情况）  

		while (sptr-- != shift)		// 该最外层循环完成给好后缀表中每一个单元进行赋值的工作  
		{
			char const* p1 = pattern + len - 2, *p2, *p3;
			// 该do...while循环完成以当前pptr所指的字符为边界时，要移动的距离  
			do 
			{
				while (p1 >= pattern && *p1-- != c);	// 该空循环，寻找与最后一个字符c匹配的字符所指向的位置  
				p2 = pattern + len - 2;
				p3 = p1;
				while (p3 >= pattern && *p3-- == *p2-- && p2 >= pptr);		// 该空循环，判断在边界内字符匹配到了什么位置  
			} while (p3 >= pattern && p2 >= pptr);
			*sptr = shift + len - sptr + p2 - p3;		// 保存好后缀表中，以pptr所在字符为边界时，要移动的位置  
			
			/*
				PS:在这里我要声明一句，*sptr = （shift + pLen - sptr） + p2 - p3;
				大家看被我用括号括起来的部分，如果只需要计算字符串移动的距离，那么括号中的那部分是不需要的。
				因为在字符串自左向右做匹配的时候，指标是一直向左移的，这里*sptr保存的内容，实际是指标要移动
				距离，而不是字符串移动的距离。我想SNORT是出于性能上的考虑，才这么做的。
			*/

			pptr--;			// 边界继续向前移动  
		}

		return shift;
	}

	bool String::operator==(char const* o) const
	{
		if (!o) return false;
		size_t oLen = strlen(o);
		if (dataLen != oLen) return false;
		return memcmp(buf, o, dataLen) == 0;
	}

	bool String::operator==(String const& o) const
	{
		if (this == &o) return true;
		if (dataLen != o.dataLen) return false;
		return memcmp(buf, o.buf, dataLen) == 0;
	}

	bool String::operator!=(String const& o) const
	{
		return !operator==(o);
	}

	bool String::operator<(String const& o) const
	{
		if (this == &o) return false;
		int r = memcmp(buf, o.buf, MIN(dataLen, o.dataLen));
		if (r == 0) return dataLen < o.dataLen;
		return r < 0;
	}

	bool String::operator>(String const& o) const
	{
		if (this == &o) return false;
		int r = memcmp(buf, o.buf, MIN(dataLen, o.dataLen));
		if (r == 0) return dataLen > o.dataLen;
		return r > 0;
	}

	bool String::operator<=(String const& o) const
	{
		return !operator>(o);
	}

	bool String::operator>=(String const& o) const
	{
		return !operator<(o);
	}

	size_t String::GetHashCode() const
	{
		return GetHashCode_CS((uint8 const*)buf, dataLen);
	}

	void String::DumpCore(char const* buf, size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			uint8 c = (uint8)buf[i];
			if (c < 32 || c > 126)
			{
				Append('.');
			}
			else Append((char)c);
		}
	}

	void String::Dump(char const* buf, size_t len)
	{
		if (len == 0)
		{
			return;
		}
		Append("\n----------------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F");
		size_t i = 0;
		for (; i < len; ++i)
		{
			if ((i % 16) == 0)
			{
				if (i)
				{           // output ascii to the end of the line
					Append("  ");
					DumpCore(buf + i - 16, 16);
				}
				Append('\n');
				AppendHex(i);
				Append("  ");
			}
			else if (i && (i % 4 == 0))
			{
				Append("  ");
			}
			else
			{
				Append(' ');
			}
			AppendHex((uint8)buf[i]);
		}
		size_t left = i % 16;
		if (left == 0)
		{
			left = 16;
		}
		if (left)
		{
			len = len + 16 - left;
			for (; i < len; ++i)
			{
				if (i && (i % 4 == 0))
				{
					Append("  ");
				}
				else
				{
					Append(' ');
				}
				Append("  ");
			}
			Append("  ");
			DumpCore(buf + i - 16, left);
		}
	}

	void String::WriteTo(ByteBuffer& bb) const
	{
		bb.VarWrite((uint)dataLen);
		if (!dataLen)
		{
			return;
		}
		bb.Write(buf, dataLen);
	}

	bool String::ReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb, 0, 0);
	}

	bool String::ReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		size_t len;
		if (!bb.VarRead(len))
		{
			return false;
		}
		if (bb.offset + len > bb.dataLen)
		{
			return false;
		}
		if (len < minLen || (maxLen > 0 && len > maxLen))
		{
			return false;
		}
		Assign(bb.buf + bb.offset, 0, len, false);
		bb.offset += len;
		return true;
	}

	void String::VarWriteTo(ByteBuffer& bb) const
	{
		WriteTo(bb);
	}

	bool String::VarReadFrom(ByteBuffer& bb)
	{
		return ReadFrom(bb);
	}

	bool String::VarReadFrom(ByteBuffer& bb, size_t minLen, size_t maxLen)
	{
		return ReadFrom(bb, minLen, maxLen);
	}

}
