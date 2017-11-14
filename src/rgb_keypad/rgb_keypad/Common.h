#pragma once

template< class Type, size_t n >
size_t countOf(Type(&)[n]) { return n; }

namespace std
{
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
        typename remove_reference<T>::type&& move(T&& arg)
    {
        return static_cast<typename remove_reference<T>::type&&>(arg);
    };
    
    template< class T >
    void swap( T& a, T& b ) noexcept
    {
        T& temp = std::move( a );
        a = std::move( b );
        b = std::move( temp );
    };

    /*
    template< class T >
    struct shared_ptr
    {

    };
    */
}