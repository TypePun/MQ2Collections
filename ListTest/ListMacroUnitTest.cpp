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
    // Test List Operations (Creation, Count, Clear, Contains, Splice, Index, Item,
    // Insert, Sort, Reverse, Append, Remove, Erase, Replace, First, Find, Head, Tail and
    // CountOf.
    //

    TEST_CLASS(ListMacroInterfaceUnitTests)
    {
    public:

        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"List")
            TEST_CLASS_ATTRIBUTE(L"MQ2Interface", L"List")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Call the Count method on an empty list.
        //
        // Result: count should return zero.
        //

        TEST_METHOD(CountOnEmptyList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed.");
            Assert::AreEqual(dest.Int, 0, L"Count should be zero.");
        }

        //
        // Call the Count method on a a populated list.
        //
        // Result: count should return five.
        //

        TEST_METHOD(CountOnPopulatedList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed.");
            Assert::AreEqual(dest.Int, 5, L"Count should be zero.");
        }

        //
        // Call the Clear method on an empty list.
        //
        // Result: clear should return succeed.
        //

        TEST_METHOD(ClearEmptyList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Clear should succeed.
            //

            bResult = List::GetMemberInvoker(source, "Clear", nullptr, dest);
            Assert::IsTrue(bResult, L"Clear invocation failed.");
            Assert::AreEqual(dest.Int, 1, L"Clear should succeed.");
        }

        //
        // Call Clear on a populated list.
        //
        // Result: list should have a count of zero.
        //

        TEST_METHOD(ClearList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Clear the list.
            //

            bResult = List::GetMemberInvoker(source, "Clear", nullptr, dest);
            Assert::IsTrue(bResult, L"Clear invocation failed.");
            Assert::AreEqual(dest.Int, 1, L"Clear should return true.");

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed");
            Assert::AreEqual(dest.Int, 0, L"Count should be zero");
        }

        //
        // Create a list, populate it and verify it contains a known element.
        //
        // Result: the list should contain the known element
        //

        TEST_METHOD(ContainsAnElement)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Contains on an item we know is in the list.
            //

            bResult = List::GetMemberInvoker(source, "Contains", "C", dest);
            Assert::IsTrue(bResult, L"Contains invocation failed.");
            Assert::AreEqual(dest.Int, 1, L"Contains should return true.");
        }

        //
        // Create an empty list and verify it does not contain an element.
        //
        // Result: the list should not contain the element.
        //

        TEST_METHOD(EmptyListDoesNotContainAnElement)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = std::make_unique<List>();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Contains on an item we know can not be in the list.
            //

            bResult = List::GetMemberInvoker(source, "Contains", "A", dest);
            Assert::IsTrue(bResult, L"Contains invocation failed.");
            Assert::AreEqual(dest.Int, 0, L"Contains should return false.");
        }

        //
        // Create a list, populate it and verify it does not contains a known element.
        //
        // Result: the list should not contain the element.
        //

        TEST_METHOD(DoesNotContainsAnElement)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Test Contains on an item we know can not be in the list.
            //

            bResult = List::GetMemberInvoker(source, "Contains", "F", dest);
            Assert::IsTrue(bResult, L"Contains invocation failed.");
            Assert::AreEqual(dest.Int, 0, L"Contains should return false.");
        }

        //
        // Create an empty list and return a splice from the zeroth position.
        //
        // Result: the splice should contain zero elements.
        //

        TEST_METHOD(EmptySpliceWithZeroIndex)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at the first (0th) element.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create a list and retrieve a splice from the end
        //
        // Result: the splice should have no elements.
        //

        TEST_METHOD(SpliceFromEnd)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting past the end of the list -- chose the 100th
            // element.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "100", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create an empty list and return a splice from a large position
        //
        // Result: the splice should contain zero elements.
        //

        TEST_METHOD(EmptySpliceWithLargeIndex)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting past the end of the list -- chose the 100th
            // element.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "100", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create a list and retrieve a splice from the start.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStart)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting past the origin of the list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list and splice must be the same.
            //

            Assert::AreEqual(pl->Count(), splice->Count(), L"Length of original list and splice must be the same.");

            CompareListAndSplice(*pl, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle.
        //
        // Result: the splice should 1/2 the length long.
        //

        TEST_METHOD(SpliceFromMiddle)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting past the origin of the list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "2", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            size_t splice_length = (size_t) std::ceil(pl->Count() / 2.0);

            //
            // Check the length of the splice.
            //

            Assert::AreEqual(splice_length, splice->Count(), L"Splice length is incorrect.");

            CompareListAndSplice(*pl, pl->Count() - splice_length, splice, 0);
        }

        //
        // Create an empty list and splice from the start with a zero length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithZeroLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at the first (0th) element with a length
            // of zero.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0,0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create an empty list and splice from the start with a positive length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at the first (0th) element with a length
            // of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0,1", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create an empty list and splice from a positive offset and a zero length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveOffsetAndZeroLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at position 10 with a length of zero.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "10,0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }

        //
        // Create an empty list and splice from a positive offset and a positive length.
        //
        // Result: a zero length splice should be returned.
        //

        TEST_METHOD(EmptySpliceWithPositiveOffsetAndPositiveLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Make a new List.
            //

            auto pl = std::make_unique<List>();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at position 10 with a length of 10.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "10,10", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Length of returned list must be zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Length of returned splice must be zero.");
        }
        //
        // Create a list and retrieve a splice from the start with a length
        // longer than the list.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStartPastTheEnd)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting at the origin past the length of the list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0, 100", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Check the length of the splice.
            //

            Assert::AreEqual(pl->Count(), splice->Count(), L"Splice and original list must be of the same length.");

            CompareListAndSplice(*pl, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the start with a length
        // equal to the list.
        //
        // Result: the splice should be the same length as the original list.
        //

        TEST_METHOD(SpliceFromStartForLengthOfList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting from the origin for the length of the list.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0, 5", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Check the length of the splice.
            //

            Assert::AreEqual(pl->Count(), splice->Count(), L"Splice and original list must be of the same length.");

            CompareListAndSplice(*pl, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the start with a length
        // of zero.
        //
        // Result: the splice should have a length of zero.
        //

        TEST_METHOD(SpliceFromStartWithZeroLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting from the origin for a length of zero.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0, 0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must have a length of zero.");
        }

        //
        // Create a list and retrieve a splice from the start with a length of one.
        //
        // Result: the splice should have a length of one.
        //

        TEST_METHOD(SpliceFromStartForLengthOfOne)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice starting from the origin for a length of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "0, 1", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(*pl, 0, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the end for a length of one.
        //
        // Result: the splice should have a length of one.
        //

        TEST_METHOD(SpliceFromEndForLengthOfOne)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the last element for a length of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "4, 1", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(*pl, 4, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the end for a length of zero.
        //
        // Result: the splice should have a length of zero.
        //

        TEST_METHOD(SpliceFromEndForLengthOfZero)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the end for a length of zero.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "4, 0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of zero.
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
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the last element for a large length.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "4, 100", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(*pl, 4, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle with a length of one.
        //
        // Result: the splice should of length one.
        //

        TEST_METHOD(SpliceFromMiddleWithLengthOfOne)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the middle for a length of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "2, 1", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of one.
            //

            Assert::AreEqual(splice->Count(), (size_t) 1, L"Splice must have a length of one.");

            CompareListAndSplice(*pl, 2, splice, 0);
        }

        //
        // Create a list and retrieve a splice from the middle with a length of zero,
        //
        // Result: the splice should of length zero.
        //

        TEST_METHOD(SpliceFromMiddleWithLengthOfZero)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the middle for a length of zero.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "2, 0", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of zero.
            //

            Assert::AreEqual(splice->Count(), (size_t) 0, L"Splice must have a length of zero.");
        }

        //
        // Create a list and retrieve a splice from the middle with a length of the
        // remainder.
        //
        // Result: the splice should contain half the elements.
        //

        TEST_METHOD(SpliceFromMiddleForRemainder)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the middle for a length of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "2, 3", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of three.
            //

            Assert::AreEqual(splice->Count(), (size_t) 3, L"Splice must have a length of three.");

            CompareListAndSplice(*pl, 2, splice, 0);
        }
        //

        // Create a list and retrieve a splice from the middle with a large length.
        //
        // Result: the splice should contain half the elements.
        //

        TEST_METHOD(SpliceFromMiddleForLargeLength)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Retrieve a splice from the middle for a length of one.
            //

            bResult = List::GetMemberInvoker(source, "Splice", "2, 100", dest);
            Assert::IsTrue(bResult, L"Splice invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Splice should not return a nullptr.");

            std::unique_ptr<List> splice(reinterpret_cast<List *>(dest.Ptr));

            //
            // Return splice must have a length of three.
            //

            Assert::AreEqual(splice->Count(), (size_t) 3, L"Splice must have a length of three.");

            CompareListAndSplice(*pl, 2, splice, 0);
        }

        /***

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
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = std::make_unique<List>();

            //
            // Initialize the source pointer for this member call.
            //

            source.Ptr = pl.get();

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed.");
            Assert::AreEqual(dest.Int, 0, L"Count should be zero.");

            //
            // Append a single item to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A", dest);
            Assert::IsTrue(bResult, L"Append invocation failed");
            Assert::AreEqual(dest.Int, 1, L"Append should return True");

            //
            // Append a sequence of items to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "B,C,D,E", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(dest.Int, 1, L"Append should return True");

            //
            // Count should return 5.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed");
            Assert::AreEqual(dest.Int, 5, L"GetMember Count should be five");

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