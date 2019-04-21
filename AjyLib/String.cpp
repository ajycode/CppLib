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
		//  �õ�ģʽ����next����   
		GetNextArrayOfKMP(actualMatch, matchLen, next);
		for (int i = 0, j = 0; i < dataLen; )
		{  
			if (-1 == j ||  buf[i] == actualMatch[j])  // ���ģʽ���α��Ѿ����˵���һ���ַ�����ƥ��ɹ�    
			{
				// ���������������ʱ�����α궼Ҫ��ǰ��һ��   
				++i;
				++j;
				if ( j == matchLen )
				{
					j = 0;
					beginIdxResults.Push(i - matchLen);
				}
			}
			else                          // ƥ�䲻�ɹ�,ģʽ���α���˵���ǰ�ַ���nextֵ   
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
		while (b_idx <= dataLen)		// �����ַ����Ƿ�ƥ�䵽�˾�ͷ  
		{
			int p_idx = matchLen, skip_stride, shift_stride;
			while (buf[--b_idx] == actualMatch[--p_idx])		// ��ʼƥ��  
			{
				if (b_idx < 0)
					return std::make_pair(matchLen, beginIdxResults);
				if (p_idx == 0)
				{
					beginIdxResults.Push(b_idx);
					b_idx += (matchLen+1);
				}
			}
			skip_stride = skip[(unsigned char)buf[b_idx]];		// ���ݻ��ַ����������Ծ�ľ���
			shift_stride = shift[p_idx];						// ���ݺú�׺���������Ծ�ľ���  
			b_idx += (skip_stride > shift_stride) ? skip_stride : shift_stride;		// ȡ����  
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
			next[actualMatch[j]] = matchLen - j;			// ��¼�ַ������Ҷε���̾���+1 
		}
		int pos = 0;
		while (pos < (dataLen - matchLen + 1))		// ĩ�˶���  
		{
			int i = pos;
			int j;
			for (j = 0; j < matchLen; ++j, ++i)
			{
				if (buf[i] != actualMatch[j])
				{
					pos += next[buf[pos + matchLen]];
					// �����ھ���Ծ,��Ծ�Ǻ���  
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
		//  ��һ���ַ���nextֵ��-1,��ΪC�е������Ǵ�0��ʼ��   
		next[0] = -1;
		for (int i = 0, j = -1; i < len - 1; )
		{  
			if (-1 == j || pattern[i] == pattern[j])     // ���ģʽ���α��Ѿ����˵���һ���ַ�����ƥ��ɹ�   
			{
				// �����α궼��ǰ��һ��   
				++i;
				++j;
				// ��ŵ�ǰ��nextֵΪ��ʱģʽ�����α�ֵ   
				next[i] = j;
			}
			else                                  // ƥ�䲻�ɹ�j�ͻ��˵���һ��nextֵ   
			{
				j = next[j];
			}
		}
	}

	/*
	Ŀ�ģ����ݻ��ַ�������Ԥ��������һ�Ż��ַ���
	������
	pattern => ģʽ��P
	len => ģʽ��P����
	���أ�
	int* => ���ַ���
	*/
	int* String::MakeSkipOfBM(char const* pattern, size_t len)
	{
		int i;
		//Ϊ�������ַ�������256��int�Ŀռ�  
		/*PS:֮����Ҫ����256��������Ϊһ���ַ���8λ��
		�����ַ�������2��8�η���256�ֲ�ͬ���*/
		int *skip = new int[256];

		//��ʼ�����ַ���256����Ԫȫ����ʼ��ΪpLen��û����ģʽ�����ֵ��ַ�����ΪpLen��  
		for (i = 0; i < 256; i++)
		{
			*(skip + i) = len;
		}

		//��������Ҫ��ֵ�ĵ�Ԫ��ֵ������ģʽ���г��ֵ��ַ��Ͳ����ٸ�ֵ��  
		while (len != 0)
		{
			*(skip + (unsigned char)*pattern++) = len--;
		}

		return skip;
	}

	/*
	Ŀ�ģ����ݺú�׺������Ԥ��������һ�źú�׺��
	������
	pattern => ģʽ��P
	len => ģʽ��P����
	���أ�
	int* => �ú�׺��
	*/
	int* String::MakeShiftOfBM(char const* pattern, size_t len)
	{
		int *shift = new int[len];				// Ϊ�ú�׺������pLen��int�Ŀռ� 
		int *sptr = shift + len - 1;			// ������ú�׺����и�ֵ��ָ��  
		char const* pptr = pattern + len - 1;	// ��¼�ú�׺��߽�λ�õ�ָ��  
		char c = *(pattern + len - 1);			// ����ģʽ�������һ���ַ�����ΪҪ�����õ��� 
		*sptr = 1;								// �����һ���ַ�Ϊ�߽�ʱ��ȷ���ƶ�1�ľ���  
		pptr--;		// �߽��ƶ��������ڶ����ַ�����������Լ�����ȥ�ģ���Ϊ���ܾ��ò�����ȥ����BUG��������ԡ�abcdd�����������ĩβ��λ�ظ��������  

		while (sptr-- != shift)		// �������ѭ����ɸ��ú�׺����ÿһ����Ԫ���и�ֵ�Ĺ���  
		{
			char const* p1 = pattern + len - 2, *p2, *p3;
			// ��do...whileѭ������Ե�ǰpptr��ָ���ַ�Ϊ�߽�ʱ��Ҫ�ƶ��ľ���  
			do 
			{
				while (p1 >= pattern && *p1-- != c);	// �ÿ�ѭ����Ѱ�������һ���ַ�cƥ����ַ���ָ���λ��  
				p2 = pattern + len - 2;
				p3 = p1;
				while (p3 >= pattern && *p3-- == *p2-- && p2 >= pptr);		// �ÿ�ѭ�����ж��ڱ߽����ַ�ƥ�䵽��ʲôλ��  
			} while (p3 >= pattern && p2 >= pptr);
			*sptr = shift + len - sptr + p2 - p3;		// ����ú�׺���У���pptr�����ַ�Ϊ�߽�ʱ��Ҫ�ƶ���λ��  
			
			/*
				PS:��������Ҫ����һ�䣬*sptr = ��shift + pLen - sptr�� + p2 - p3;
				��ҿ������������������Ĳ��֣����ֻ��Ҫ�����ַ����ƶ��ľ��룬��ô�����е��ǲ����ǲ���Ҫ�ġ�
				��Ϊ���ַ�������������ƥ���ʱ��ָ����һֱ�����Ƶģ�����*sptr��������ݣ�ʵ����ָ��Ҫ�ƶ�
				���룬�������ַ����ƶ��ľ��롣����SNORT�ǳ��������ϵĿ��ǣ�����ô���ġ�
			*/

			pptr--;			// �߽������ǰ�ƶ�  
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
