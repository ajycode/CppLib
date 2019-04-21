#pragma once

namespace Ajy
{

	template<class T1, class T2>
	struct Pair
	{
		//more varible in here...
		T1 first;
		T2 second;

		//more function in here...
		Pair() :first(), second() {}
		Pair(T1 const& f, T2 const& s) :first(f), second(s) {}
		Pair(Pair const&) = default;
		Pair(Pair&&) = default;
		Pair& operator=(Pair const& copy)
		{
			if (this != &copy)
			{
				first = copy.first;
				second = copy.second;
			}
			return *this;
		}
		Pair& operator=(Pair&& copy)
		{
			first = std::forward<T1>(copy.first);
			second = std::forward<T2>(copy.second);
			return *this;
		}
		~Pair() {}
		/*bool operator==( Pair const& _pair ) const
		{
			return first == _pair.first && second == _pair.second;
		}
		bool operator!=( Pair const& _pair ) const
		{
			return !( *this == _pair );
		}
		bool operator<( Pair const& _pair ) const
		{
			return first < _pair.first || ( first == _pair.first && second < _pair.second );
		}
		bool operator>( Pair const& _pair ) const
		{
			return first > _pair.first || ( first == _pair.first && second > _pair.second );
		}
		bool operator<=( Pair const& _pair ) const
		{
			return !( *this > _pair );
		}
		bool operator>=( Pair const& _pair ) const
		{
			return !( *this < _pair );
		}*/
		static Pair MakePair(T1 f, T2  s)
		{
			return Pair(f, s);
		}
	};

	template<class T1, class T2>
	inline bool operator==(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (p1.first == p2.first && p1.second == p2.second);
	}

	template<class T1, class T2>
	inline bool operator!=(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (!(p1 == p2));
	}

	template<class T1, class T2>
	inline bool operator<(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second));
	}

	template<class T1, class T2>
	inline bool operator>(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (p2 < p1);
	}

	template<class T1, class T2>
	inline bool operator<=(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (!(p2 < p1));
	}

	template<class T1, class T2>
	inline bool operator>=(Pair<T1, T2> const& p1, Pair<T1, T2> const& p2)
	{
		return (!(p1 < p2));
	}

}
