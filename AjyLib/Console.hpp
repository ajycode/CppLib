#pragma once

namespace Ajy
{

	template<class... Parms>
	void Console::Write(Parms const&... parms)
	{
		String s;
		s.Append(parms...);
		//printf( "%s", s.C_str() );
		fwrite(s.C_str(), 1, s.Size(), stdout);
	}

	template<class... Parms>
	void Console::WriteLine(Parms const&... parms)
	{
		Write(parms..., '\n');
	}

	template<class... Parms>
	void Console::WriteFormat(char const* format, Parms const&... parms)
	{
		String s;
		s.AppendFormat(format, parms...);
		//printf( "%s", s.C_str() );
		fwrite(s.C_str(), 1, s.Size(), stdout);
	}

	template<class... Parms>
	void Console::WriteLineFormat(char const* format, Parms const&... parms)
	{
		WriteFormat(format, parms...);
		Write('\n');
	}

	template<class... Parms>
	void Console::PosWrite(short x, short y, Parms const&... parms)
	{
#ifdef Ajy_WINDOWS
		static auto h = GetStdHandle(STD_OUTPUT_HANDLE);
		static COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(h, pos);
#else
		//printf( "\033[%d;%dH", x + 1, y + 1 );
		Write("\033[", x + 1, ";", y + 1, "H");
#endif
		Write(parms...);
	}

	template<class T>
	Console const& Console::operator<<(T const& t) const
	{
		Write(t);
		return *this;
	}

	template<class... Parms>
	Console Console::operator()(Parms const&... parms) const
	{
		Write(parms...);
		return *this;
	}

}

