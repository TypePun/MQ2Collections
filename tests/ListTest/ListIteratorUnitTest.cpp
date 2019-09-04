#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

//
// Global lock used to access the Member map. The Mutex is initialized
// inside MQ2 when it is running. In our unit tests, the initialization
// code is not run so initialize it before the unit test.
//

EQLIB_VAR HANDLE ghMemberMapLock;

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
        // Initialize the Global Mutex required by the MQ2 API.
        //

        TEST_CLASS_INITIALIZE(InitClassRequirements)
        {
            if (!ghMemberMapLock)
            {
                ghMemberMapLock = CreateMutex(NULL, FALSE, NULL);
                Assert::IsNotNull(ghMemberMapLock, L"Could not initialize global member mutex.");
            }
        }

        //
        // Close the Global Mutex required by the MQ2 API.
        //

        TEST_CLASS_CLEANUP(CleanupClassRequirements)
        {
            if (!ghMemberMapLock)
            {
                BOOL b = CloseHandle(ghMemberMapLock);
                Assert::IsTrue(b, L"Could not close global member mutex.");
            }
        }

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

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsTrue(iterator->IsEnd(), L"List iterator must not be at end.");
        }

        //
        // Acquire an iterator using First on an empty collection and Clone it.
        //
        // Result: a non-null iterator should be returned and IsEnd should be true
        // for both the initial iterator and the clone.
        //

        TEST_METHOD(CloneIteratorFromFirstOnEmptyList)
        {
            List l;

            auto iterator = l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();
            
            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsTrue(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsTrue(clone->IsEnd(), L"Cloned List iterator must not be at end.");
        }

        //
        // Acquire an iterator using the First method.
        //
        // Result: a non-null iterator should be returned and IsEnd should be false.
        //

        TEST_METHOD(AcquireIteratorUsingFirst)
        {
            auto iterator = _l.First();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");
        }

        //
        // Acquire an iterator using the First method and Clone it.
        //
        // Result: a non-null iterator should be returned and IsEnd should be false
        // for both the iterator and the clone.
        //

        TEST_METHOD(CloneIteratorFromFirst)
        {
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");
        }

        //
        // Acquire an iterator using Find on each element.
        //
        // Result: a non-null iterator should be returned where IsEnd is false.
        //

        TEST_METHOD(AcquireIteratorUsingFind)
        {
            auto iterator = _l.Find("A");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            iterator = _l.Find("B");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            iterator = _l.Find("C");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            iterator = _l.Find("D");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            iterator = _l.Find("E");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");
        }

        // Acquire an iterator using Find on each element and Clone them.
        //
        // Result: a non-null iterator should be returned where IsEnd is false
        // for each of the iterators and the clones.
        //

        TEST_METHOD(CloneIteratorUsingFind)
        {
            auto iterator = _l.Find("A");
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");

            iterator = _l.Find("B");
            clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");

            iterator = _l.Find("C");
            clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");

            iterator = _l.Find("D");
            clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");

            iterator = _l.Find("E");
            clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"List iterator must not be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator must not be at end.");
        }

        //
        // Acquire an iterator on an element not in the list.
        //
        // Result: a non-null iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(AcquireIteratorForNonexistantElement)
        {
            auto iterator = _l.Find("Z");

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsTrue(iterator->IsEnd(), L"List iterator must be at end.");
        }

        //
        // Acquire an iterator on an element not in the list.
        //
        // Result: a non-null iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(CloneIteratorForNonexistantElement)
        {
            auto iterator = _l.Find("Z");
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            Assert::IsNotNull(iterator, L"List iterator must not be null.");
            Assert::IsTrue(iterator->IsEnd(), L"List iterator must be at end.");

            Assert::IsNotNull(clone.get(), L"Cloned List iterator must not be null.");
            Assert::IsTrue(clone->IsEnd(), L"Cloned List iterator must be at end.");
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
        // Advance Count() times on a list.
        //
        // Result: Advance() should return true for Count() - 1 calls then
        // it should return False and IsEnd should be true.
        //

        TEST_METHOD(CloneWalkIteratorUsingAdvance)
        {
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            WalkIteratorOverList(_l, iterator);

            //
            // Now Walk over the clone.
            //

            WalkIteratorOverList(_l, clone.get());
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
        // Reset the iterator after reaching the end and traverse through the
        // collection again.
        //
        // Result: reset should permit multiple traverals through the list.
        //

        TEST_METHOD(CloneUseResetOnIterator)
        {
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            WalkIteratorOverList(_l, iterator);

            iterator->Reset();

            Assert::IsFalse(iterator->IsEnd(), L"List iterator after Reset must not be at end.");
            WalkIteratorOverList(_l, iterator);

            //
            // Do the same thing with the clone. The state of the clone should not be
            // affected by the state of the original iterator.
            //

            WalkIteratorOverList(_l, clone.get());

            clone->Reset();

            Assert::IsFalse(clone->IsEnd(), L"Cloned List iterator after Reset must not be at end.");
            WalkIteratorOverList(_l, clone.get());

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

        //
        // Retrieve the value under an iterator.
        //
        // Result: the value method should return each element in the list.
        //

        TEST_METHOD(CloneRetrieveValueUnderIterator)
        {
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();
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

            //
            // Do the same thing for the clone.
            //

            Assert::IsTrue(clone->Value(&value), L"Clone Could not retrieve Value from list iterator.");
            Assert::IsNotNull(value, L"Clone Value returned must not be null.");
            Assert::AreEqual(*value, std::string("A"), L"Clone Item returned does not match expected value.");

            Assert::IsTrue(clone->Advance(), L"Clone Advance of list iterator should not have failed.");
            Assert::IsTrue(clone->Value(&value), L"Clone Value returned must not be null.");
            Assert::IsNotNull(value, L"Clone Value returned must not be null.");
            Assert::AreEqual(*value, std::string("B"), L"Clone Item returned does not match expected value.");

            Assert::IsTrue(clone->Advance(), L"Clone Advance of list iterator should not have failed.");
            Assert::IsTrue(clone->Value(&value), L"Clone Value returned must not be null.");
            Assert::IsNotNull(value, L"Clone Value returned must not be null.");
            Assert::AreEqual(*value, std::string("C"), L"Clone Item returned does not match expected value.");

            Assert::IsTrue(clone->Advance(), L"Clone Advance of list iterator should not have failed.");
            Assert::IsTrue(clone->Value(&value), L"Clone Value returned must not be null.");
            Assert::IsNotNull(value, L"Clone Value returned must not be null.");
            Assert::AreEqual(*value, std::string("D"), L"Clone Item returned does not match expected value.");

            Assert::IsTrue(clone->Advance(), L"Clone Advance of list iterator should not have failed.");
            Assert::IsTrue(clone->Value(&value), L"Clone Value returned must not be null.");
            Assert::IsNotNull(value, L"Clone Value returned must not be null.");
            Assert::AreEqual(*value, std::string("E"), L"Clone Item returned does not match expected value.");
        }

    private:

        //
        // Acquire an iterator and walk through each element.
        //

        void WalkIteratorOverList(const List & l,
                Collections::ValueIterator<std::list<std::string>> * iterator) const
        {
            Assert::IsNotNull(iterator, L"Iterator should not be null.");
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