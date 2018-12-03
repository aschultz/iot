#pragma once

#include "Common.h"

//template<typename Data, size_t n>
//array<Data>&& toArray( Data(&data)[n] ) { return std::move(array<Data>(data, n)); }
//
//template<typename Data, size_t n>
//array<const Data>&& toConstArray( Data( &data )[n] ) { return std::move( array<const Data>( data, n ) ); }

template<typename T, size_t N>
class array
{
public:
	typedef T		  value_type;
	typedef T *		  pointer;
	typedef const T * const_pointer;
	typedef T &		  reference;
	typedef const T & const_reference;
	typedef T *		  iterator;
	typedef const T * const_iterator;
	typedef size_t	  size_type;
	
	// Iterators
	constexpr iterator begin() const { return m_data; }
	constexpr iterator end() const { return m_data + N; }
	constexpr const_iterator cbegin() const { return m_data; }
	constexpr const_iterator cend() const { return m_data + N; }

	// Capacity
	constexpr size_type size() { return N; }
	constexpr bool empty() const { return N == 0; }
	
	// Element access
	constexpr T& operator[](size_type index) { return m_data[index]; }
	//constexpr const T& operator[](size_t index) const { return m_data[index]; }
	constexpr T * data() { return m_data; }
	//constexpr const T * data() const { return m_data; }

private:
	T m_data[N];
};

template<typename T>
class vector
{
public:
	typedef T		  value_type;
	typedef T *		  pointer;
	typedef const T * const_pointer;
	typedef T &		  reference;
	typedef const T & const_reference;
	typedef T *		  iterator;
	typedef const T * const_iterator;
	typedef size_t	  size_type;

	// Constructors
	vector();
	vector(size_type size);
	vector(size_type size, const T& value);
	~vector();

	// Copy / Move
	vector(const vector& other);
	vector(vector&& other);
	vector& operator=(vector other);

	// Iterators
	constexpr iterator begin() const { return m_data; }
	constexpr iterator end() const { return m_data + m_size; }
	constexpr const_iterator cbegin() const { return m_data; }
	constexpr const_iterator cend() const { return m_data + m_size; }

	// Capacity
	constexpr size_type size() const { return m_size; }
	constexpr size_type capacity() const { return m_capacity; }
	constexpr bool empty() const { return m_size == 0; }

	// Element access
	constexpr T& operator[](size_type index) { return m_data[index]; }
	//constexpr const T& operator[](size_t index) const { return m_data[index]; }
	constexpr T * data() { return m_data; }
	//constexpr const T * data() const { return m_data; }

	// Modifiers
	void push_back(const T& value);
	void pop_back();
	void insert(const_iterator position, const T& val);

	friend void swap(vector& first, vector& second)
	{
		using std::swap;
		swap(first.m_size, second.m_size);
		swap(first.m_capacity, second.m_capacity);
		swap(first.m_data, second.m_data);
	}

private:
	size_type m_size;
	size_type m_capacity;
	T * m_data;
};

template<typename T>
vector<T>::vector()
	: m_size(0), m_capacity(0), m_data(nullptr)
{
}

template<typename T>
vector<T>::vector(size_type size)
	: m_size(size), m_capacity(size), m_data(m_size ? new T[size]() : nullptr)
{
}

template<typename T>
vector<T>::vector(size_type size, const T& value)
	: m_size(size), m_capacity(size), m_data(m_size ? new T[m_size]() : nullptr)
{
	for (size_type i = 0; i < size; ++i)
	{
		m_data[i] = value;
	}
}

template<typename T>
vector<T>::~vector()
{
	delete[] m_data;
}

template<typename T>
vector<T>::vector(const vector<T> &v)
	: m_size(v.size()), m_capacity(v.size()), m_data(m_size ? new T[m_size] : nullptr)
{
	for (size_type i = 0; i < m_size; ++i)
	{
		m_data[i] = v[i];
	}
}

template<typename T>
vector<T>::vector(vector<T> &&other)
	: vector()
{
	swap(*this, other);
}

template<typename T>
vector<T>& vector<T>::operator=(vector<T> other)
{
	swap(*this, other);
	return *this;
}


template<typename T>
void vector<T>::push_back(const T& value)
{
}

template<typename T>
void vector<T>::pop_back()
{
}

template<typename T>
void vector<T>::insert(const_iterator position, const T& val)
{
}

//
//template <typename Data>
//class vector
//{
//private:
//    size_t m_size;
//    size_t m_capacity;
//    Data *m_data;
//
//public:
//    vector() : m_size(0), m_capacity(0), m_data(NULL) {}
//    ~vector()
//    {
//        free(m_data);
//    }
//
//    // Copy ctor
//    vector(vector const &other)
//        : m_size(other.m_size), m_capacity(other.m_capacity), m_data(NULL)
//    {
//        m_data = malloc(m_capacity);
//        memcpy(other.m_data, m_data, m_size);
//    }
//
//    constexpr size_t size() const { return m_size; }
//    constexpr size_t capacity() const { return m_capacity; }
//
//    const Data& operator[](size_t index) const {
//        return m_data[index];
//    }
//
//    Data& operator[](size_t index) {
//        return &m_data[index];
//    }
//
//    void resize(size_t newCapacity)
//    {
//        if (newCapacity < m_size)
//        {
//            m_size = newCapacity;
//        }
//        m_capacity = newCapacity;
//
//        Data *newData = malloc(newCapacity);
//        memcpy(newData, m_data, m_size);
//        free(m_data);
//        m_data = newData;
//    }
//
//    void push_back(Data &item)
//    {
//        if (m_size == m_capacity)
//        {
//            resize(m_capacity * 2);
//        }
//        m_data[m_size++] = item;        
//    }       
//};


template<typename T>
class array_ref
{
public:
	typedef T value_type;
	typedef T *		  pointer;
	typedef const T * const_pointer;
	typedef const T & reference;
	typedef const T & const_reference;
	typedef T *		  iterator;
	typedef const T * const_iterator;
	typedef size_t size_type;

	// Constructors
	constexpr array_ref() = default;
	array_ref(const vector<T> &v);
	template<size_t N> constexpr array_ref(const T(&a)[N]);
	template<size_t N> constexpr array_ref(const array<T, N> &a);

	// Copy / Move
	array_ref(const array_ref &) = default;
	array_ref(array_ref &&) = default;
	array_ref & operator=(array_ref&) = default;

	// Iterators
	constexpr iterator begin() const { return m_data; }
	constexpr iterator end() const { return m_data + m_size; }
	constexpr const_iterator cbegin() const { return m_data; }
	constexpr const_iterator cend() const { return m_data + m_size; }

	// Size
	constexpr size_type size() const { return m_size; }
	constexpr bool empty() const { return m_size == 0; };

	// Indexing
	constexpr reference operator[](size_type i) { return m_data[i]; }
	//constexpr const_reference operator[](size_type i) const { return m_data[i]; }
	constexpr pointer * data() const { return m_data; }

	friend void swap(array_ref& first, array_ref& second)
	{
		using std::swap;
		swap(first.m_size, second.m_size);
		swap(first.m_data, second.m_data);
	}

private:
	size_type m_size;
	pointer m_data;
};


template<typename T>
array_ref<T>::array_ref(const vector<T> &v)
	: m_size(v.size()), m_data(v.data())
{
}

template<typename T>
template<size_t N>
constexpr array_ref<T>::array_ref(const T(&a)[N])
	: m_size(N), m_data(a)
{
}

template<typename T>
template<size_t N>
constexpr array_ref<T>::array_ref(const array<T, N> &a)
	: m_size(a.size()), m_data(a.data())
{
}
