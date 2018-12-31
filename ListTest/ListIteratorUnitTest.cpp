#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

namespace ListUnitTests
{
    //
    // Test List Iterator Operations (Reset, Advance, IsEnd, Value).
    // List iterators are acquireed by calling Find or First on a List.
    //

    TEST_CLASS(ListIteratorInterfaceUnitTests)
    {
    public:

            BEGIN_TEST_CLASS_ATTRIBUTE()
                TEST_CLASS_ATTRIBUTE(L"Collections", L"List")
                TEST_CLASS_ATTRIBUTE(L"Iterators", L"List")
            END_TEST_CLASS_ATTRIBUTE()

            //
            // Populate the list used by the iterator tests.
            //

            ListIteratorInterfaceUnitTests()
            {
                _l.Append("A");
                _l.Append("B");
                _l.Append("C");
                _l.Append("D");
                _l.Append("E");
            }

            //
            // Acquire an iterator using First on an empty collection.
            //
            // Result: a non-null iterator should be returned and IsEnd should be true.
            //

            TEST_METHOD(AcquireIteratorUsingFirstOnEmptyList)
            {
                List l;

                auto iterator = l.First();

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsTrue(iterator->IsEnd(), L"List iterator must not be at end.");
            }

            //
            // Acquire an iterator using the First method.
            //
            // Result: a non-null iterator should be returned and IsEnd should be false.
            //

            TEST_METHOD(AcquireIteratorUsingFirst)
            {
                auto iterator = _l.First();

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must be at end.");
            }

            //
            // Acquire an iterator using Find on each element.
            //
            // Result: a non-null iterator should be returned where IsEnd is false.
            //

            TEST_METHOD(AcquireIteratorUsingFind)
            {
                auto iterator = _l.Find("A");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

                iterator = _l.Find("B");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

                iterator = _l.Find("C");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

                iterator = _l.Find("D");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

                iterator = _l.Find("E");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");
            }

            //
            // Acquire an iterator on an element on in the list.
            //
            // Result: a non-null iterator should be returned where IsEnd is true.
            //

            TEST_METHOD(AcquireIteratorForNonexistantElement)
            {
                auto iterator = _l.Find("Z");

                Assert::IsNotNull(iterator.get(), L"List iterator must not be null.");
                Assert::IsTrue(iterator->IsEnd(), L"List iterator must be at end.");
            }

            //
            // Advance Count() times on a list.
            //
            // Result: Advance() should return true for Count() - 1 calls then
            // it should return False and IsEnd should be true.
            //

            TEST_METHOD(WalkIteratorUsingAdvance)
            {
                auto iterator = _l.First();
                WalkIteratorOverList(_l, iterator);
            }

            //
            // Reset the iterator after reaching the end and traverse through the
            // collection again.
            //
            // Result: reset should permit multiple traverals through the list.
            //

            TEST_METHOD(UseResetOnIterator)
            {
                auto iterator = _l.First();
                WalkIteratorOverList(_l, iterator);

                iterator->Reset();

                Assert::IsFalse(iterator->IsEnd(), L"List iterator after Reset must not be at end.");
                WalkIteratorOverList(_l, iterator);
            }

            //
            // Retrieve the value under an iterator.
            //
            // Result: the value method should return each element in the list.
            //

            TEST_METHOD(RetrieveValueUnderIterator)
            {
                auto iterator = _l.First();
                std::string const * value = nullptr;

                Assert::IsTrue(iterator->Value(&value), L"Could not retrieve Value from list iterator.");
                Assert::IsNotNull(value, L"Value returned must not be null.");
                Assert::AreEqual(*value, std::string("A"), L"Item returned does not match expected value.");

                Assert::IsTrue(iterator->Advance(), L"Advance of list iterator should not have failed.");
                Assert::IsTrue(iterator->Value(&value), L"Value returned must not be null.");
                Assert::IsNotNull(value, L"Value returned must not be null.");
                Assert::AreEqual(*value, std::string("B"), L"Item returned does not match expected value.");

                Assert::IsTrue(iterator->Advance(), L"Advance of list iterator should not have failed.");
                Assert::IsTrue(iterator->Value(&value), L"Value returned must not be null.");
                Assert::IsNotNull(value, L"Value returned must not be null.");
                Assert::AreEqual(*value, std::string("C"), L"Item returned does not match expected value.");

                Assert::IsTrue(iterator->Advance(), L"Advance of list iterator should not have failed.");
                Assert::IsTrue(iterator->Value(&value), L"Value returned must not be null.");
                Assert::IsNotNull(value, L"Value returned must not be null.");
                Assert::AreEqual(*value, std::string("D"), L"Item returned does not match expected value.");

                Assert::IsTrue(iterator->Advance(), L"Advance of list iterator should not have failed.");
                Assert::IsTrue(iterator->Value(&value), L"Value returned must not be null.");
                Assert::IsNotNull(value, L"Value returned must not be null.");
                Assert::AreEqual(*value, std::string("E"), L"Item returned does not match expected value.");
            }

    private:

        //
        // Acquire an iterator and walk through each element.
        //

        void WalkIteratorOverList(const List & l,
                std::unique_ptr<Collections::ValueIterator<std::list<std::string>>> & iterator) const
        {
            Assert::IsNotNull(iterator.get(), L"Iterator should not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator should not be at end.");

            for (size_t i = 1; i < l.Count(); ++i)
            {
                Assert::IsTrue(iterator->Advance(), L"Could not advance iterator.");
            }

            Assert::IsTrue(iterator->Advance(), L"Could not advance iterator.");
            Assert::IsTrue(iterator->IsEnd(), L"Expected IsEnd to return true.");
        }

        List _l;
    };
}