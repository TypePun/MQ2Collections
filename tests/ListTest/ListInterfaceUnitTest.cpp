#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>
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
    // Test List Operations (Creation, Count, Clear, Contains, Splice, Index, Item,
    // Insert, Sort, Reverse, Append, Remove, Erase, Replace, First, Find, Head, Tail and
    // CountOf.
    //

    TEST_CLASS(ListInterfaceUnitTests)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"List")
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
        // Test the List constructor.
        //
        // Result: a new zero element list is created.
        //

        TEST_METHOD(Constructor)
        {
            //
            // Create a new list.
            //

            List l;

            Assert::AreEqual(l.Count(), (size_t) 0, L"There must be zero elements in the list.");
        }

        //
        // Create a list and clear it.
        //
        // Result: the list should contain zero elements.
        //

        TEST_METHOD(ClearEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            l.Clear();

            Assert::AreEqual(l.Count(), (size_t) 0, L"There must be zero elements in the list.");
        }

        //
        // Create a list, populate it and clear it.
        //
        // Result: the list should contain zero elements.
        //

        TEST_METHOD(ClearList)
        {
            //
            // Create a new list.
            //

            List l;

            AppendFive(l);
            l.Clear();

            Assert::AreEqual(l.Count(), (size_t) 0, L"There must be zero elements in the list.");
        }

        //
        // Create a list, populate it and verify it contains a known element.
        //
        // Result: the list should contain the known element
        //

        TEST_METHOD(ContainsAnElement)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // See if the list contains 'Three'.
            //

            Assert::IsTrue(l.Contains(std::string("Three")), L"List should contain 'Three'.");
        }

        //
        // Create an empty list and verify it does not contain an element.
        //
        // Result: the list should not contain the element.
        //

        TEST_METHOD(EmptyListDoesNotContainAnElement)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // The list must not contain 'One'.
            //

            Assert::IsFalse(l.Contains(std::string("One")), L"List should not contain 'One'.");
        }

        //
        // Create a list, populate it and verify it does not contains a known element.
        //
        // Result: the list should not contain the element.
        //

        TEST_METHOD(DoesNotContainsAnElement)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // See if the list contains 'Six'.
            //

            Assert::IsFalse(l.Contains(std::string("Six")), L"List should contain not 'Six'.");
        }

        //
        // Create an empty list and return a splice from the zeroth position.
        //
        // Result: the splice should contain zero elements.
        //

        TEST_METHOD(EmptySpliceWithZeroIndex)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create a list and retrieve a splice from the end
        //
        // Result: the splice should have no elements.
        //

        TEST_METHOD(SpliceFromEnd)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(l.Count());

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must be zero length.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must have zero elements.");
        }

        //
        // Create an empty list and return a splice from a large position
        //
        // Result: the splice should contain zero elements.
        //

        TEST_METHOD(EmptySpliceWithLargeIndex)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(10000L);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create a list and retrieve a splice from the start.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStart)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must be the same length as the original list.
            //

            Assert::AreEqual(l.Count(),
                             splice->Count(),
                             L"Splice and original must be the same length.");

            CompareListAndSplice(l, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle.
        //
        // Result: the splice should 1/2 the length long.
        //

        TEST_METHOD(SpliceFromMiddle)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice((size_t) l.Count() / 2);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            size_t splice_length = (size_t) std::ceil(l.Count() / 2.0);

            //
            // Check the length of the splice.
            //

            Assert::AreEqual(splice_length, splice->Count(), L"Splice length is incorrect.");

            CompareListAndSplice(l, l.Count() - splice_length, splice, 0);
        }

        //
        // Create an empty list and splice from the start with a zero length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithZeroLength)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(0, 0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create an empty list and splice from the start with a positive length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveLength)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(0, 1);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create an empty list and splice from a positive offset and a zero length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveOffsetAndZeroLength)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(10, 0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create an empty list and splice from a positive offset and a positive length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveOffsetAndPositiveLength)
        {
            //
            // Create a new list.
            //

            List l;

            auto splice = l.Splice(10, 10);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have no elements.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must be empty.");
        }

        //
        // Create a list and retrieve a splice from the start with a length
        // longer than the list.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStartPastTheEnd)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(0, l.Count() * 2);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must be the same length as the original list.
            //

            Assert::AreEqual(l.Count(),
                             splice->Count(),
                             L"Splice and original must be the same length.");

            CompareListAndSplice(l, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the start with a length
        // equal to the list.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStartForLengthOfList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(0, l.Count());

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must be the same length as the original list.
            //

            Assert::AreEqual(l.Count(),
                             splice->Count(),
                             L"Splice and original must be the same length.");

            CompareListAndSplice(l, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the start with a length
        // of zero.
        //
        // Result: the splice should have a length of zero.
        //

        TEST_METHOD(SpliceFromStartWithZeroLength)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(0, 0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have a length of zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Returned splice must have a length of zero.");
        }

        //
        // Create a list and retrieve a splice from the start with a length of one.
        //
        // Result: the splice should have a length of one.
        //

        TEST_METHOD(SpliceFromStartForLengthOfOne)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(0, 1);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(l, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the end for a length of one.
        //
        // Result: the splice should have a length of one.
        //

        TEST_METHOD(SpliceFromEndForLengthOfOne)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(l.Count() - 1, 1);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(l, l.Count() - 1, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the end for a length of zero.
        //
        // Result: the splice should have a length of zero.
        //

        TEST_METHOD(SpliceFromEndForLengthOfZero)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(l.Count() - 1, 0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have a length of zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must have a length of zero.");
        }

        //
        // Create a list and retrieve a splice from the end for a large length.
        //
        // Result: the splice should have a length of one.
        //

        TEST_METHOD(SpliceFromEndForLargeLength)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto splice = l.Splice(l.Count() - 1, l.Count() * 2);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");

            //
            // Splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(l, l.Count() - 1, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle with a length of one.
        //
        // Result: the splice should of length one.
        //

        TEST_METHOD(SpliceFromMiddleWithLengthOfOne)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            size_t splice_pos = l.Count() / 2;
            auto splice = l.Splice(splice_pos, 1);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");
            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice length is incorrect.");

            CompareListAndSplice(l, splice_pos, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle with a length of zero,
        //
        // Result: the splice should of length zero.
        //

        TEST_METHOD(SpliceFromMiddleWithLengthOfZero)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            size_t splice_pos = l.Count() / 2;
            auto splice = l.Splice(splice_pos, 0);

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");
            Assert::AreEqual(splice->Count(), (size_t)0, L"Splice must have a length of zero.");
        }

        //
        // Create a list and retrieve a splice from the middle with a length of the
        // remainder.
        //
        // Result: the splice should contain half the elements.
        //

        TEST_METHOD(SpliceFromMiddleForRemainder)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            size_t splice_pos = l.Count() / 2;
            auto splice = l.Splice(splice_pos, l.Count() - splice_pos);
            auto splice_length = l.Count() - splice_pos;

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");
            Assert::AreEqual(splice->Count(), splice_length, L"Splice length is incorrect");

            CompareListAndSplice(l, splice_pos, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle with a large length.
        //
        // Result: the splice should contain half the elements.
        //

        TEST_METHOD(SpliceFromMiddleForLargeLength)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            size_t splice_pos = l.Count() / 2;
            auto splice = l.Splice(splice_pos, 10000);
            auto splice_length = l.Count() - splice_pos;

            //
            // See if the returned splice exists.
            //

            Assert::IsNotNull(splice.get(), L"Splice must not be a nullptr.");
            Assert::AreEqual(splice->Count(), splice_length, L"Splice length is incorrect.");

            CompareListAndSplice(l, splice_pos, splice, 0);
        }

        //
        // Test the list index method on an empty list.
        //
        // Result: -1 should be returned.
        //

        TEST_METHOD(IndexOnEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Look up 'Zero'  -- it should return -1 indicating the element is
            // not in the list.
            //

            Assert::AreEqual(l.Index(std::string("Zero")), (long) -1, L"Index of 'Zero' should be -1.");
        }

        //
        // Test the list index method. An index is the zero-based position of
        // an item in the list.
        //
        // Result: return the index of an item in a list.
        //

        TEST_METHOD(IndexForKnownElements)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Index each element in the list.  It should succeed.
            //

            for (long index = 0; index < sizeof(elements) / sizeof(elements[0]); ++index)
            {
                Assert::AreEqual(l.Index(elements[index]), index, L"Index was incorrect for element.");
            }
        }

        //
        // Test the list index method for an element not in the list.
        //
        // Result: -1 should be returned.
        //

        TEST_METHOD(IndexForUnknownElement)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Look up 'Zero'  -- it should return -1 indicating the element is
            // not in the list.
            //

            Assert::AreEqual(l.Index(std::string("Zero")), (long) -1, L"Index of 'Zero' should be -1.");
        }

        //
        // Retrieve the item at position zero in an empty list.
        //
        // Result: the item method should return false.
        //

        TEST_METHOD(ItemEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            const std::string *pitem;

            //
            // Look up an entry beyond the end of the list.  It should fail.
            //

            Assert::IsFalse(l.Item(0, &pitem), L"Item for empty list should fail.");
        }

        //
        // Retrieve each item in a populated list.
        //
        // Result: the items should be returned in order.
        //

        TEST_METHOD(ItemCompareEachEntry)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Compare the list to the expected elements.
            //

            CompareListToElements(l, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Retrieve an item after the end of a list.
        //
        // Result: the items should be returned in order.
        //

        TEST_METHOD(ItemAfterListEnd)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Retrieve and item after the list end.
            //

            const std::string * pitem;
            Assert::IsFalse(l.Item(l.Count() + 1, &pitem), L"Item beyond end should return false.");
        }

        //
        // Insert an empty list into an empty list.
        //
        // Result: the list should be empty.
        //

        TEST_METHOD(InsertEmptyListIntoEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Create a new list with no elements.
            //

            std::list<std::string> l1;

            //
            // Insert the empty list at the start of l.
            //

            Assert::IsTrue(l.Insert(0, l1), L"Could not insert at start of list.");

            //
            // l should be zero entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"Final list should be empty.");
        }

        //
        // Insert an empty list at the start of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListAtStartOfList)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with no elements.
            //

            std::list<std::string> l1;

            //
            // Insert an empty list at the start of the list.
            //

            Assert::IsTrue(l.Insert(0, l1), L"Could not insert at start of list.");

            //
            // l should be 5 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Target list should be 5 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                elements,
                sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert an empty list at the end of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListAtEndOfList)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with no elements.
            //

            std::list<std::string> l1;

            //
            // Insert an empty list at the end of the list.
            //

            Assert::IsTrue(l.Insert(l.Count() - 1, l1), L"Could not insert at end of list.");

            //
            // l should be 5 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Target list should be 5 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                elements,
                sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert an empty list in the middle of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListInMiddleOfList)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with no elements.
            //

            std::list<std::string> l1;

            //
            // Insert an empty list in the middle of the list.
            //

            Assert::IsTrue(l.Insert(l.Count() / 2, l1), L"Could not insert in the middle of list.");

            //
            // l should be 5 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Target list should be 5 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                elements,
                sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list at the start of another list.
        //
        // Result: new list is prepended to the old list.
        //

        TEST_METHOD(InsertListAtStartOfAnotherList)
        {
            std::string insertedelements[] =
            {
                "A",
                "B",
                "C",
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with three elements.
            //

            std::list<std::string> l1 =
            {
                "A",
                "B",
                "C"
            };

            //
            // Insert a new list at the start of l.
            //

            Assert::IsTrue(l.Insert(0, l1), L"Could not insert at start of list.");

            //
            // l should be 8 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 8, L"Target list should be 8 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                insertedelements,
                sizeof(insertedelements) / sizeof(insertedelements[0]));
        }

        //
        // Insert a list at the end of another list.
        //
        // Result: new list is appended to the old list.
        //

        TEST_METHOD(InsertListAtEndOfAnotherList)
        {
            std::string insertedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five",
                "A",
                "B",
                "C"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with three elements.
            //

            std::list<std::string> l1 =
            {
                "A",
                "B",
                "C"
            };

            //
            // Insert the new list at the end of l.
            //

            Assert::IsTrue(l.Insert(l.Count(), l1), L"Could not insert at end of list.");

            //
            // l should be 8 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 8, L"Target list should be 8 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                insertedelements,
                sizeof(insertedelements) / sizeof(insertedelements[0]));
        }

        //
        // Insert a list in the middle of another list.
        //
        // Result: the new list is composed of first list with second list
        // inserted in the middle of the first list.
        //

        TEST_METHOD(InsertListInMiddleOfAnotherList)
        {
            std::string insertedelements[] =
            {
                "One",
                "Two",
                "Three",
                "A",
                "B",
                "C",
                "Four",
                "Five",
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with three elements.
            //

            std::list<std::string> l1 =
            {
                "A",
                "B",
                "C"
            };

            //
            // Insert the new list in the middle of the list.
            //

            Assert::IsTrue(l.Insert((l.Count() / 2) + 1, l1), L"Could not insert in middle of list.");

            //
            // l should be 8 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 8, L"Target list should be 8 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                insertedelements,
                sizeof(insertedelements) / sizeof(insertedelements[0]));
        }

        //
        // Insert a list at in invalid entry past the end of the list.
        //
        // Result: list should not be modified.
        //

        TEST_METHOD(InsertPastEnd)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Create a new list with three elements.
            //

            std::list<std::string> l1 =
            {
                "A",
                "B",
                "C"
            };

            //
            // Insert the new list after the end of the list.
            //

            Assert::IsFalse(l.Insert(l.Count() + 1, l1), L"Insert past end should fail.");

            //
            // l should be 5 entries long (unmodified).
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Target list should be 5 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                elements,
                sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list into an empty list.
        //
        // Result: list should contain the inserted elements.
        //

        TEST_METHOD(InsertIntoEmptyList)
        {
            std::string insertedelements[] =
            {
                "A",
                "B",
                "C"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Create a new list with three elements.
            //

            std::list<std::string> l1 =
            {
                "A",
                "B",
                "C"
            };

            //
            // Insert the empty list at the start of l.
            //

            Assert::IsTrue(l.Insert(0, l1), L"Could not insert at start of list.");

            //
            // l should be 3 entries long.
            //

            Assert::AreEqual(l.Count(), (size_t) 3, L"Target list should be 3 entries long.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(
                l,
                insertedelements,
                sizeof(insertedelements) / sizeof(insertedelements[0]));
        }

        //
        // Sort an empty list.
        //
        // Result: empty list should be returned.
        //

        TEST_METHOD(SortEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Sort the list.
            //

            l.Sort();

            //
            // List should contain zero elements.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"List must have zero entries.");
        }

        //
        // Sort the list.
        //
        // Result: list should be sorted.
        //

        TEST_METHOD(SortList)
        {
            std::string sortedelements[] =
            {
                "Five",
                "Four",
                "One",
                "Three",
                "Two"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Sort the list.
            //

            l.Sort();

            //
            // List must be five in length.
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Sorted list must be same length.");

            //
            // Verify the elements are sorted.
            //

            CompareListToElements(
                l,
                sortedelements,
                sizeof(sortedelements) / sizeof(sortedelements[0]));
        }

        //
        // Reverse an empty list.
        //
        // Result: an empty list should be returned.
        //

        TEST_METHOD(ReverseAnEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Reverse the list.
            //

            l.Reverse();

            //
            // List must contain zero elements.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"Reversed list must be empty.");
        }

        //
        // Reverse the list.
        //
        // Result: list should be reversed.
        //

        TEST_METHOD(ReverseList)
        {
            std::string reservsedelements[] =
            {
                "Five",
                "Four",
                "Three",
                "Two",
                "One"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Reverse the list.
            //

            l.Reverse();

            //
            // List must be five in length.
            //

            Assert::AreEqual(l.Count(), (size_t) 5, L"Reversed list must be same length.");

            //
            // Verify the elements are reversed.
            //

            CompareListToElements(
                l,
                reservsedelements,
                sizeof(reservsedelements) / sizeof(reservsedelements[0]));
        }

        //
        // Append an item to an empty list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToEmptyList)
        {
            std::string appendedelements[] =
            {
                "One"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append an item to the list
            //

            l.Append("One");

            //
            // Verify the list has one item.
            //

            Assert::AreEqual(l.Count(), (size_t) 1, L"List length must be one.");

            //
            // Verify the elements are correct.
            //

            CompareListToElements(
                l,
                appendedelements,
                sizeof(appendedelements) / sizeof(appendedelements[0]));
        }

        //
        // Append an item to a populated list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToList)
        {
            std::string appendedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five",
                "One",
                "Two"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Append an item to the list
            //

            l.Append("One");
            l.Append("Two");

            //
            // Verify the list has seven item.
            //

            Assert::AreEqual(l.Count(), (size_t) 7, L"List length must be seven.");

            //
            // Verify the elements are correct.
            //

            CompareListToElements(
                l,
                appendedelements,
                sizeof(appendedelements) / sizeof(appendedelements[0]));
        }

        //
        // Remove an item from an empty list.
        //
        // Result: remove should return zero items removed.
        //

        TEST_METHOD(RemoveFromEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Remove an element from the empty list.
            //

            Assert::AreEqual(l.Remove("Zero"), (size_t) 0, L"Can not remove item from empty list.");

            //
            // Length of the list should be zero.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"List must be empty.");
        }

        //
        // Remove an item that is not in the list.
        //
        // Result: No items should be remvoed and the list should not be modified.
        //

        TEST_METHOD(RemoveItemNotInList)
        {
            std::string removedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Remove an element not in the list.
            //

            Assert::AreEqual(l.Remove(std::string("Zero")), (size_t) 0, L"No elements should be removed from the list");

            //
            // Verify no elements were removed.
            //

            CompareListToElements(
                l,
                removedelements,
                sizeof(removedelements) / sizeof(removedelements[0]));
        }

        //
        // Remove the first element from the list.
        //
        // Result: only the first item should be removed.
        //

        TEST_METHOD(RemoveFirstItem)
        {
            std::string removedelements[] =
            {
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Remove the first element.  Only one entry should have been
            // removed.
            //

            Assert::AreEqual(l.Remove(std::string("One")), (size_t) 1, L"Only one element should be removed from the list");

            //
            // Verify the element was removed.
            //

            CompareListToElements(
                l,
                removedelements,
                sizeof(removedelements) / sizeof(removedelements[0]));
        }

        //
        // Remove the last element from the list.
        //
        // Result: only the last item should be removed.
        //

        TEST_METHOD(RemoveLastItem)
        {
            std::string removedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Remove the first element.  Only one entry should have been
            // removed.
            //

            Assert::AreEqual(l.Remove(std::string("Five")), (size_t) 1, L"Only one element should be removed from the list");

            //
            // Verify the element was removed.
            //

            CompareListToElements(
                l,
                removedelements,
                sizeof(removedelements) / sizeof(removedelements[0]));
        }

        //
        // Remove a middle element from the list.
        //
        // Result: only the middle item should be removed.
        //

        TEST_METHOD(RemoveMiddleItem)
        {
            std::string removedelements[] =
            {
                "One",
                "Two",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Remove the first element.  Only one entry should have been
            // removed.
            //

            Assert::AreEqual(l.Remove(std::string("Three")), (size_t) 1, L"Only one element should be removed from the list");

            //
            // Verify the element was removed.
            //

            CompareListToElements(
                l,
                removedelements,
                sizeof(removedelements) / sizeof(removedelements[0]));
        }

        //
        // Duplicate a list and remove an element.
        //
        // Result: removed item should not be in the list and the remove
        // count should be two.
        //

        TEST_METHOD(RemoveMultiple)
        {
            std::string removedelements[] =
            {
                "One",
                "Two",
                "Four",
                "Five",
                "One",
                "Two",
                "Four",
                "Five"
            };
            std::string appendelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append the list to itself.
            //

            AppendFive(l);

            //
            // Append five elements to the list.
            //

            for (auto x : appendelements)
            {
                l.Append(x);
            }

            //
            // Remove the third element.  Two entries should have been
            // removed.
            //

            Assert::AreEqual(l.Remove(std::string("Three")), (size_t) 2, L"Two elements should be removed from the list.");

            //
            // Verify the element was removed.
            //

            CompareListToElements(
                l,
                removedelements,
                sizeof(removedelements) / sizeof(removedelements[0]));
        }

        //
        // Erase an item from an empty list.
        //
        // Result: erase should return false.
        //

        TEST_METHOD(EraseFromEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Erase an element not in the list element.
            //

            Assert::IsFalse(l.Erase(0), L"No elements should be removed from an empty list.");
        }

        //
        // Erase the first element.
        //
        // Result: erase should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseFirstItem)
        {
            std::string erasedelements[] =
            {
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Erase the first element. The erase should succeed.
            //

            Assert::IsTrue(l.Erase(0), L"Erase should succeed for first element.");

            //
            // Verify the element was erased.
            //

            CompareListToElements(
                l,
                erasedelements,
                sizeof(erasedelements) / sizeof(erasedelements[0]));
        }

        //
        // Erasing a middle element.
        //
        // Result: erase should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseMiddleItem)
        {
            std::string erasedelements[] =
            {
                "One",
                "Two",
                "Four",
                "Five",
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Erase the third element.
            //

            Assert::IsTrue(l.Erase(2), L"Middle element should be erased from the list.");

            //
            // Verify the element was erased.
            //

            CompareListToElements(
                l,
                erasedelements,
                sizeof(erasedelements) / sizeof(erasedelements[0]));
        }

        //
        // Erasing the last element.
        //
        // Result: erase method should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseLastItem)
        {
            std::string erasedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Erase the last element.
            //

            Assert::IsTrue(l.Erase(l.Count() - 1), L"Last element should be erased from the list.");

            //
            // Verify the element was erased.
            //

            CompareListToElements(
                l,
                erasedelements,
                sizeof(erasedelements) / sizeof(erasedelements[0]));
        }

        //
        // Erase an item beyond the list.
        //
        // Result: erase method should fail and the list should not be modified.
        //

        TEST_METHOD(EraseItemOutOfBounds)
        {
            std::string erasedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Erase an element not in the list element.
            //

            Assert::IsFalse(l.Erase(l.Count()), L"No elements should be removed from the list.");

            //
            // Verify no elements were removed.
            //

            CompareListToElements(
                l,
                erasedelements,
                sizeof(erasedelements) / sizeof(erasedelements[0]));
        }

        //
        // Replace an item in an empty list.
        //
        // Result: count of items replaced should be zero.
        //

        TEST_METHOD(ReplaceInEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Replace an element in the empty list.
            //

            Assert::AreEqual(l.Replace("Zero", "Empty"), (size_t) 0, L"Empty list replace should fail.");
        }

        //
        // Replace the first element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceFirstElement)
        {
            std::string replacedelements[] =
            {
                "Zero",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Replace the first element.  Only one entry should have been
            // replaced.
            //

            Assert::AreEqual(l.Replace("One", "Zero"), (size_t) 1, L"Only one element should be replaced in the list.");

            //
            // Verify the element was replaced.
            //

            CompareListToElements(
                l,
                replacedelements,
                sizeof(replacedelements) / sizeof(replacedelements[0]));
        }

        //
        // Replace the last element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceLastElement)
        {
            std::string replacedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Last"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Replace the last element.  Only one entry should have been
            // replaced.
            //

            Assert::AreEqual(l.Replace("Five", "Last"), (size_t) 1, L"Only one element should be replaced in the list.");

            //
            // Verify the element was replaced.
            //

            CompareListToElements(
                l,
                replacedelements,
                sizeof(replacedelements) / sizeof(replacedelements[0]));
        }

        //
        // Replace a middle element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceMiddleElement)
        {
            std::string replacedelements[] =
            {
                "One",
                "Two",
                "Middle",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Replace the last element.  Only one entry should have been
            // replaced.
            //

            Assert::AreEqual(l.Replace("Three", "Middle"), (size_t) 1, L"Only one element should be replaced in the list.");

            //
            // Verify the element was replaced.
            //

            CompareListToElements(
                l,
                replacedelements,
                sizeof(replacedelements) / sizeof(replacedelements[0]));
        }

        //
        // Replace an item that is not in the list.
        //
        // Result: list should not be modified.
        //

        TEST_METHOD(ReplaceItemNotInList)
        {
            std::string replacedelements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Replace an element not in the list element.
            //

            Assert::AreEqual(l.Replace("Zero", "Empty"), (size_t) 0, L"No elements should be replaced in the list.");

            //
            // Verify no elements were replaced.
            //

            CompareListToElements(
                l,
                replacedelements,
                sizeof(replacedelements) / sizeof(replacedelements[0]));
        }

        //
        // Replace an entry that occurs more than once in a list.
        //
        // Result: replaced item should be replaced for each occurence in the original list.
        //

        TEST_METHOD(ReplaceMultiple)
        {
            std::string replacedelements[] =
            {
                "One",
                "Two",
                "Zero",
                "Four",
                "Five",
                "One",
                "Two",
                "Zero",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Append the list to itself.
            //

            std::string appenditems[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            for (auto ix : appenditems)
            {
                l.Append(ix);
            }

            //
            // Replace the third element. Two entries should have been
            // replaced.
            //

            Assert::AreEqual(l.Replace("Three", "Zero"), (size_t) 2, L"Two elements should be replaced in the list.");

            //
            // Verify the element was replaced.
            //

            CompareListToElements(
                l,
                replacedelements,
                sizeof(replacedelements) / sizeof(replacedelements[0]));
        }

        //
        // Get the head of an empty list.
        //
        // Result: the head method call should fail.
        //

        TEST_METHOD(HeadOfEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Popping the head should fail.
            //

            std::unique_ptr<const std::string> pitem;

            Assert::IsFalse(l.Head(&pitem), L"Head of an empty list should fail.");
        }

        //
        // Retrieve the head of the list.
        //
        // Result: the head should be successfully returned.
        //

        TEST_METHOD(HeadOfList)
        {
            std::string element = "One";

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Pop the head and verify it is expected value.
            //

            std::unique_ptr<const std::string> pItem;
            Assert::IsTrue(l.Head(&pItem), L"Could not remove head of list.");

            Assert::AreEqual(*pItem, element, L"Incorrect item popped from list.");
        }

        //
        // Pop all entries from the list using the Head method.
        //
        // Result: Each entry must match and at the end the list must be empty.
        //

        TEST_METHOD(PopAllFromHead)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Pop each item and verify that it is the same as the
            // corresponding entry in the elements list.
            //

            std::unique_ptr<const std::string> pitem;
            size_t nItem;

            nItem = 0;
            while (true)
            {
                Assert::IsTrue(l.Head(&pitem), L"Could not retrieve item from head of list.");
                Assert::AreEqual(*pitem, elements[nItem], L"Item returned is incorrect.");

                ++nItem;

                if (nItem == (sizeof(elements) / sizeof(elements[0])))
                {
                    break;
                }
            }

            //
            // The list should be empty once all of the elements are removed.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"List must be empty.");
        }

        //
        // Get the tail of an empty list.
        //
        // Result: the tail method call should fail.
        //

        TEST_METHOD(TailOfEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Popping the tail should fail.
            //

            std::unique_ptr<const std::string> pitem;

            Assert::IsFalse(l.Tail(&pitem), L"Tail of an empty list should fail.");
        }

        //
        // Retrieve the tail of the list.
        //
        // Result: the tail should be successfully returned.
        //

        TEST_METHOD(TailOfList)
        {
            std::string element = "Five";

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Pop the tail and verify it is expected value.
            //

            std::unique_ptr<const std::string> pItem;
            Assert::IsTrue(l.Tail(&pItem), L"Could not remove tail of list.");

            Assert::AreEqual(*pItem, element, L"Incorrect item popped from list.");
        }

        //
        // Pop all entries from the list using the Tail method.
        //
        // Result: Each entry must match and at the end the list must be empty.
        //

        TEST_METHOD(PopAllFromTail)
        {
            std::string elements[] =
            {
                "Five",
                "Four",
                "Three",
                "Two",
                "One"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Pop each item and verify that it is the same as the
            // corresponding entry in the elements list.
            //

            std::unique_ptr<const std::string> pitem;
            size_t nItem;

            nItem = 0;
            while (true)
            {
                Assert::IsTrue(l.Tail(&pitem), L"Could not retrieve item from tail of list.");
                Assert::AreEqual(*pitem, elements[nItem], L"Item returned is incorrect.");

                ++nItem;

                if (nItem == (sizeof(elements) / sizeof(elements[0])))
                {
                    break;
                }
            }

            //
            // The list should be empty once all of the elements are removed.
            //

            Assert::AreEqual(l.Count(), (size_t) 0, L"List must be empty");
        }

        //
        // Retrieve the CountOf on an empty list.
        //
        // Result: The value returned should be zero.
        //

        TEST_METHOD(CountOfOnEmptyList)
        {
            //
            // Create a new list.
            //

            List l;

            Assert::AreEqual(l.CountOf("Zero"), (size_t) 0, L"CountOf should return zero.");
        }

        //
        // Retrieve CountOf for an element that does not exist.
        //
        // Result: the value should be zero.
        //

        TEST_METHOD(CountOfNonexistantElement)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            Assert::AreEqual(l.CountOf("Zero"), (size_t) 0, L"CountOf should return zero.");
        }
        
        //
        // Retrieve the CountOf for each element in the list.
        //
        // Result: each element should occur once in the list.
        //

        TEST_METHOD(CountOfKnownElements)
        {
            std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            //
            // Verify that each element occurs once in the the list.
            //

            for (auto it : elements)
            {
                Assert::AreEqual(l.CountOf(it), (size_t) 1, L"CountOf should return one.");
            }
        }

    private:

        //
        // Append five elements to a list.
        //

        void AppendFive(List & l) const
        {
            //
            // Append five elements to the list, testing the length after
            // each append.
            //

            l.Append(std::string("One"));
            Assert::AreEqual(l.Count(), (size_t) 1, L"There must be one elements in the list");

            l.Append(std::string("Two"));
            Assert::AreEqual(l.Count(), (size_t) 2, L"There must be two elements in the list");

            l.Append(std::string("Three"));
            Assert::AreEqual(l.Count(), (size_t) 3, L"There must be three elements in the list");

            l.Append(std::string("Four"));
            Assert::AreEqual(l.Count(), (size_t) 4, L"There must be four elements in the list");

            l.Append(std::string("Five"));
            Assert::AreEqual(l.Count(), (size_t) 5, L"There must be five elements in the list");
        }

        //
        // Compare a list and a splice starting at an origin in the list and splice.
        // A number of tests are performed on the original and splice indices and element
        // values.
        //

        void CompareListAndSplice(const List & l,
                                  size_t list_origin,
                                  const std::unique_ptr<List> & splice,
                                  size_t splice_origin)
        {
            const std::string *original_item;
            const std::string *splice_item;
            size_t splice_index = splice_origin;
            for (size_t index = list_origin; index < std::min<size_t>(l.Count(), splice->Count()); ++index)
            {
                Assert::IsTrue(l.Item(index, &original_item), L"Could not retrieve list item.");
                Assert::IsTrue(splice->Item(splice_index, &splice_item), L"Could not retrieve splice item.");
                Assert::IsNotNull(original_item, L"List item returned is a nullptr.");
                Assert::IsNotNull(splice_item, L"Split item returned is a nullptr.");
                Assert::AreEqual(*original_item, *splice_item, L"List and splice strings are not the same.");

                Assert::IsTrue(splice_index++ < splice->Count(), L"splice_index past splice Count.");
            }
        }

        //
        // Compare a list with a sequence of strings.
        //

        void CompareListToElements(
            const List & l,
            const std::string elements[],
            size_t numElements) const
        {
            //
            // Retrieve the item at a position in the list.
            //

            const std::string *pitem;
            for (size_t index = 0; index < numElements; ++index)
            {
                Assert::IsTrue(l.Item(index, &pitem), L"Index was incorrect for element.");

                Assert::AreEqual(*pitem, elements[index], L"Returned string does not match expected string");
            }
        }
    };
}