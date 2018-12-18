#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "map.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

namespace MapUnitTests
{
    //
    // Test Map Iterator operations (Reset, Advance, IsEnd, Value and Key).
    // Map iterators are acquired by calling Find or First on a Set.
    //

    TEST_CLASS(MapIteratorUnitTest)
    {
    public:

        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Map")
            TEST_CLASS_ATTRIBUTE(L"Iterators", L"Map")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Populate the map used by the iterator tests.
        //

        MapIteratorUnitTest()
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

        TEST_METHOD(TestIteratorFromFirst)
        {
            auto iterator = _m.First();

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
            auto iterator = _m.Find("A");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());

            iterator = _m.Find("B");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());

            iterator = _m.Find("C");

            Assert::IsNotNull(iterator);
            Assert::IsFalse(iterator->IsEnd());
        }

        //
        // Test Find on an element not in the map.
        //
        // Result: An iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(TestIteratorForNonexistantElement)
        {
            auto iterator = _m.Find("D");

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
            auto iterator = _m.First();

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
            auto iterator = _m.First();
            WalkIteratorOverMap(_m, iterator);
        }

        //
        // Test the Reseting the iterator after reaching the end lets us
        // traverse through the collection again.
        //
        // Result: Reset should permit multiple traverals through the map.
        //

        TEST_METHOD(TestIteratorReset)
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

        TEST_METHOD(TestValueUnderIterator)
        {
            auto iterator = _m.First();
            std::string const * value = nullptr;

            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("Value1"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("Value2"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Value(&value));
            Assert::IsNotNull(value);
            Assert::AreEqual(*value, std::string("Value3"));
        }

        //
        // Test the key method on an iterator.
        //
        // Result: The key method should return each element in the map.
        //

        TEST_METHOD(TestKeyUnderIterator)
        {
            auto iterator = _m.First();
            std::string const * key = nullptr;

            Assert::IsTrue(iterator->Key(&key));
            Assert::IsNotNull(key);
            Assert::AreEqual(*key, std::string("A"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Key(&key));
            Assert::IsNotNull(key);
            Assert::AreEqual(*key, std::string("B"));

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->Key(&key));
            Assert::IsNotNull(key);
            Assert::AreEqual(*key, std::string("C"));
        }

    private:

        //
        // Acquire an iterator and walk through each element.
        //

        void WalkIteratorOverMap(const Map & m,
            std::unique_ptr<Collections::KeyValueIterator<std::map<std::string, std::string>,
                                                          std::string,
                                                          std::string>> & iterator
        ) const
        {
            Assert::IsNotNull(iterator.get());
            Assert::IsFalse(iterator->IsEnd());

            for (size_t i = 1; i < m.Count(); ++i)
            {
                Assert::IsTrue(iterator->Advance());
            }

            Assert::IsTrue(iterator->Advance());
            Assert::IsTrue(iterator->IsEnd());
        }

        Map _m;
    };
}