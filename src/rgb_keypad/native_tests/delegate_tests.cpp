#include "stdafx.h"
#include "CppUnitTest.h"
#include "delegate.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace native_tests
{
	int gValue;

	void GlobalOnEventCalled(int value)
	{
		gValue = value;
	}

	TEST_CLASS(DelegateTests)
	{
	private:
		int mValue;

	public:
		//https://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible

		TEST_METHOD(TestFuncPtr)
		{
			typedef void(*GlobalFuncPtr)(int);

			// Pointer to funcion
			void(*funcPtr)(int) = &GlobalOnEventCalled;
			GlobalFuncPtr funcPtr2 = &GlobalOnEventCalled;
			delegate<void(int)> d = delegate<void(int)>::create<&GlobalOnEventCalled>();

			(*funcPtr)(1);
			Assert::IsTrue(1 == gValue, L"Value was not set");

			(*funcPtr2)(2);
			Assert::IsTrue(2 == gValue, L"Value was not set");

			d(3);
			Assert::IsTrue(3 == gValue, L"Value was not set");
		}

		TEST_METHOD(TestMemberFunc)
		{
			typedef void(DelegateTests::*MemberFuncPtr)(int);

			// Pointer to member function
			void(DelegateTests::*funcPtr)(int) = &DelegateTests::OnEventCalled;
			MemberFuncPtr funcPtr2 = &DelegateTests::OnEventCalled;
			delegate<void(int)> d = delegate<void(int)>::create<DelegateTests, &DelegateTests::OnEventCalled>(this);

			(this->*funcPtr)(1);
			Assert::IsTrue(1 == mValue, L"Value was not set");

			(this->*funcPtr2)(2);
			Assert::IsTrue(2 == mValue, L"Value was not set");

			d(3);
			Assert::IsTrue(3 == mValue, L"Value was not set");
		}

		TEST_METHOD(TestConstMemberFunc)
		{
			typedef void(DelegateTests::*MemberFuncPtr)(int) const;

			// Pointer to const member function
			void(DelegateTests::*funcPtr)(int) const = &DelegateTests::OnEventCalledConst;
			MemberFuncPtr funcPtr2 = &DelegateTests::OnEventCalledConst;
			delegate<void(int)> d = delegate<void(int)>::create<DelegateTests, &DelegateTests::OnEventCalledConst>(this);

			(this->*funcPtr)(1);
			//Assert::IsTrue(1 == mValue, L"Value was not set");

			(this->*funcPtr2)(2);
			//Assert::IsTrue(2 == mValue, L"Value was not set");

			d(3);
			//Assert::IsTrue(3 == mValue, L"Value was not set");
		}

		TEST_METHOD(TestLambdaFunc)
		{
			auto foo = [this](int a)
			{
				mValue = a;
			};

			delegate<void(int)> d = delegate<void(int)>::create(&foo);

			foo(1);
			Assert::IsTrue(1 == mValue, L"Value was not set");

			d(2);
			Assert::IsTrue(2 == mValue, L"Value was not set");
		}

		void OnEventCalled(int value)
		{
			mValue = value;
		}

		void OnEventCalledConst(int value) const
		{
		}
	};
}