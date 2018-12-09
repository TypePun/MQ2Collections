#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "set.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

namespace SetUnitTests
{		
    TEST_CLASS (SetUnitTest1)
    {
    public:

        //
        // Test the Set constructor.
        //
        // Result: a new zero element set should be constructed.
        TEST_METHOD (SetConstructor)
        {
            Set s;

            Assert::IsTrue (s.Count () == 0);
        }

        //
        // Add a string to a set.
        //
        // Result: the set should contain one element.
        //

        TEST_METHOD (SetOfOneElement)
        {
            Set s;

            s.Add ("A");

            Assert::IsTrue (s.Count () == 1);
        }

        //
        // Add three strings to a set.
        // 
        // Result: the set should contain three elements.
        //

        TEST_METHOD (SetOfThreeElements)
        {
            Set s;

            s.Add ("A");
            s.Add ("B");
            s.Add ("C");

            Assert::IsTrue (s.Count () == 3);
        }

        //
        // Add an element to a set and test if it is there.
        //
        // Result: the set should contain the element.
        //

        TEST_METHOD (SetContainsOneElement)
        {
            Set s;

            s.Add ("A");

            Assert::IsTrue (s.Contains ("A"));
        }

        //
        // Add three elements to a set and test that it contains all three
        // elements.
        //
        // Result: the set should contain all three elements.
        //

        TEST_METHOD (SetContainsThreeElements)
        {
            Set s;

            s.Add ("A");
            s.Add ("B");
            s.Add ("C");

            Assert::IsTrue (s.Contains ("A"));
            Assert::IsTrue (s.Contains ("B"));
            Assert::IsTrue (s.Contains ("C"));
        }

        //
        // Add an element to a set and test if another element is there.
        //
        // Result: the set should not contain the element.
        //

        TEST_METHOD (SetDoesNotContainsOneElement)
        {
            Set s;

            s.Add ("A");

            Assert::IsFalse (s.Contains ("B"));
        }

        //
        // Create a set and clear it.
        //
        // Result: the set should contain zero elements.
        //

        TEST_METHOD (SetCreatedAndClearedContainsNoElements)
        {
            Set s;

            s.Clear ();

            Assert::IsTrue (s.Count () == 0);
        }

        //
        // Create a new set. Add three elements and clear it.
        //
        // Result: the set should contain zero elements.
        //

        TEST_METHOD (SetCreatedThenPopulatedAndClearedContainsNoElements)
        {
            Set s;

            s.Add ("A");
            s.Add ("B");
            s.Add ("C");

            s.Clear ();

            Assert::IsTrue (s.Count () == 0);
        }

        //
        // Create a new set and add an element. Then remove it.
        //
        // Result: the set should permit the removal and contain no elements.
        //

        TEST_METHOD (SetRemovingOnlyElement)
        {
            Set s;

            s.Add ("A");
            
            Assert::IsTrue (s.Remove ("A"));
            Assert::IsTrue (s.Count () == 0);
        }

        //
        // Create a new set and add three elements. Then remove them.
        //
        // Result: The removals should succeed and the set should be empty.
        //

        TEST_METHOD (SetRemoveAllElements)
        {
            Set s;

            s.Add ("A");
            s.Add ("B");
            s.Add ("C");

            Assert::IsTrue (s.Remove ("A"));
            Assert::IsTrue (s.Remove ("B"));
            Assert::IsTrue (s.Remove ("C"));
            Assert::IsTrue (s.Count () == 0);
        }

        //
        // Create a new set and add an element. Then remove another element.
        //
        // Result: The remove should fail and the first element should be in
        // the set.
        //

        TEST_METHOD (SetRemoveOfNonExistantElement)
        {
            Set s;

            s.Add ("A");

            Assert::IsFalse (s.Remove ("B"));
            Assert::IsTrue (s.Count () == 1);
        }

        //
        // Create a set and insert a duplicate element.
        //
        // Result: The cardinality of the set should be 1.
        //

        TEST_METHOD (SetAddDuplicate)
        {
            Set s;

            s.Add ("A");
            s.Add ("A");

            Assert::IsTrue (s.Count () == 1);
        }
    };
}