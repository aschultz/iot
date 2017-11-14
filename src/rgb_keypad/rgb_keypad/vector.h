#pragma once

#include "Common.h"

template <typename Data>
class array
{
private:
    size_t  m_size;
    Data   *m_data;
    bool    m_isConst;

public:
    constexpr array()
        : m_size(0), m_data(nullptr), m_isConst(false)
    {
    }
    constexpr array(size_t size)
        : m_size(size), m_data(new Data[m_size]), m_isConst(false)
    {
    }
    // Copy ctor
    array( const array<Data> &other )
        : m_size(other.m_size), m_data(new Data[m_size]), m_isConst(false)
    {
        memcpy(m_data, other.m_data, m_size);
    }
    // Move ctor
    array( array<Data> &&other )
        : m_size(std::move(other.m_size)), m_data(std::move(other.m_data)), m_isConst(std::move(other.m_isConst))
    {
        other.m_size = 0;
        other.m_data = nullptr;
    }
    array(const Data* data, size_t n)
        : m_size(n), m_data(data), m_isConst(true)
    {        
    }
    
    ~array()
    {
        if (m_data != NULL && !m_isConst)
        {
            delete[] m_data;
        }
    }

    constexpr size_t size() const { return m_size; }

    Data const& operator[](size_t index) const { return m_data[index];  }
    Data& operator[](size_t index) { return m_data[index]; }

    Data* begin() { return m_data; }
    Data* end() { return m_data + m_size; }
    Data const* cbegin() const { return m_data; }
    Data const* cend()   const { return m_data + m_size; }
    Data const* begin()  const { return cbegin(); }
    Data const* end()    const { return cend(); }

    // Move assignment
    array<Data>& operator=( array<Data> &&other ) noexcept
    {
        if (&other != this)
        {
            if (m_data != NULL && !m_isConst)
            {
                delete[] m_data;
            }
            m_size = std::move( other.m_size );
            m_data = std::move( other.m_data );
            m_isConst = std::move( other.m_isConst );
            other.m_size = 0;
            other.m_data = nullptr;
        }
        return *this;
    }
};

template<typename Data, size_t n>
array<Data>&& toArray( Data(&data)[n] ) { return std::move(array<Data>(data, n)); }

template<typename Data, size_t n>
array<const Data>&& toConstArray( Data( &data )[n] ) { return std::move( array<const Data>( data, n ) ); }



template <typename Data>
class vector
{
private:
    size_t m_size;
    size_t m_capacity;
    Data *m_data;

public:
    vector() : m_size(0), m_capacity(0), m_data(NULL) {}
    ~vector()
    {
        free(m_data);
    }

    // Copy ctor
    vector(vector const &other)
        : m_size(other.m_size), m_capacity(other.m_capacity), m_data(NULL)
    {
        m_data = malloc(m_capacity);
        memcpy(other.m_data, m_data, m_size);
    }

    constexpr size_t size() const { return m_size; }
    constexpr size_t capacity() const { return m_capacity; }

    const Data& operator[](size_t index) const {
        return m_data[index];
    }

    Data& operator[](size_t index) {
        return &m_data[index];
    }

    void resize(size_t newCapacity)
    {
        if (newCapacity < m_size)
        {
            m_size = newCapacity;
        }
        m_capacity = newCapacity;

        Data *newData = malloc(newCapacity);
        memcpy(newData, m_data, m_size);
        free(m_data);
        m_data = newData;
    }

    void push_back(Data &item)
    {
        if (m_size == m_capacity)
        {
            resize(m_capacity * 2);
        }
        m_data[m_size++] = item;        
    }       
};