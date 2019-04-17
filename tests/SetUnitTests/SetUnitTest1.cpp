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
    // Test Set Operations (Creation, Count, Clear, Contains, Add and Remove)
    //

    TEST_CLASS(SetUnitTest1)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Set")
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
        // Test the Set constructor.
        //
        // Result: a new zero element set should be constructed.
        //

        TEST_METHOD(Constructor)
        {
            Set s;

            Assert::AreEqual(s.Count(), (size_t) 0);
        }

        //
        // Add a string to a set.
        //
        // Result: the set should contain one element.
        //

        TEST_METHOD(MakeSetOfOneElement)
        {
            Set s;

            s.Add("A");

            Assert::AreEqual(s.Count(), (size_t) 1);
        }

        //
        // Add three strings to a set.
        // 
        // Result: the set should contain three elements.
        //

        TEST_METHOD(MakeSetOfThreeElements)
        {
            Set s;

            InsertElements(&s);

            Assert::AreEqual(s.Count(), (size_t) 3);
        }

        //
        // Add an element to a set and test if it is there.
        //
        // Result: the set should contain the element.
        //

        TEST_METHOD(TestSetContainsOneElement)
        {
            Set s;

            s.Add("A");

            Assert::IsTrue(s.Contains("A"));
        }

        //
        // Add three elements to a set and test that it contains all three
        // elements.
        //
        // Result: the set should contain all three elements.
        //

        TEST_METHOD(TestSetContainsThreeElements)
        {
            Set s;

            InsertElements(&s);

            Assert::IsTrue(s.Contains("A"));
            Assert::IsTrue(s.Contains("B"));
            Assert::IsTrue(s.Contains("C"));
        }

        //
        // Add an element to a set and test if another element is there.
        //
        // Result: the set should not contain the element.
        //

        TEST_METHOD(TestSetDoesNotContainsAnElement)
        {
            Set s;

            s.Add("A");

            Assert::IsFalse(s.Contains("B"));
        }

        //
        // Create a set and clear it.
        //
        // Result: the set should contain zero elements.
        //

        TEST_METHOD(TestClearedEmptySetContainsNoElements)
        {
            Set s;

            s.Clear();

            Assert::AreEqual(s.Count(), (size_t) 0);
        }

        //
        // Create a new set. Add three elements and clear it.
        //
        // Result: the set should contain zero elements.
        //

        TEST_METHOD(TestClearedSetContainsNoElements)
        {
            Set s;

            InsertElements(&s);
            s.Clear();

            Assert::AreEqual(s.Count(), (size_t) 0);
        }

        //
        // Create a new set and add an element. Then remove it.
        //
        // Result: the set should permit the removal and contain no elements.
        //

        TEST_METHOD(TestRemovalOfOneElementLeavesAnEmptySet)
        {
            Set s;

            s.Add("A");

            Assert::IsTrue(s.Remove("A"));
            Assert::AreEqual(s.Count(), (size_t) 0);
        }

        //
        // Create a new set and add three elements. Then remove them.
        //
        // Result: The removals should succeed and the set should be empty.
        //

        TEST_METHOD(TestRemovalOfAllElementsLeavesAnEmptySet)
        {
            Set s;

            InsertElements(&s);

            Assert::IsTrue(s.Remove("A"));
            Assert::IsTrue(s.Remove("B"));
            Assert::IsTrue(s.Remove("C"));
            Assert::AreEqual(s.Count(), (size_t) 0);
        }

        //
        // Create a new set and add an element. Then remove another element.
        //
        // Result: The remove should fail and the first element should be in
        // the set.
        //

        TEST_METHOD(TestRemovalOfNonExistantElementFails)
        {
            Set s;

            s.Add("A");

            Assert::IsFalse(s.Remove("B"));
            Assert::AreEqual(s.Count(), (size_t) 1);
        }

        //
        // Create a set and insert a duplicate element.
        //
        // Result: The cardinality of the set should be 1.
        //

        TEST_METHOD(TestAddDuplicate)
        {
            Set s;

            s.Add("A");
            s.Add("A");

            Assert::AreEqual(s.Count(), (size_t) 1);
        }

        //
        // Create a set and add an empty string elements.
        //
        // Result: The cardinality of the set should be 0.
        //

        TEST_METHOD(TestAddItemsZero)
        {
            Set s;

            s.AddItems("");

            Assert::AreEqual(s.Count(), (size_t)1);
        }

        //
        // Create a set and add one string elements.
        //
        // Result: The cardinality of the set should be 1.
        //

        TEST_METHOD(TestAddItemsOne)
        {
            Set s;

            s.AddItems("A");

            Assert::AreEqual(s.Count(), (size_t)1);
        }

        //
        // Create a set and add two string elements.
        //
        // Result: The cardinality of the set should be 2.
        //

        TEST_METHOD(TestAddItemsTwo)
        {
            Set s;

            s.AddItems("A,B");

            Assert::AreEqual(s.Count(), (size_t)2);
        }

        //
        // Create a set and add duplicate items.
        //
        // Result: The cardinality of the set should be 1.
        //

        TEST_METHOD(TestAddItemsDuplicate)
        {
            Set s;

            s.AddItems("A,A");

            Assert::AreEqual(s.Count(), (size_t)1);
        }

    private:
        //
        // Insert three elements into a set.
        //

        void InsertElements(Set * s)
        {
            Assert::IsNotNull(s);

            s->Add("A");
            s->Add("B");
            s->Add("C");
        }
    };
}