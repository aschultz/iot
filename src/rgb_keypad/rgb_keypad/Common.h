#pragma once

template< class Type, size_t n >
size_t countOf(Type(&)[n]) { return n; }

namespace std
{
#ifndef _XTR1COMMON_
    template< class T >
    struct remove_reference
    {
        typedef T type;
    };
    template< class T >
    struct remove_reference<T&>
    {
        typedef T type;
    };
    template< class T >
    struct remove_reference<T&&>
    {
        typedef T type;
    };

    template< typename T> inline
    constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(arg);
    };

	template< class T>
	constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept
	{
		return static_cast<T&&>(t);
	}

	template< class T>
	constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept
	{
		return static_cast<T&&>(t);
	}

    
    template< class T >
    void swap( T& a, T& b ) noexcept
    {
        T temp(std::move( a ));
        a = std::move( b );
        b = std::move( temp );
    };
	
	template< typename A, typename B>
	struct pair {
		typedef A first_type;
		typedef B second_type;

		A first;
		B second;

		constexpr pair() = default;

		pair(const first_type& a, const second_type& b)
			: first(a), second(b)
		{}

		pair& operator= (const pair& pr)
		{
			this->first = pr.first;
			this->second = pr.second;
		}

		pair& operator= (pair&& pr)
		{
			first = std::move(pr.first);
			second = std::move(pr.second);
			return *this;
		}

		void swap(pair& pr) noexcept
		{
			swap(first, pr.first);
			swap(second, pr.second);
		}
	};
#endif
}