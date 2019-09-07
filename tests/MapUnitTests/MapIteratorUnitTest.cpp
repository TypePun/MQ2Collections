#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "Map.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

//
// Global lock used to access the Member map. The Mutex is initialized
// inside MQ2 when it is running. In our unit tests, the initialization
// code is not run so initialize it before the unit test.
//

EQLIB_VAR HANDLE ghMemberMapLock;

namespace MapUnitTests
{
    //
    // Test Map Iterator operations (Reset, Advance, IsEnd, Value and Key).
    // Map iterators are acquired by calling Find or First on a Set.
    //

    TEST_CLASS(MapIteratorUnitTests)
    {
    public:

        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Map")
            TEST_CLASS_ATTRIBUTE(L"Iterators", L"Map")
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
        // Populate the map used by the iterator tests.
        //

        MapIteratorUnitTests()
        {
            _m.Add("A", "Value1");
            _m.Add("B", "Value2");
            _m.Add("C", "Value3");
        }

        //
        // Test that we can acquire an iterator on the map.
        //
        // Result: A non-null iterator should be returned.
        //

        TEST_METHOD(AcquireIteratorFromFirst)
        {
            auto iterator = _m.First();

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
        }

        //
        // Test that we can acquire a new iterator on each of the elements
        // by using Find.
        //
        // Result: Non-null iterators should be returned where IsEnd is false.
        //

        TEST_METHOD(AcquireIteratorFromFind)
        {
            auto iterator = _m.Find("A");

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator must not be at end.");

            iterator = _m.Find("B");

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator must not be at end.");

            iterator = _m.Find("C");

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator must not be at end.");
        }

        //
        // Test Find on an element not in the map.
        //
        // Result: An iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(AcquireIteratorForNonexistantElement)
        {
            auto iterator = _m.Find("D");

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsTrue(iterator->IsEnd(), L"Iterator must be at end.");
        }

        //
        // Test that returning a new iterator is not at the end.
        //
        // Result: IsEnd should be false.
        //

        TEST_METHOD(FirstIteratorIsNotAtEnd)
        {
            auto iterator = _m.First();

            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator must not be at end.");
        }

        //
        // Test that advancing Count() times reaches the end.
        //
        // Result: Advance() should return true for Count() - 1 calls then
        // it should return False and IsEnd should be true.
        //

        TEST_METHOD(IteratorAdvance)
        {
            auto iterator = _m.First();
            WalkIteratorOverMap(_m, iterator);
        }

        //
        // Test the Reseting the iterator after reaching the end lets us
        // traverse through the collection again.
        //
        // Result: Reset should permit multiple traverals through the map.
        //

        TEST_METHOD(IteratorReset)
        {
            auto iterator = _m.First();
            WalkIteratorOverMap(_m, iterator);

            iterator->Reset();

            Assert::IsFalse(iterator->IsEnd());
            WalkIteratorOverMap(_m, iterator);
        }

        //
        // Test the Value method on an iterator.
        //
        // Result: The value method should return each element in the map.
        //

        TEST_METHOD(ValueUnderIterator)
        {
            auto iterator = _m.First();
            std::string const * value = nullptr;

            Assert::IsTrue(iterator->Value(&value), L"Iterator->Value returned false.");
            Assert::IsNotNull(value, L"Value returned is null.");
            Assert::AreEqual(std::string("Value1"), *value, L"Expected 'Value1' to be returned.");

            Assert::IsTrue(iterator->Advance(), L"Iterator->Advance returned false.");
            Assert::IsTrue(iterator->Value(&value), L"Iterator->Value returned false.");
            Assert::IsNotNull(value, L"Value returned is null.");
            Assert::AreEqual(std::string("Value2"), *value, L"Expected 'Value2' to be returned.");

            Assert::IsTrue(iterator->Advance(), L"Iterator->Advance returned false.");
            Assert::IsTrue(iterator->Value(&value), L"Iterator->Value returned false.");
            Assert::IsNotNull(value, L"Value returned is null.");
            Assert::AreEqual(std::string("Value3"), *value, L"Expected 'Value3' to be returned.");
        }

        //
        // Test the key method on an iterator.
        //
        // Result: The key method should return each element in the map.
        //

        TEST_METHOD(KeyUnderIterator)
        {
            auto iterator = _m.First();
            std::string const * key = nullptr;

            Assert::IsTrue(iterator->Key(&key), L"Iterator->Key returned false.");
            Assert::IsNotNull(key, L"Key returned is null.");
            Assert::AreEqual(std::string("A"), *key, L"Expected 'A' to be returned.");

            Assert::IsTrue(iterator->Advance(), L"Iterator->Advance returned false.");
            Assert::IsTrue(iterator->Key(&key), L"Iterator->Key returned false.");
            Assert::IsNotNull(key, L"Key returned is null.");
            Assert::AreEqual(std::string("B"), *key, L"Expected 'B' to be returned.");

            Assert::IsTrue(iterator->Advance(), L"Iterator->Advance returned false.");
            Assert::IsTrue(iterator->Key(&key), L"Iterator->Key returned false.");
            Assert::IsNotNull(key, L"Key returned is null.");
            Assert::AreEqual(std::string("C"), *key, L"Expected 'C' to be returned.");
        }

    private:

        //
        // Acquire an iterator and walk through each element.
        //

        void WalkIteratorOverMap(const Map & m,
            Collections::KeyValueIterator<std::map<std::string, std::string>,
                                                          std::string,
                                                          std::string> * iterator
        ) const
        {
            Assert::IsNotNull(iterator, L"Iterator must not be null.");
            Assert::IsFalse(iterator->IsEnd(), L"Iterator should not be at end.");

            for (size_t i = 1; i < m.Count(); ++i)
            {
                Assert::IsTrue(iterator->Advance(), L"Iterator->Advance returned false.");
            }

            Assert::IsTrue(iterator->Advance(), L"Iterator Advance returned false.");
            Assert::IsTrue(iterator->IsEnd(), L"Iterator should be at the end.");
        }

        Map _m;
    };
}