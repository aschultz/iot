#pragma once

#include "Common.h"
#include "vector.h"

template<typename T> class delegate;

template<typename RET, typename ...PARAMS>
class delegate<RET (PARAMS...)>
{
private:
#pragma region Stubs
	using stub_ptr_type = RET (*)(void*, PARAMS&&...);

	// Stub function for executing a static function
	template<RET (*functionPtr)(PARAMS...)>
	static RET function_stub(void* const, PARAMS&&... args)
	{
		return functionPtr(std::forward<PARAMS>(args)...);
	}

	// Stub function for executing a non-static member function
	template<class C, RET(C::*methodPtr)(PARAMS...)>
	static RET member_func_stub(void* const instance, PARAMS&&... args)
	{
		return (static_cast<C*>(instance)->*methodPtr)(std::forward<PARAMS>(args)...);
	}

	template<class C, RET(C::*methodPtr)(PARAMS...) const>
	static RET const_method_stub(void* const instance, PARAMS&&... args)
	{
		return (static_cast<C const*>(instance)->*methodPtr)(std::forward<PARAMS>(args)...);
	}

	template<typename T>
	static RET lambda_stub(void* const instance, PARAMS&&... args)
	{
		return (*static_cast<T*>(instance))(std::forward<PARAMS>(args)...);
	}
#pragma endregion Stubs

	void *m_instance;
	stub_ptr_type m_stub;

	delegate(void* const o, stub_ptr_type const m) noexcept :
		m_instance(o),
		m_stub(m)
	{
	}

public:
#pragma region Constructors
	delegate() = default;
	delegate(delegate const&) = default;
	delegate(delegate&&) = default;
	delegate(nullptr_t const) noexcept : delegate() { }
#pragma endregion Constructors

#pragma region Create
	// For a global or static function
	template<RET (*const FUNC)(PARAMS...)>
	static delegate create() noexcept
	{
		return { nullptr, &function_stub<FUNC> };
	}

	// For an instance method
	template<class C, RET (C::*METHOD)(PARAMS...)>
	static delegate create(C* instance) noexcept
	{
		return { instance, &member_func_stub<C, METHOD> };
	}

	// For a const instance method 
	template<class C, RET(C::*METHOD)(PARAMS...) const>
	static delegate create(C* instance) noexcept
	{
		return { instance, &const_method_stub<C, METHOD> };
	}

	// For a lambda method
	template<typename T>
	static delegate create(T* instance) noexcept
	{
		return { instance, &lambda_stub<T> };
	}

#pragma endregion Create

#pragma region Operators
	RET operator()(PARAMS... args) const
	{
		return m_stub(m_instance, std::forward<PARAMS>(args)...);
	}

	delegate& operator=(delegate rhs) noexcept
	{
		swap(*this, rhs);
		return *this;
	}

	bool operator==(nullptr_t) const noexcept
	{
		return m_stub == nullptr;
	}
	bool operator==(delegate const& rhs) const noexcept
	{
		return (m_instance == rhs.m_instance) && (m_stub == rhs.m_stub);
	}
	bool operator!=(nullptr_t) const noexcept
	{
		return m_stub != nullptr;
	}
	bool operator!=(delegate const& rhs) const noexcept
	{
		return (m_instance != rhs.m_instance) || (m_stub != rhs.m_stub);
	}
	explicit operator bool() const noexcept
	{
		return m_stub != nullptr; 
	}
#pragma endregion Operators

	template <class C>
	using member_pair = std::pair<C* const, RET(C::* const)(PARAMS...)>;

	friend void swap(delegate& first, delegate& second) noexcept
	{
		using namespace std;
		swap(first.m_stub, second.m_stub);
		swap(first.m_instance, second.m_instance);
	}
};

//class Delegate
//{
//public:
//	//template<class C, typename RET, typename ...PARAMS>
//	//static delegate<RET(PARAMS...)> create(C* const instance, RET (C::* const methodPtr)(PARAMS...))
//	//{
//	//	return delegate<RET (PARAMS...)>::template create(instance, methodPtr);
//	//}
//
//	template<class C, typename RET, typename ...PARAMS>
//	static delegate<RET (PARAMS...)> create(C* instance, RET(C::*METHOD)(PARAMS...)) noexcept
//	{
//		typedef RET(C::*methodPtr)(PARAMS...);
//
//		return delegate<RET (PARAMS...)>::template create<C, methodPtr>(instance);
//	}
//};