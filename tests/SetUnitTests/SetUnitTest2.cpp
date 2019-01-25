#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "Set.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

//
// Global lock used to access the Member map. The Mutex is initialized
// inside MQ2 when it is running. In our unit tests, the initialization
// code is not run so initialize it before the unit test.
//

EQLIB_VAR HANDLE ghMemberMapLock;

namespace SetUnitTests
{
    //
    // Test Set Iterator operations (Reset, Advance, IsEnd and Value).
    // Set iterators are acquired by calling Find or First on a Set.
    //

    TEST_CLASS(SetIteratorUnitTest)
    {
    public:

        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Set")
            TEST_CLASS_ATTRIBUTE(L"Iterators", L"Set")
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
        // Populate the set used by the iterator tests.
        //

        SetIteratorUnitTest()
        {
            _s.Add("A");
            _s.Add("B");
            _s.Add("C");
        }

        //
        // Test that we can acquire an iterator on the set.
        //
        // Result: A non-null iterator should be returned.
        //

        TEST_METHOD(TestIteratorFromFirst)
        {
            auto iterator = _s.First();

            Assert::IsNotNull(iterator.get());
        }

        //
        // Test that we can acquire a new iterator on each of the elements
        // by using Find.
        //
        // Result: Non-null iterators should be returned where IsEnd is false.
        //

        TEST_METHOD(TestIteratorFromFind)
        {
            auto iterator = _s.Find("A");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());

            iterator = _s.Find("B");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());

            iterator = _s.Find("C");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());
        }

        //
        // Test Find on an element not in the set.
        //
        // Result: An iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(TestIteratorForNonexistantElement)
        {
            auto iterator = _s.Find("D");

            Assert::IsNotNull(iterator);
            Assert::IsTrue(iterator->IsEnd());
        }

        //
        // Test that returning a new iterator is not at the end.
        //
        // Result: IsEnd should be false.
        //

        TEST_METHOD(TestIteratorIsNotAtEnd)
        {
            auto iterator = _s.First();

            Assert::IsNotNull(iterator.get());
            Assert::IsFalse(iterator->IsEnd());
        }

        //
        // Test that advancing Count() times reaches the end.
        //
        // Result: Advance() should return true for Count() - 1 calls then
        // it should return False and IsEnd should be true.
        //

        TEST_METHOD(TestIteratorAdvance)
        {
            auto iterator = _s.First();
            WalkIteratorOverSet(_s, iterator);
        }

        //
        // Test the Reseting the iterator after reaching the end lets us
        // traverse through the collection again.
        //
        // Result: Reset should permit multiple traverals through the set.
        //

        TEST_METHOD(TestIteratorReset)
        {
            auto iterator = _s.First();
            WalkIteratorOverSet(_s, iterator);

            iterator->Reset();

            Assert::IsFalse(iterator->IsEnd());
            WalkIteratorOverSet(_s, iterator);
        }

        //
        // Test the Value method on an iterator.
        //
        // Result: The value method should return each element in the set.
        //

        TEST_METHOD(TestValueUnderIterator)
        {
            auto iterator = _s.First();
            std::string const * value = nullptr;

            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("A"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("B"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("C"));
        }

    private:

        //
        // Acquire an iterator and walk through each element.
        //

        void WalkIteratorOverSet(const Set & s,
            std::unique_ptr<Collections::ValueIterator<std::set<std::string>>> & iterator
        ) const
        {
            Assert::IsNotNull(iterator.get());
            Assert::IsFalse(iterator->IsEnd());

            for (size_t i = 1; i < s.Count(); ++i)
            {
                Assert::IsTrue(iterator->Advance());
            }

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->IsEnd());
        }

        Set _s;
    };
}