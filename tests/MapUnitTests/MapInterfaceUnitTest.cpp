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
    // Test Map Operations (Creation, Count, Clear, Contains, Add and Remove)
    //

    TEST_CLASS(MapInterfaceUnitTests)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Map")
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
        // Test the Map constructor.
        //
        // Result: a new zero element map should be constructed.
        //

        TEST_METHOD(Constructor)
        {
            Map m;

            Assert::AreEqual((size_t) 0, m.Count(), L"Map should have size of zero.");
        }

        //
        // Associate a key with a value.
        //
        // Result: the map should contain one element.
        //

        TEST_METHOD(MakeMapOfOneElement)
        {
            Map m;

            m.Add("A", "Value1");

            Assert::AreEqual((size_t) 1, m.Count(), L"Map should have a size of 1.");
        }

        //
        // Add three keys to a map.
        // 
        // Result: the map should contain three elements.
        //

        TEST_METHOD(MakeMapOfThreeElements)
        {
            Map m;

            InsertElements(&m);

            Assert::AreEqual((size_t) 3, m.Count(), L"Map should have a size of 3.");
        }

        //
        // Add an element to a map and test if it is there.
        //
        // Result: the map should contain the element.
        //

        TEST_METHOD(ContainsOneElement)
        {
            Map m;

            m.Add("A", "Value1");

            Assert::IsTrue(m.Contains("A"), L"Expected map to contain element 'A'.");
        }

        //
        // Add three elements to a map and test that it contains all three
        // elements.
        //
        // Result: the map should contain all three elements.
        //

        TEST_METHOD(ContainsThreeElements)
        {
            Map m;

            InsertElements(&m);

            Assert::IsTrue(m.Contains("A"), L"Expected map to contain element 'A'.");
            Assert::IsTrue(m.Contains("B"), L"Expected map to contain element 'B'.");
            Assert::IsTrue(m.Contains("C"), L"Expected map to contain element 'C'.");
        }

        //
        // Add an element to a map and test if another element is there.
        //
        // Result: the map should not contain the element.
        //

        TEST_METHOD(DoesNotContainsAnElement)
        {
            Map m;

            m.Add("A", "Value1");

            Assert::IsFalse(m.Contains("B"), L"Map should not contain 'B'.");
        }

        //
        // Create a map and clear it.
        //
        // Result: the map should contain zero elements.
        //

        TEST_METHOD(ClearedEmptyMapContainsNoElements)
        {
            Map m;

            m.Clear();

            Assert::AreEqual((size_t) 0, m.Count(), L"Map should have a size of zero.");
        }

        //
        // Create a new map. Add three elements and clear it.
        //
        // Result: the map should contain zero elements.
        //

        TEST_METHOD(ClearedMapContainsNoElements)
        {
            Map m;

            InsertElements(&m);
            m.Clear();

            Assert::AreEqual((size_t) 0, m.Count(), L"Map should have a size of zero.");
        }

        //
        // Create a new map and add an element. Then remove it.
        //
        // Result: the map should permit the removal and contain no elements.
        //

        TEST_METHOD(RemovalOfOneElementLeavesAnEmptyMap)
        {
            Map m;

            m.Add("A", "Value1");

            Assert::IsTrue(m.Remove("A"));
            Assert::AreEqual((size_t) 0, m.Count(), L"Map should have a size of zero.");
        }

        //
        // Create a new map and add three elements. Then remove them.
        //
        // Result: The removals should succeed and the map should be empty.
        //

        TEST_METHOD(RemovalOfAllElementsLeavesAnEmptyMap)
        {
            Map m;

            InsertElements(&m);

            Assert::IsTrue(m.Remove("A"));
            Assert::IsTrue(m.Remove("B"));
            Assert::IsTrue(m.Remove("C"));
            Assert::AreEqual((size_t) 0, m.Count(), L"Map should have a size of zero.");
        }

        //
        // Create a new map and add an element. Then remove another element.
        //
        // Result: The remove should fail and the first element should be in
        // the map.
        //

        TEST_METHOD(RemovalOfNonExistantElementFails)
        {
            Map m;

            m.Add("A", "Value1");

            Assert::IsFalse(m.Remove("B"), L"Failed to remove element 'B'.");
            Assert::AreEqual((size_t) 1, m.Count(), L"Map should have a size of one.");
        }

        //
        // Create a map and insert a duplicate element.
        //
        // Result: The cardinality of the map should be 1.
        //

        TEST_METHOD(AddDuplicateKeys)
        {
            Map m;

            m.Add("A", "Value1");
            m.Add("A", "Value2");

            Assert::AreEqual((size_t) 1, m.Count(), L"Map should have a size of one.");
        }

    private:
        //
        // Insert three elements into a map.
        //

        void InsertElements(Map * m)
        {
            Assert::IsNotNull(m);

            m->Add("A", "Value1");
            m->Add("B", "Value2");
            m->Add("C", "Value3");
        }
    };
}