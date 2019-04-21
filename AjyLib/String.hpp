#pragma once

namespace Ajy
{

	template<size_t sLen>
	bool String::operator==(char const(&s)[sLen]) const
	{
		//调用示例
		/*char q[ 2 ] = { '1','\0' };
		Ajy::String s1 = Ajy::String::MakeString( 1 );
		bool b = ( s1.operator==<2>( q ) );*/
		if (sLen != dataLen) return false;
		return memcmp(buf, s, dataLen) == 0;
	}

	template<class... Args>
	void String::Append(Args const&... args)
	{
		size_t maxLen = GetFillMaxLength(args...) + 1;
		if (dataLen + maxLen > bufLen)
			Reserve(dataLen + maxLen);
		dataLen += Fill(buf + dataLen, args...);
	}

	template<class... Args>
	String String::MakeString(Args const&... args)
	{
		String rtv;
		rtv.Append(args...);
		return rtv;
	}

	template<class... Args, size_t _bufLen>
	String String::MakeString(char(&_buf)[_bufLen], Args const&... args)
	{
		//调用示例
		/*char q[ 2 ] = { '1','\0' };
		Ajy::String s1 = Ajy::String::MakeString( q, 1 );*/
		String rtv(_buf, false);
		rtv.Append(args...);
		return rtv;
	}

	template<class T>
	String& String::operator<<(T const& v)
	{
		Append(v);
		return *this;
	}

	template<class T>
	String String::ToString(T const& v)
	{
		return MakeString(v);
	}

	template<class T>
	String String::ToHexString(T const& v)
	{
		String s;
		s.AppendHex(v);
		return s;
	}

	template<class T>
	void String::GetFillMaxLengthCore(size_t& len, T const& v)
	{
		len += Ajy::GetToStringMaxLength(v);
	}

	template<class T, class... Args>
	void String::GetFillMaxLengthCore(size_t& len, T const& v, Args const&... args)
	{
		GetFillMaxLengthCore(len, v);
		GetFillMaxLengthCore(len, args...);
	}

	template<class... Args>
	size_t String::GetFillMaxLength(Args const&... args)
	{
		size_t len = 0;
		GetFillMaxLengthCore(len, args...);
		return len;
	}

	template<class T>
	void String::FillCore(char*& _buf, size_t& offset, T const& v)
	{
		offset += Ajy::ToString(_buf + offset, v);
	}

	template<class T, class... Args>
	void String::FillCore(char*& _buf, size_t& offset, T const& v, Args const&... args)
	{
		FillCore(_buf, offset, v);
		FillCore(_buf, offset, args...);
	}

	template<class T>
	void String::FillHexCore(char * & _buf, size_t& offset, T const& v)
	{
		offset += Ajy::ToHexString(_buf + offset, v);
	}

	template<class T, class... Args>
	void String::FillHexCore(char * & _buf, size_t& offset, T const& v, Args const&... args)
	{
		FillHexCore(_buf, offset, v);
		FillHexCore(_buf, offset, args...);
	}

	template<class... Args>
	size_t String::Fill(char* _buf, Args const&... args)
	{
		size_t offset = 0;
		FillCore(_buf, offset, args...);
		_buf[offset] = '\0';
		return offset;
	}

	template<class... Args>
	size_t String::FillHex(char * _buf, Args const&... args)
	{
		size_t offset = 0;
		FillHexCore(_buf, offset, args...);
		return offset;
	}

	template<class T>
	void String::AppendFormatCore(String& s, size_t& n, T const& v)
	{
		if (!n) 
			s.Append(v);
	}

	template<class T, class... Args>
	void String::AppendFormatCore(String& s, size_t& n, T const& v, Args const&... args)
	{
		if (n-- == 0)
		{
			s.Append(v);
			return;
		}
		AppendFormatCore(s, n, args...);
	}

	template<class... Args>
	void String::AppendFormat(char const* format, Args const&... args)
	{
		StringFormatHelper helpers[sizeof...(args)];
		memset(helpers, 0, sizeof(StringFormatHelper)*sizeof...(args));

		Ajy_ALIGN8(char numBuf[32]);
		String numStr(numBuf, 32, 0, true);

		size_t offset = 0, i = 0, n = 0;
		while (char c = format[offset])
		{
			if (c == '{')
			{
				c = format[++offset];
				if (c == '{')
				{
					Push('{');
				}
				else
				{
					while ((c = format[offset]))
					{
						if (c == '}')
						{
							numStr.C_str();				// for terminate that FromString is need it
							Ajy::FromString(i, numBuf);
							numStr.Clear();
							if (i < 0 || i >= sizeof...(args))
							{
								throw std::invalid_argument("argument out of range.");
							}
							if (helpers[i].len)
							{
								if (dataLen + helpers[i].len > bufLen)
								{
									Reserve(dataLen + helpers[i].len);
								}
								memcpy(buf + dataLen, buf + helpers[i].idx, helpers[i].len);
								dataLen += helpers[i].len;
								buf[dataLen] = '\0';
							}
							else
							{
								helpers[i].idx = dataLen;
								n = i;
								AppendFormatCore(*this, n, args...);
								helpers[i].len = dataLen - helpers[i].idx;
							}
							break;
						}
						else
						{
							numStr.Push(c);
						}
						++offset;
					}
				}
			}
			else
			{
				Push(c);
			}
			++offset;
		}
	}

	template<class T>
	void String::AppendHex(T const& v)
	{
		Reserve(dataLen + 17);
		dataLen += Ajy::ToHexString(buf + dataLen, v);
		buf[dataLen] = '\0';
	}

	template<class... Args>
	String String::MakeFormatString(char const* format, Args const&... args)
	{
		String rtv;
		rtv.AppendFormat(format, args...);
		return rtv;
	}

}
