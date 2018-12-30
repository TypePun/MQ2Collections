#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>
#include "list.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

namespace ListTest
{
    //
    // Test List Operations (Creation, Count, Clear, Contains, Splice, Index, Item,
    // Insert, Sort, Reverse, Append, Remove, Erase, Replace, First, Find, Head, Tail and
    // CountOf.
    //

    TEST_CLASS(ListUnitTest1)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"List")
        END_TEST_CLASS_ATTRIBUTE()

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
        // Create a list and retrieve a splice from the middle with a large length/
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
            // Remove an element not in the list element.
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

        TEST_METHOD(ReplaceItemNoInList)
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

            Assert::IsFalse(l.Head(&pitem), L"Head of an empty list should fail");
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

            Assert::AreEqual(*pItem, std::string("One"), L"Incorrect item popped from list.");
        }

        //
        // Pop all entries from the list using the Head method.
        //
        // Result: Each entry must match and at the end the list must be empty.
        //

        TEST_METHOD(PopAllWithHead)
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

            Assert::AreEqual(l.Count(), (size_t) 0, L"List must be empty");
        }

        /***

        //
        // Test the list Find method, finding each element
        // in the list and using the returned iterator.
        //
        // Result: The iterator should be returned and be usable.
        //

        TEST_METHOD(ListFind1)
        {
            const std::string elements[] =
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
            // Iterate over the elements, finding each one.  The find must
            // succeed and the list of elements must be equal to the
            // tail following the current element.
            //

            size_t items = sizeof(elements) / sizeof(elements[0]);
            size_t currentItem = 0;
            for (const auto & item : elements)
            {
                auto iter = l.Find(item);

                //
                // Validate and use the iterator.
                //

                ValidateAndUseIterator(
                    iter,
                    elements,
                    items,
                    currentItem
                );

                ++currentItem;

                //
                // Then reset the iterator and use it.
                //

                iter->Reset();
                ValidateAndUseIterator(
                    iter,
                    elements,
                    items,
                    0
                );
            }
        }

        //
        // Test the list find method, finding an element that is
        // not in the list.
        //
        // Result: Returned iterator should be at end.
        //

        TEST_METHOD(ListFind2)
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
            // Find an item that is not in the list.  IsEnd should be true.
            //

            auto iter = l.Find(std::string("Zero"));

            Assert::IsTrue(iter->IsEnd(),
                L"IsEnd should be true",
                LINE_INFO()
            );
        }

        //
        // Test the list find method on an empty list.
        //
        // Result: Returned iterator should be at end.
        //

        TEST_METHOD(ListFind3)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Find an item.  Since the list is empty, IsEnd should be true.
            //

            auto iter = l.Find(std::string("Zero"));

            Assert::IsTrue(iter->IsEnd(),
                L"IsEnd should be true",
                LINE_INFO()
            );
        }

        //
        // Test the list first method, returning an iterator to the first
        // element of the list.
        //
        // Result: the iterator should be returned and be usable.
        //

        TEST_METHOD(ListFirst1)
        {
            const std::string elements[] =
            {
                "One",
                "Two",
                "Three",
                "Four",
                "Five"
            };
            size_t items = sizeof(elements) / sizeof(elements[0]);

            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            auto iter = l.First();

            //
            // Validate and use the iterator.
            //

            ValidateAndUseIterator(
                iter,
                elements,
                items,
                0
            );

            //
            // Then reset the iterator and use it.
            //

            iter->Reset();
            ValidateAndUseIterator(
                iter,
                elements,
                items,
                0
            );
        }

        //
        // Test the list first method on an empty list.
        //
        // Result: the iterator should be returned and be at the end.
        //

        TEST_METHOD(ListFirst2)
        {
            //
            // Create a new list.
            //

            List l;

            auto iter = l.First();

            Assert::IsTrue(iter->IsEnd(),
                L"IsEnd should be true",
                LINE_INFO()
            );
        }



        //
        // Test the list Tail method.
        //
        // Result: Remove each element by popping the tail until the list
        // is empty.
        //

        TEST_METHOD(ListTail1)
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

            const std::string *pitem;
            size_t nItem;

            nItem = 4;
            while (l.Tail(&pitem))
            {
                Assert::IsTrue(*pitem == elements[nItem],
                    L"Item returned is incorrect",
                    LINE_INFO()
                );

                //
                // Done with the item.  Delete it.
                //

                delete pitem;

                --nItem;
            }

            //
            // The list should be empty once all of the elements are removed.
            //

            Assert::IsTrue(l.Count() == 0,
                L"List must be empty",
                LINE_INFO()
            );
        }

        //
        // Test the list Tail method on an empty list.
        //
        // Result: The Tail method call should fail.
        //

        TEST_METHOD(ListTail2)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Popping the tail should fail.
            //

            const std::string *pitem;

            Assert::IsFalse(l.Tail(&pitem),
                L"Tail of an empty list should fail",
                LINE_INFO()
            );
        }

        //
        // Test the list CountOf method.
        //
        // Result: Each element should occur once in the list.
        //

        TEST_METHOD(ListCountOf1)
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
                Assert::IsTrue(l.CountOf(it) == 1,
                    L"CountOf should return 1",
                    LINE_INFO()
                );
            }
        }

        //
        // Test the list CountOf method.  Append the list to itself.
        //
        // Result: Each element should occur twice in the list.
        //

        TEST_METHOD(ListCountOf2)
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
            // Append the list to itself.
            //

            const std::string *pitem;
            size_t appendCount;

            appendCount = l.Count();
            for (size_t index = 0; index < appendCount; ++index)
            {
                Assert::IsTrue(l.Item(index, &pitem),
                    L"Could not retrieve item",
                    LINE_INFO()
                );

                l.Append(*pitem);
            }

            //
            // Verify that each element occurs twice in the the list.
            //

            for (auto it : elements)
            {
                Assert::IsTrue(l.CountOf(it) == 2,
                    L"CountOf should return 2",
                    LINE_INFO()
                );
            }
        }

        //
        // Test the list CountOf method for an element that does not exist.
        //
        // Result: The value should be zero.
        //

        TEST_METHOD(ListCountOf3)
        {
            //
            // Create a new list.
            //

            List l;

            //
            // Append five elements to the list.
            //

            AppendFive(l);

            Assert::IsTrue(l.CountOf(std::string("Zero")) == 0,
                L"CountOf should return 0",
                LINE_INFO()
            );
        }

        //
        // Test the list CountOf method on an empty list.
        //
        // Result: The value should be zero.
        //

        TEST_METHOD(ListCountOf4)
        {
            //
            // Create a new list.
            //

            List l;

            Assert::IsTrue(l.CountOf(std::string("Zero")) == 0,
                L"CountOf should return 0",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Clear method on
        // an empty list.
        //

        TEST_METHOD(ListGetMemberClear)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Clear the list.
            //

            bResult = List::GetMemberInvoker(source, "Clear", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Clear failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Clear should return True",
                LINE_INFO()
            );

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Count failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Count should be zero",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Count, Clear
        // and Append methods.
        //

        TEST_METHOD(ListGetMemberAppendAndClear)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Clear the list.
            //

            bResult = List::GetMemberInvoker(source, "Clear", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Clear failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Clear should return True",
                LINE_INFO()
            );

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Count failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Count should be zero",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Contains method.
        //

        TEST_METHOD(ListGetMemberContains1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Verify that each element is contained in the list.
            //

            PCHAR argument;
            for (auto & item : elements)
            {
                argument = const_cast<PCHAR>(item.c_str());
                bResult = List::GetMemberInvoker(source, "Contains", argument, dest);
                Assert::IsTrue(bResult,
                    L"GetMember Contains failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Int == 1,
                    L"GetMember Contains should return True",
                    LINE_INFO()
                );
            }

            //
            // Test Contains on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Contains", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Contains failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Contains should return False",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Contains method
        // on an empty list.
        //

        TEST_METHOD(ListGetMemberContains2)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Contains on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Contains", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Contains failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Contains should return False",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Index method.
        //

        TEST_METHOD(ListGetMemberIndex1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Verify that each element is contained in the list at the
            // same offset as in the array.
            //

            PCHAR argument;
            int index;

            index = 0;
            for (auto & item : elements)
            {
                argument = const_cast<PCHAR>(item.c_str());
                bResult = List::GetMemberInvoker(source, "Index", argument, dest);
                Assert::IsTrue(bResult,
                    L"GetMember Index failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Int == index,
                    L"GetMember Index result mismatch",
                    LINE_INFO()
                );

                //
                // Advance to the next element.
                //

                ++index;
            }

            //
            // Test Index on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Index", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Index failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == -1,
                L"GetMember Index should return -1",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Index method
        // on an empty list.
        //

        TEST_METHOD(ListGetMemberIndex2)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Index on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Index", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Index failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == -1,
                L"GetMember Index should return -1",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Item method.
        //

        TEST_METHOD(ListGetMemberItem1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve each item at the specified offset and compare
            // it to the same offset in the elements array.
            //

            PCHAR argument;
            std::string listItem;
            for (auto & item : {0, 1, 2, 3})
            {
                listItem = std::to_string(item);
                argument = const_cast<PCHAR>(listItem.c_str());
                bResult = List::GetMemberInvoker(source, "Item", argument, dest);
                Assert::IsTrue(bResult,
                    L"GetMember Item failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Ptr != nullptr,
                    L"GetMember Item null pointer returned",
                    LINE_INFO()
                );
                Assert::IsTrue(reinterpret_cast<const char *>(dest.Ptr) == elements[item],
                    L"GetMember Item result mismatch",
                    LINE_INFO()
                );
            }

            //
            // Test Item on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Item", "5", dest);
            Assert::IsTrue(bResult,
                L"GetMember Item failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Item should return False",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Item method
        // on an empty List.
        //

        TEST_METHOD(ListGetMemberItem2)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Item on an item we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Item", "5", dest);
            Assert::IsTrue(bResult,
                L"GetMember Item failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Item should return False",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where a sequence is inserted before the first entry.
        //

        TEST_METHOD(ListGetMemberInsert1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "X",
                "Y",
                "Z",
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the sequence X,Y,Z before the first element.
            // Since the collection has a zero origin, use zero as
            // the position.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,X,Y,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 7,
                L"List must have seven elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where a sequence is inserted in the middle of the list.
        //

        TEST_METHOD(ListGetMemberInsert2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "X",
                "Y",
                "Z",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the sequence X,Y,Z after the second element.
            // Since the collection has a zero origin, use two as
            // the position.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "2,X,Y,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 7,
                L"List must have seven elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where a sequence is inserted after the end of the list.
        //
        // Note: This operation should fail.
        //

        TEST_METHOD(ListGetMemberInsert3)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Inserting a sequence after the end of the list is an
            // append operation. This should fail.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "4,X,Y,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Insert should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values. That is,
            // the list should not have been modified.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where a sequence composed of a single item is inserted
        // before the first entry.
        //

        TEST_METHOD(ListGetMemberInsert4)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "X",
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the item X before the first element.  Since the collection has
            // a zero origin, use zero as the position.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,X", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 5,
                L"List must have five elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where an empty list is inserted into an existing list.
        //
        // Result: Existing list should contain a null string at the
        // point of insertion.
        //

        TEST_METHOD(ListGetMemberInsert5)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the item 
            //

            bResult = List::GetMemberInvoker(source, "Insert", "2,", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 5,
                L"List must have five elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where an index is specified and no sequence is provided.
        //
        // Note: The insert should fail.
        //

        TEST_METHOD(ListGetMemberInsert6)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert an empty sequence before the first entry.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Insert should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Insert
        // method where a list is inserted into an empty list.
        //
        // Result: List should be inserted at the start.
        //

        TEST_METHOD(ListGetMemberInsert7)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the sequence at the start of the empty list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,A,B,C,D", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Sort
        // method.
        //

        TEST_METHOD(ListGetMemberSort)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D",
                "X",
                "Y",
                "Z",
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Insert the sequence X,Y,Z before the first element.
            // Since the collection has a zero origin, use zero as
            // the position.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,X,Y,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Insert failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Insert should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 7,
                L"List must have seven elements",
                LINE_INFO()
            );

            //
            // Sort the list.
            //

            bResult = List::GetMemberInvoker(source, "Sort", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Sort failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Sort should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 7,
                L"List must have seven elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Reverse
        // method.
        //

        TEST_METHOD(ListGetMemberReverse)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "D",
                "C",
                "B",
                "A"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Reverse the list.
            //

            bResult = List::GetMemberInvoker(source, "Reverse", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Reverse failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Reverse should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Remove
        // method where an item is in the list.
        //

        TEST_METHOD(ListGetMemberRemove1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Remove the second element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "B", dest);
            Assert::IsTrue(bResult,
                L"GetMember Remove failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Remove should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Remove
        // method where an item is not in the list.
        //

        TEST_METHOD(ListGetMemberRemove2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Remove an element that does not exist in the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Remove failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Remove should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Remove
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberRemove3)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Remove an element that does not exist in the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "E", dest);
            Assert::IsTrue(bResult,
                L"GetMember Remove failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Remove should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 0,
                L"List must have zero elements",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using the first element -- index 0.
        //

        TEST_METHOD(ListGetMemberErase1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the first element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "0", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using the last element -- index 3.
        //

        TEST_METHOD(ListGetMemberErase2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the last element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "3", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using an element in the middle of the list.
        //

        TEST_METHOD(ListGetMemberErase3)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the third element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "2", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using an element not in the list.
        //

        TEST_METHOD(ListGetMemberErase4)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase an element past the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "4", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Erase should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using a negative offset to erase the last element.
        //

        TEST_METHOD(ListGetMemberErase5)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the last element using an offset from the end.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "-1", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using a negative offset to erase the first element.
        //

        TEST_METHOD(ListGetMemberErase6)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the first element using an offset from the end.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "-4", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using a negative offset to erase an element in the middle
        // of the list.
        //

        TEST_METHOD(ListGetMemberErase7)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase the second element using an offset from the end.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "-3", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Erase should return True",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method using a negative offset to erase an element before the
        // beginning of the list.
        //

        TEST_METHOD(ListGetMemberErase8)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase an element before the beginning using an offset from
            // the end.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "-5", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Erase should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Erase
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberErase9)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Erase an element before the beginning using an offset from
            // the end.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "0", dest);
            Assert::IsTrue(bResult,
                L"GetMember Erase failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Erase should return False",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 0,
                L"List must have zero elements",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Replace
        // method where an existing element is replaced with a new value.
        //

        TEST_METHOD(ListGetMemberReplace1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "Z",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Replace the first element with a new value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Replace should return one replacement",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Replace
        // method where an element that does not exist is replaced.
        //

        TEST_METHOD(ListGetMemberReplace2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Replace a value that is not in the list with a new value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "E,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Replace should return zero replacements",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Replace
        // method where a value is replaced multiple times.
        //

        TEST_METHOD(ListGetMemberReplace3)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "1",
                "B",
                "1",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Replace the first element with a new value and then the third
            // element with a new value.  Then replace both of those values
            // with the same value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Replace should return one replacement",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            bResult = List::GetMemberInvoker(source, "Replace", "C,Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Replace should return one replacement",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            bResult = List::GetMemberInvoker(source, "Replace", "Z,1", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 2,
                L"GetMember Replace should return one replacement",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Replace
        // method where the replacement string is a null string.
        //

        TEST_METHOD(ListGetMemberReplace4)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Replace a value that is not in the list with a new value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Replace should return one replacement",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 4,
                L"List must have four elements",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Replace
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberReplace5)
        {
            std::unique_ptr<List> pl(new List);
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Replace a value that is not in the list with a new value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,", dest);
            Assert::IsTrue(bResult,
                L"GetMember Replace failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Replace should return zero replacements",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 0,
                L"List must have zero elements",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Head
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberHead1)
        {
            std::unique_ptr<List> pl = std::make_unique<List>();
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve the head of the list.
            //

            bResult = List::GetMemberInvoker(source, "Head", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Head failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Head should be false",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 0,
                L"List must have zero elements",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Head
        // method to return the first member.
        //

        TEST_METHOD(ListGetMemberHead2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve the head of the list.
            //

            bResult = List::GetMemberInvoker(source, "Head", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Head failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Head should return an item",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the returned item is the former first item ("A").
            //

            Assert::IsTrue(std::string("A") == reinterpret_cast<const char *>(dest.Ptr),
                L"GetMember Head did not return expected value",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the Tail
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberTail1)
        {
            std::unique_ptr<List> pl = std::make_unique<List>();
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve the head of the list.
            //

            bResult = List::GetMemberInvoker(source, "Tail", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Tail failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Tail should be false",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 0,
                L"List must have zero elements",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the Tail
        // method on a list.
        //

        TEST_METHOD(ListGetMemberTail2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve the head of the list.
            //

            bResult = List::GetMemberInvoker(source, "Tail", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Tail failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Tail should return an item",
                LINE_INFO()
            );

            Assert::IsTrue(pl.get()->Count() == 3,
                L"List must have three elements",
                LINE_INFO()
            );

            //
            // Verify that the returned item is the former last item ("D").
            //

            Assert::IsTrue(std::string("D") == reinterpret_cast<const char *>(dest.Ptr),
                L"GetMember Head did not return expected value",
                LINE_INFO()
            );

            //
            // Verify that the output list has the expected values.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );
        }

        //
        // Test the results of the GetMember interface for the CountOf
        // method on an empty list.
        //

        TEST_METHOD(ListGetMemberCountOf1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = std::unique_ptr<List>(new List());

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Count the number of "A"'s in the empty list.  The result should be
            // zero.
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "A", dest);
            Assert::IsTrue(bResult,
                L"GetMember CountOf failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember CountOf should return zero",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the CountOf
        // method where the item does not occur in the list.
        //

        TEST_METHOD(ListGetMemberCountOf2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Count the number of "Z"'s in the empty list.  The result should be
            // zero.
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "Z", dest);
            Assert::IsTrue(bResult,
                L"GetMember CountOf failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember CountOf should return zero",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the CountOf
        // method where the item occurs once in the list.
        //

        TEST_METHOD(ListGetMemberCountOf3)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Count the number of "C"'s in the empty list.  The result should be
            // one.
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "A", dest);
            Assert::IsTrue(bResult,
                L"GetMember CountOf failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember CountOf should return one",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the CountOf
        // method where the item occurs twice in the list.
        //

        TEST_METHOD(ListGetMemberCountOf4)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Append a "C" to the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "C", dest);
            Assert::IsTrue(bResult,
                L"GetMember Append failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Append should return True",
                LINE_INFO()
            );

            //
            // Count the number of "C"'s in the empty list.  The result should be
            // two.
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "C", dest);
            Assert::IsTrue(bResult,
                L"GetMember CountOf failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 2,
                L"GetMember CountOf should return two",
                LINE_INFO()
            );
        }

        //
        // Test the results of the GetMember interface for the splice method
        // with one argument.  A positive argument represent an origin of the
        // splice to the end of the list.  A negative arguments represents an
        // offset from the end of the list.
        //
        // Result: a new list is returned from a given position.
        //

        TEST_METHOD(ListGetMemberSplice1)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at the first (0th) element.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0", dest);
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of original and splice must be the same.
            //

            Assert::IsTrue(splice->Count() == pl->Count(),
                L"Splice length must be equal to length of original list",
                LINE_INFO()
            );

            //
            // Verify that the original and returned list have the same
            // elements.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );

            CompareListToElements(
                *splice.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );

            //
            // Test splice returning an empty list.  Use an index beyond the
            // list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "5", dest);
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
            Assert::IsTrue(splice->Count() == 0,
                L"Splice must be empty",
                LINE_INFO()
            );

            //
            // Test splice from the end of the list, returning the entire list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "-4", dest);
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
            Assert::IsTrue(splice->Count() == pl->Count(),
                L"Splice length must be equal to length of original list",
                LINE_INFO()
            );

            //
            // Create forward splices from position zero to the length.
            //

            for (unsigned long index = 0; index < pl->Count(); ++index)
            {
                auto indexAsString = std::to_string(index);

                bResult = List::GetMemberInvoker(
                    source,
                    "Splice",
                    const_cast<PCHAR>(indexAsString.c_str()),
                    dest
                );
                Assert::IsTrue(bResult,
                    L"GetMember Splice failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Ptr != nullptr,
                    L"GetMember Splice should not return a nullptr",
                    LINE_INFO()
                );

                splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
                Assert::IsTrue(splice->Count() == (pl->Count() - index),
                    L"Splice must equal index length",
                    LINE_INFO()
                );
            }

            //
            // Create reverse splices from the back of the list to the start.
            //

            for (unsigned long index = 1; index <= pl->Count(); ++index)
            {
                auto indexAsString = std::to_string(-(long) index);

                bResult = List::GetMemberInvoker(
                    source,
                    "Splice",
                    const_cast<PCHAR>(indexAsString.c_str()),
                    dest
                );
                Assert::IsTrue(bResult,
                    L"GetMember Splice failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Ptr != nullptr,
                    L"GetMember Splice should not return a nullptr",
                    LINE_INFO()
                );

                splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
                Assert::IsTrue(splice->Count() == index,
                    L"Splice must equal index length",
                    LINE_INFO()
                );
            }
        }

        //
        // Test the results of the GetMember interface for the splice method
        // with two arguments.  The first argument is a starting position and
        // the second is an ending position.  If an argument is positive,
        // it represents an offset from the start of the list and if it is
        // negative it represents an offset from the end of the list.
        //
        // The ending index is an exclusive range, that is, it is not included
        // in the output list.
        //
        // Result: a new list is returned.
        //

        TEST_METHOD(ListGetMemberSplice2)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Return a splice of the list from the start to the end.
            //

            auto argAsString = "0," + std::to_string(pl->Count());
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of original and splice must be the same.
            //

            Assert::IsTrue(splice->Count() == pl->Count(),
                L"Splice length must be equal to length of original list",
                LINE_INFO()
            );

            //
            // Verify that the original and returned list have the same
            // elements.
            //

            CompareListToElements(
                *pl.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );

            CompareListToElements(
                *splice.get(),
                elements,
                sizeof(elements) / sizeof(elements[0])
            );

            //
            // Test splice returning an empty list.  Use an index beyond the
            // list.
            //

            argAsString = std::to_string(pl->Count() + 1) + std::to_string(pl->Count());
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
            Assert::IsTrue(splice->Count() == 0,
                L"Splice must be empty",
                LINE_INFO()
            );

            //
            // Test splice from the end of the list, returning the list
            // exclusive of the last element.
            //

            argAsString = std::to_string(-((long) pl->Count())) + ",-1";
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
            Assert::IsTrue(splice->Count() == pl->Count() - 1,
                L"Splice length must be one shorter than the length of original list",
                LINE_INFO()
            );

            //
            // Create forward splices from the start to the next to last
            // position.
            //

            for (unsigned long index = 0; index <= pl->Count() - 1; ++index)
            {
                argAsString = std::to_string(index) + ",-1";
                bResult = List::GetMemberInvoker(
                    source,
                    "Splice",
                    const_cast<PCHAR>(argAsString.c_str()),
                    dest
                );
                Assert::IsTrue(bResult,
                    L"GetMember Splice failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Ptr != nullptr,
                    L"GetMember Splice should not return a nullptr",
                    LINE_INFO()
                );

                splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));
                Assert::IsTrue(splice->Count() == (pl->Count() - index) - 1,
                    L"Splice must equal index length",
                    LINE_INFO()
                );
            }

            //
            // Create reverse splices from the back of the list to the start.
            //

            //
            // Use the std::max templated function, not the macro.
            //

#if defined(max)
#pragma push_macro("max")
#undef max
#define PUSHED_MAX
#endif

            long upperBound;
            long lowerBound;

            upperBound = pl->Count() - 1;
            for (unsigned long index = 0; index <= pl->Count(); ++index)
            {
                argAsString = std::to_string(-((long) index)) + ",-1";
                bResult = List::GetMemberInvoker(
                    source,
                    "Splice",
                    const_cast<PCHAR>(argAsString.c_str()),
                    dest
                );
                Assert::IsTrue(bResult,
                    L"GetMember Splice failed",
                    LINE_INFO()
                );
                Assert::IsTrue(dest.Ptr != nullptr,
                    L"GetMember Splice should not return a nullptr",
                    LINE_INFO()
                );

                splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));

                //
                // Compute the expected length of the splice.  If bounds are
                // negative, the position is the length - bound.
                //

                lowerBound = -(long) index;
                if (lowerBound < 0)
                {
                    lowerBound = (long) pl->Count() + lowerBound;
                }

                Assert::IsTrue(splice->Count() == std::max(
                    0L,
                    upperBound - lowerBound
                ),
                    L"Splice must equal index length",
                    LINE_INFO()
                );

                //
                // If we undefined the macro, make it visible again.
                //

#if defined(PUSHED_MAX)
#pragma pop_macro("max")
#undef PUSHED_MAX
#endif

            }
        }

        //
        // Test the list splice method with three arguments.  This is like
        // ListSplice2 except the third argument is a 'stride' which is how
        // many elements to skip.
        //
        // Result: a new list is returned.
        //

        TEST_METHOD(ListGetMemberSplice3)
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            std::string firstthird[] = {
                "A",
                "C"
            };
            std::string elements[] = {
                "A",
                "B",
                "C",
                "D"
            };

            //
            // Create a new list.
            //

            pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Return a splice of the list from the start to the end, skipping
            // no elements.
            //

            auto argAsString = "0," + std::to_string(pl->Count()) + ",1";
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of original and splice must be the same.
            //

            Assert::IsTrue(splice->Count() == pl->Count(),
                L"Splice length must be equal to length of original list",
                LINE_INFO()
            );

            //
            // Return a splice with a stride of 0.  This should return an
            // empty list.
            //

            argAsString = "0," + std::to_string(pl->Count()) + ",0";
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));

            Assert::IsTrue(splice->Count() == 0,
                L"Splice length must be zero",
                LINE_INFO()
            );

            //
            // Test stride of 2.  This should return a list half the length
            // of the source list.
            //

            argAsString = "0," + std::to_string(pl->Count()) + ",2";
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));

            Assert::IsTrue(splice->Count() == pl->Count() / 2,
                L"Splice must be half the length of the original",
                LINE_INFO()
            );

            //
            // Test for the 1st and 3rd elements.
            //

            CompareListToElements(
                *splice,
                firstthird,
                sizeof(firstthird) / sizeof(firstthird[0])
            );

            //
            // Test a stride longer than the source list.  The first element
            // should be returned.
            //

            argAsString = "0," + std::to_string(pl->Count()) + "," + std::to_string(pl->Count() + 1);
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));

            Assert::IsTrue(splice->Count() == 1,
                L"Splice must contain only one element",
                LINE_INFO()
            );

            const std::string *pitem;
            Assert::IsTrue(splice->Item(0, &pitem),
                L"Could not return item",
                LINE_INFO()
            );

            Assert::IsTrue(*pitem == elements[0],
                L"Items in splice are not equal",
                LINE_INFO()
            );

            //
            // Return a slice from the end by -3.  This should return the
            // last and third elements.
            //

            argAsString = "0," + std::to_string(pl->Count()) + ",-3";
            bResult = List::GetMemberInvoker(
                source,
                "Splice",
                const_cast<PCHAR>(argAsString.c_str()),
                dest
            );
            Assert::IsTrue(bResult,
                L"GetMember Splice failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Ptr != nullptr,
                L"GetMember Splice should not return a nullptr",
                LINE_INFO()
            );

            splice = std::unique_ptr<List>(reinterpret_cast<List *>(dest.Ptr));

            Assert::IsTrue(splice->Count() == pl->Count() / 3,
                L"Splice must be a third of the list long",
                LINE_INFO()
            );
        }

        ***/

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

        //
        // Create a list and append the elements using the GetMember interface.
        //

        std::unique_ptr<List> CreateAndAppendUsingGetMember() const
        {
            std::unique_ptr<List> pl;
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            pl = std::unique_ptr<List>(new List());

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Count failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 0,
                L"GetMember Count should be zero",
                LINE_INFO()
            );

            //
            // Append a single item to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A", dest);
            Assert::IsTrue(bResult,
                L"GetMember Append failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Append should return True",
                LINE_INFO()
            );

            //
            // Append a sequence of items to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "B,C,D", dest);
            Assert::IsTrue(bResult,
                L"GetMember Append failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 1,
                L"GetMember Append should return True",
                LINE_INFO()
            );

            //
            // Count should return 4.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult,
                L"GetMember Count failed",
                LINE_INFO()
            );
            Assert::IsTrue(dest.Int == 4,
                L"GetMember Count should be 4",
                LINE_INFO()
            );

            return pl;
        }

        //
        // Use the iterator to access each of the elements in the
        // list.
        //

        template<typename T>
        void ValidateAndUseIterator(
            const T & iter,
            const std::string items[],
            size_t elements,
            size_t currentItem
        ) const
        {
            //
            // We should not be at the end unless the currentItem is the
            // last item.
            //

            if (iter.get()->IsEnd())
            {
                Assert::IsTrue(currentItem == elements,
                    L"IsEnd succeeded and we are not at end",
                    LINE_INFO()
                );
            }
            else
            {
                Assert::IsTrue(currentItem != elements,
                    L"IsEnd failed and we are at end",
                    LINE_INFO()
                );
            }

            //
            // Process each of the elements, comparing against the items.
            //

            const std::string *item;
            while (!iter.get()->IsEnd())
            {
                //
                // Retrieve the current element and compare it to
                // the current position.
                //

                Assert::IsTrue(iter.get()->Value(&item),
                    L"Value failed for iterator",
                    LINE_INFO()
                );

                Assert::IsTrue(*item == items[currentItem],
                    L"item should be equal to item under current position",
                    LINE_INFO()
                );

                //
                // Advance the iterator.  This should succeed.
                //

                Assert::IsTrue(iter.get()->Advance(),
                    L"Advance failed",
                    LINE_INFO()
                );

                //
                // Advance the current position.
                //

                ++currentItem;
            }
        }
    };
}