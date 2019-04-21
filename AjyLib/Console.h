#pragma once

namespace Ajy
{

	const class Console final
	{
	public:
		Console() {}

		template<class... Parms>
		static void Write(Parms const&... parms);

		template<class... Parms>
		static void WriteLine(Parms const&... parms);

		template<class... Parms>
		static void WriteFormat(char const* format, Parms const&... parms);

		template<class... Parms>
		static void WriteLineFormat(char const* format, Parms const&... parms);

		template<class... Parms>
		static void PosWrite(short x, short y, Parms const&... parms);

		template<class T>
		Console const& operator<<(T const& t) const;

		template<class... Parms>
		Console operator()(Parms const&... parms) const;

	}  Cout;

}

