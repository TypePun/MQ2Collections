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
            Assert::AreEqual(0, dest.Int, L"Count should be zero.");
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
            Assert::AreEqual(5, dest.Int, L"Count should be zero.");
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
            Assert::AreEqual(1, dest.Int, L"Clear should succeed.");
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
            Assert::AreEqual(1, dest.Int, L"Clear should return true.");

            //
            // Count should return 0.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed");
            Assert::AreEqual(0, dest.Int, L"Count should be zero");
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
            Assert::AreEqual(1, dest.Int, L"Contains should return true.");
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
            Assert::AreEqual(0, dest.Int, L"Contains should return false.");
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
            Assert::AreEqual(0, dest.Int, L"Contains should return false.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Length of returned splice must be zero.");
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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Splice must have a length of zero.");
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

            Assert::AreEqual((size_t) 1, splice->Count(), L"Splice must have a length of one.");

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

            Assert::AreEqual((size_t) 1, splice->Count(), L"Splice must have a length of one.");

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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Splice must have a length of zero.");
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

            Assert::AreEqual((size_t) 1, splice->Count(), L"Splice must have a length of one.");

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

            Assert::AreEqual((size_t) 1, splice->Count(), L"Splice must have a length of one.");

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

            Assert::AreEqual((size_t) 0, splice->Count(), L"Splice must have a length of zero.");
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

            Assert::AreEqual((size_t) 3, splice->Count(),L"Splice must have a length of three.");

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

            Assert::AreEqual((size_t) 3, splice->Count(), L"Splice must have a length of three.");

            CompareListAndSplice(*pl, 2, splice, 0);
        }

        //
        // Test the list index method on an empty list.
        //
        // Result: -1 should be returned.
        //

        TEST_METHOD(IndexOnEmptyList)
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
            // Index of 'Zero' should be -1, indicating it was not found.
            //

            bResult = List::GetMemberInvoker(source, "Index", "Zero", dest);
            Assert::IsTrue(bResult, L"Index invocation failed.");
            Assert::AreEqual(-1, dest.Int, L"Index of 'Zero' should be -1.");
        }

        //
        // Test the list index method on an empty list for a quoted string.
        //
        // Result: -1 should be returned.
        //

        TEST_METHOD(IndexQuotedItemOnEmptyList)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Index of 'Zero' should be -1, indicating it was not found.
            //

            bResult = List::GetMemberInvoker(source, "Index", "\"Zero, Item\"", dest);
            Assert::IsTrue(bResult, L"Index invocation failed.");
            Assert::AreEqual(-1, dest.Int, L"Index of 'Zero, Item' should be -1.");
        }

        //
        // Test the list index method. An index is the zero-based position of
        // an item in the list.
        //
        // Result: return the index of an item in a list.
        //

        TEST_METHOD(IndexForKnownElements)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Index each element in the list.  It should succeed.
            //

            for (long index = 0; index < sizeof(elements) / sizeof(elements[0]); ++index)
            {
                bResult = List::GetMemberInvoker(source, "Index", elements[index], dest);
                Assert::IsTrue(bResult, L"Index invocation failed.");
                Assert::AreEqual(index, (long) dest.Int, L"Index is incorrect for element.");
            }
        }

        //
        // Test the list index method on quoted items. An index is the zero-based position of
        // an item in the list.
        //
        // Result: return the index of an item in a list.
        //

        TEST_METHOD(IndexForKnownQuotedElements)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C, is an item",
                "D",
                "E has a quoted quote\" and comma (,)"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            bool bResult;

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendQuotedItemsUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Index each element in the list.  It should succeed.
            //

            for (long index = 0; index < sizeof(elements) / sizeof(elements[0]); ++index)
            {
                bResult = List::GetMemberInvoker(source, "Index", elements[index], dest);
                Assert::IsTrue(bResult, L"Index invocation failed.");
                Assert::AreEqual(index, (long)dest.Int, L"Index is incorrect for element.");
            }
        }

        //
        // Test the list index method for an element not in the list.
        //
        // Result: -1 should be returned.
        //

        TEST_METHOD(IndexForUnknownElement)
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
            // Index of 'Zero' should be -1, indicating it was not found.
            //

            bResult = List::GetMemberInvoker(source, "Index", "Zero", dest);
            Assert::IsTrue(bResult, L"Index invocation failed.");
            Assert::AreEqual(-1, dest.Int, L"Index of 'Zero' should be -1.");
        }

        //
        // Retrieve the item at position zero in an empty list.
        //
        // Result: the item method should return false.
        //

        TEST_METHOD(ItemEmptyList)
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
            // Index of 'Zero' should be -1, indicating it was not found.
            //

            bResult = List::GetMemberInvoker(source, "Item", "0", dest);
            Assert::IsTrue(bResult, L"Item invocation failed.");
            Assert::IsNull(dest.Ptr, L"Returned pointer should be null.");
        }

        //
        // Retrieve each item in a populated list.
        //
        // Result: the items should be returned in order.
        //

        TEST_METHOD(ItemCompareEachEntry)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Compare the list to the expected elements.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Retrieve an item after the end of a list.
        //
        // Result: the items should be returned in order.
        //

        TEST_METHOD(ItemAfterListEnd)
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
            // Get the item after the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Item", "100", dest);
            Assert::IsTrue(bResult, L"Item invocation failed.");
            Assert::AreEqual(0, dest.Int,L"Item method should return false.");
        }

        //
        // Insert an empty list into an empty list.
        //
        // Result: the list should be empty.
        //

        TEST_METHOD(InsertEmptyListIntoEmptyList)
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
            // Insert an empty sequence at the start of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Insert an empty list at the start of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListAtStartOfList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert an empty sequence at the start of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert an empty list at the end of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListAtEndOfList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert an empty sequence at the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "5,", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert an empty list in the middle of a list.
        //
        // Result: existing list should be the same.
        //

        TEST_METHOD(InsertEmptyListInMiddleOfList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert an empty sequence in the middle of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "3,", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list at the start of another list.
        //
        // Result: new list is prepended to the old list.
        //

        TEST_METHOD(InsertListAtStartOfAnotherList)
        {
            PCHAR elements[] =
            {
                "One",
                "Two",
                "Three",
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert a sequence at the start of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,One,Two,Three", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list with quoted items at the start of another list.
        //
        // Result: new list is prepended to the old list.
        //

        TEST_METHOD(InsertListWithQuotedItemsAtStartOfAnotherList)
        {
            PCHAR elements[] =
            {
                "One",
                "Two, with a comma",
                "Three with a \" quote!",
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Insert a sequence at the start of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,One,\"Two, with a comma\",\"Three with a \\\" quote!\"", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list at the end of another list.
        //
        // Result: new list is appended to the old list.
        //

        TEST_METHOD(InsertListAtEndOfAnotherList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two",
                "Three"
            };

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
            // Insert a sequence at the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "5,One,Two,Three", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list with quoted items at the end of another list.
        //
        // Result: new list is appended to the old list.
        //

        TEST_METHOD(InsertListWithQuotedItemsAtEndOfAnotherList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two, with a comma",
                "Three with a \" quote!"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Insert a sequence at the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "5,One,\"Two, with a comma\",\"Three with a \\\" quote!\"", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list in the middle of another list.
        //
        // Result: the new list is composed of first list with second list
        // inserted in the middle of the first list.
        //

        TEST_METHOD(InsertListInMiddleOfAnotherList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "One",
                "Two",
                "Three",
                "D",
                "E"
            };

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
            // Insert a sequence in the middle of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "3,One,Two,Three", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list with quoted items in the middle of another list.
        //
        // Result: the new list is composed of first list with second list
        // inserted in the middle of the first list.
        //

        TEST_METHOD(InsertListWithQuotedItemsInMiddleOfAnotherList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "One",
                "Two, with a comma",
                "Three with a \" quote!",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Insert a sequence in the middle of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "3,One,\"Two, with a comma\",\"Three with a \\\" quote!\"", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list at in invalid entry past the end of the list.
        //
        // Result: list should not be modified.
        //

        TEST_METHOD(InsertPastEnd)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert a sequence past the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "6,One,Two,Three", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Insert should return false.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list with quoted elements at in invalid entry past the end of the list.
        //
        // Result: list should not be modified.
        //

        TEST_METHOD(InsertWithQuotedItemsPastEnd)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Insert a sequence past the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "6,One,\"Two, has a comma\",\"Three has a quote \\\"\"", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Insert should return false.");

            Assert::AreEqual((size_t)5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list into an empty list.
        //
        // Result: list should contain the inserted elements.
        //

        TEST_METHOD(InsertIntoEmptyList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Insert a sequence past the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,A,B,C,D,E", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Insert a list with quoted items into an empty list.
        //
        // Result: list should contain the inserted elements.
        //

        TEST_METHOD(InsertWithQuotedItemsIntoEmptyList)
        {
            PCHAR elements[] =
            {
                "A, 1, 2, 3, 4",
                "B, \'\" quote! @",
                "C",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Insert a sequence past the end of the list.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,\"A, 1, 2, 3, 4\",\"B, \\\'\\\" quote! @\",C,D,E", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t)5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Sort an empty list.
        //
        // Result: empty list should be returned.
        //

        TEST_METHOD(SortEmptyList)
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
            // Sort the empty list.
            //

            bResult = List::GetMemberInvoker(source, "Sort", nullptr, dest);
            Assert::IsTrue(bResult, L"Sort invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Sort should return true.");

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Sort the list.
        //
        // Result: list should be sorted.
        //

        TEST_METHOD(SortList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "F",
                "G",
                "H"
            };

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
            // Insert larger elements at the front.
            //

            bResult = List::GetMemberInvoker(source, "Insert", "0,F,G,H", dest);
            Assert::IsTrue(bResult, L"Insert invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Insert should return true.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Sort the list.
            //

            bResult = List::GetMemberInvoker(source, "Sort", nullptr, dest);
            Assert::IsTrue(bResult, L"Sort invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Sort should return true.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Reverse an empty list.
        //
        // Result: an empty list should be returned.
        //

        TEST_METHOD(ReverseAnEmptyList)
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
            // Reverse the empty list.
            //

            bResult = List::GetMemberInvoker(source, "Reverse", nullptr, dest);
            Assert::IsTrue(bResult, L"Reverse invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Reverse should return true.");

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Reverse the list.
        //
        // Result: list should be reversed.
        //

        TEST_METHOD(ReverseList)
        {
            PCHAR elements[] =
            {
                "E",
                "D",
                "C",
                "B",
                "A"
            };

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
            // Reverse the list.
            //

            bResult = List::GetMemberInvoker(source, "Reverse", nullptr, dest);
            Assert::IsTrue(bResult, L"Reverse invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Reverse should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append an item to an empty list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToEmptyList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Append a sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A,B,C,D,E", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append an item to an empty list with a non-default delimiter.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToEmptyListWithCustomDelimiter)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR old_delim = { 0 };
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
            // Change the delimiter for the list to a "|".
            //

            bResult = List::GetMemberInvoker(source, "Delimiter", "|", old_delim);
            Assert::IsTrue(bResult, L"Delimiter invocation failed.");

            //
            // Append a sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A|B|C|D|E", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append quoted items to an empty list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendQuotedItemsToEmptyList)
        {
            PCHAR elements[] =
            {
                "A",
                "B \"",
                "C with a single \' quote",
                "D, has some commas, right",
                "E has quotes \"and\" commas 1,2,3"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Append a sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "\"A\",\"B \\\"\",\"C with a single \\\' quote\",\"D, has some commas, right\",\"E has quotes \\\"and\\\" commas 1,2,3\"", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append quoted items to an empty list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendQuotedItemsToEmptyListWithCustomDelimiter)
        {
            PCHAR elements[] =
            {
                "A",
                "B \"",
                "C with a single \' quote",
                "D, has some commas, right",
                "E has quotes \"and\" bars |||"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR old_delim = { 0 };
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
            // Change the delimiter for the list to a "|".
            //

            bResult = List::GetMemberInvoker(source, "Delimiter", "|", old_delim);
            Assert::IsTrue(bResult, L"Delimiter invocation failed.");

            //
            // Append a sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "\"A\"|\"B \\\"\"|\"C with a single \\\' quote\"|\"D, has some commas, right\"|\"E has quotes \\\"and\\\" bars |||\"", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append an item to a populated list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two",
                "Three"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Append a sequence onto the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "One,Two,Three", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append an item to a populated list with a custom delimiter.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendToListWithCustomDelimiter)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two",
                "Three"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR old_delim = { 0 };
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
            // Change the delimiter for the list to a "|".
            //

            bResult = List::GetMemberInvoker(source, "Delimiter", "|", old_delim);
            Assert::IsTrue(bResult, L"Delimiter invocation failed.");

            //
            // Append a sequence onto the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "One|Two|Three", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append quoted items to a populated list.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendQuotedItemsToList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two, with a comma",
                "Three with a \" quote!"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            // Append a sequence onto the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "\"One\",\"Two, with a comma\",\"Three with a \\\" quote!\"", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append quoted items to a populated list with a custom delimiter.
        //
        // Result: new elements should be added to the list.
        //

        TEST_METHOD(AppendQuotedItemsToListWithCustomDelimiter)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E",
                "One",
                "Two, with a comma",
                "Three with a \" quote!"
            };

            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR old_delim = { 0 };
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
            // Change the delimiter for the list to a "|".
            //

            bResult = List::GetMemberInvoker(source, "Delimiter", "|", old_delim);
            Assert::IsTrue(bResult, L"Delimiter invocation failed.");

            //
            // Append a sequence onto the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "\"One\"|\"Two, with a comma\"|\"Three with a \\\" quote!\"", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return true.");

            Assert::AreEqual((size_t)8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Append an empty sequence to an empty list.
        //
        // Result: no element should be added to the list.
        //

        TEST_METHOD(AppendEmptySequenceToEmptyList)
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
            // Append an empty sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Append should return false.");

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Append an empty sequence to an empty list.
        //
        // Result: no element should be added to the list.
        //

        TEST_METHOD(AppendEmptySequenceToEmptyListWithCustomDelimiter)
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR old_delim = { 0 };
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
            // Change the delimiter for the list to a "|".
            //

            bResult = List::GetMemberInvoker(source, "Delimiter", "|", old_delim);
            Assert::IsTrue(bResult, L"Delimiter invocation failed.");

            //
            // Append an empty sequence onto an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Append should return false.");

            Assert::AreEqual((size_t)0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Remove an item from an empty list.
        //
        // Result: remove should return zero items removed.
        //

        TEST_METHOD(RemoveFromEmptyList)
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
            // Remove an item from an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "A", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Remove should return zero items removed.");

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must have zero elements.");
        }

        //
        // Remove an item that is not in the list.
        //
        // Result: No items should be remvoed and the list should not be modified.
        //

        TEST_METHOD(RemoveItemNotInList)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Remove an element not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "Zero", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Remove should return zero elements removed.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Remove the first element from the list.
        //
        // Result: only the first item should be removed.
        //

        TEST_METHOD(RemoveFirstItem)
        {
            PCHAR elements[] =
            {
                "B",
                "C",
                "D",
                "E"
            };

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
            // Remove the first element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "A", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Remove should return one element removed.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Remove the last element from the list.
        //
        // Result: only the last item should be removed.
        //

        TEST_METHOD(RemoveLastItem)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "C",
                "D"
            };

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
            // Remove the last element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "E", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Remove should return one element removed.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Remove a middle element from the list.
        //
        // Result: only the middle item should be removed.
        //

        TEST_METHOD(RemoveMiddleItem)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "D",
                "E"
            };

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
            // Remove the middle element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "C", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Remove should return one element removed.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Duplicate a list and remove an element.
        //
        // Result: removed item should not be in the list and the remove
        // count should be two.
        //

        TEST_METHOD(RemoveMultiple)
        {
            PCHAR elements[] =
            {
                "A",
                "B",
                "D",
                "E",
                "A",
                "B",
                "D",
                "E"
            };
            PCHAR appenditems[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Append a new sequence to the list.
            //

            for (auto item : appenditems)
            {
                bResult = List::GetMemberInvoker(source, "Append", item, dest);
                Assert::IsTrue(bResult, L"Append invocation failed.");
                Assert::AreEqual(1, dest.Int, L"Append should return true.");
            }

            //
            // Remove the middle element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Remove", "C", dest);
            Assert::IsTrue(bResult, L"Remove invocation failed.");
            Assert::AreEqual(2, dest.Int, L"Remove should return two elements removed.");

            Assert::AreEqual((size_t) 8, pl->Count(), L"List must have eight elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Erase an item from an empty list.
        //
        // Result: erase should return false.
        //

        TEST_METHOD(EraseFromEmptyList)
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
            // Remove an item from an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "0", dest);
            Assert::IsTrue(bResult, L"Erase invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Erase should return false.");
        }

        //
        // Erase the first element.
        //
        // Result: erase should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseFirstItem)
        {
            PSTR elements[] =
            {
                "B",
                "C",
                "D",
                "E"
            };
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
            // Erase the first element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "0", dest);
            Assert::IsTrue(bResult, L"Erase invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Erase should return true.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Erasing a middle element.
        //
        // Result: erase should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseMiddleItem)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "D",
                "E"
            };
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
            // Erase the middle element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "2", dest);
            Assert::IsTrue(bResult, L"Erase invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Erase should return true.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Erasing the last element.
        //
        // Result: erase method should succeed and the erased item should not be in the list.
        //

        TEST_METHOD(EraseLastItem)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D"
            };
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
            // Erase the middle element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "4", dest);
            Assert::IsTrue(bResult, L"Erase invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Erase should return true.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Erase an item beyond the list.
        //
        // Result: erase method should fail and the list should not be modified.
        //

        TEST_METHOD(EraseItemOutOfBounds)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };
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
            // Erase the middle element from the list.
            //

            bResult = List::GetMemberInvoker(source, "Erase", "5", dest);
            Assert::IsTrue(bResult, L"Erase invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Erase should return false.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Replace an item in an empty list.
        //
        // Result: count of items replaced should be zero.
        //

        TEST_METHOD(ReplaceInEmptyList)
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
            // Replace an item from an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,Empty", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Replace should return zero.");
        }

        //
        // Replace the first element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceFirstElement)
        {
            PSTR elements[] =
            {
                "First",
                "B",
                "C",
                "D",
                "E"
            };
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
            // Replace the first element in the list with another value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "A,First", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Replace should return one entry replaced.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Replace the last element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceLastElement)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "Last"
            };
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
            // Replace the last element in the list with another value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "E,Last", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Replace should return one entry replaced.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Replace a middle element.
        //
        // Result: replaced element should have a new value.
        //

        TEST_METHOD(ReplaceMiddleElement)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "Middle",
                "D",
                "E"
            };
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
            // Replace the middle element in the list with another value.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "C,Middle", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Replace should return one entry replaced.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Replace an item that is not in the list.
        //
        // Result: list should not be modified.
        //

        TEST_METHOD(ReplaceItemNotInList)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };
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
            // Replace an item that is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "Zero,Empty", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Replace should return zero entries replaced.");

            Assert::AreEqual((size_t) 5, pl->Count(), L"List must have five elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Replace an entry that occurs more than once in a list.
        //
        // Result: replaced item should be replaced for each occurence in the original list.
        //

        TEST_METHOD(ReplaceMultiple)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "Middle",
                "D",
                "E",
                "A",
                "B",
                "Middle",
                "D",
                "E"
            };
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;
            PSTR appendelements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            //
            // Create a new list.
            //

            auto pl = CreateAndAppendUsingGetMember();

            //
            // Set the source pointer to the new instance.
            //

            source.Ptr = pl.get();

            //
            // Append the contents of the list to itself.
            //

            for (auto item : appendelements)
            {
                bResult = List::GetMemberInvoker(source, "Append", item, dest);
                Assert::IsTrue(bResult, L"Append invocation failed.");
                Assert::AreEqual(1, dest.Int, L"Append should return true.");

            }

            //
            // Replace the middle element.
            //

            bResult = List::GetMemberInvoker(source, "Replace", "C,Middle", dest);
            Assert::IsTrue(bResult, L"Replace invocation failed.");
            Assert::AreEqual(2, dest.Int, L"Replace should return two entries replaced.");

            Assert::AreEqual((size_t) 10, pl->Count(), L"List must have ten elements.");

            //
            // Verify the elements in the list.
            //

            CompareListToElements(source, elements, sizeof(elements) / sizeof(elements[0]));
        }

        //
        // Get the head of an empty list.
        //
        // Result: the head method call should fail.
        //

        TEST_METHOD(HeadOfEmptyList)
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
            // Pop the head from an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Head", nullptr, dest);
            Assert::IsTrue(bResult, L"Head invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Head should return false.");
        }

        //
        // Retrieve the head of the list.
        //
        // Result: the head should be successfully returned.
        //

        TEST_METHOD(HeadOfList)
        {
            PSTR head = "A";
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
            // Pop the head and verify it is expected value.
            //

            bResult = List::GetMemberInvoker(source, "Head", nullptr, dest);
            Assert::IsTrue(bResult, L"Head invocation failed.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            Assert::AreEqual(
                        (const char *) head,
                        (const char *) dest.Ptr,
                        false,
                        L"Head is not equal to the expected value.");
        }

        //
        // Pop all entries from the list using the Head method.
        //
        // Result: Each entry must match and at the end the list must be empty.
        //

        TEST_METHOD(PopAllFromHead)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Pop each item and verify that it is the same as the
            // corresponding entry in the elements list.
            //

            size_t nItem;

            nItem = 0;
            while (true)
            {
                //
                // Pop the head and verify it is expected value.
                //

                bResult = List::GetMemberInvoker(source, "Head", nullptr, dest);
                Assert::IsTrue(bResult, L"Head invocation failed.");

                Assert::AreEqual(
                    (const char *) elements[nItem],
                    (const char *) dest.Ptr,
                    false,
                    L"Head is not equal to the expected value.");

                ++nItem;

                if (nItem == (sizeof(elements) / sizeof(elements[0])))
                {
                    break;
                }
            }

            //
            // The list should be empty once all of the elements are removed.
            //

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must be empty.");
        }
        //
        // Get the tail of an empty list.
        //
        // Result: the tail method call should fail.
        //

        TEST_METHOD(TailOfEmptyList)
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
            // Pop the tail from an empty list.
            //

            bResult = List::GetMemberInvoker(source, "Tail", nullptr, dest);
            Assert::IsTrue(bResult, L"Tail invocation failed.");
            Assert::AreEqual(0, dest.Int, L"Tail should return false.");
        }

        //
        // Retrieve the tail of the list.
        //
        // Result: the tail should be successfully returned.
        //

        TEST_METHOD(TailOfList)
        {
            PSTR tail = "E";
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
            // Pop the tail and verify it is expected value.
            //

            bResult = List::GetMemberInvoker(source, "Tail", nullptr, dest);
            Assert::IsTrue(bResult, L"Tail invocation failed.");

            Assert::AreEqual((size_t) 4, pl->Count(), L"List must have four elements.");

            Assert::AreEqual(
                (const char *) tail,
                (const char *) dest.Ptr,
                false,
                L"Tail is not equal to the expected value.");
        }

        //
        // Pop all entries from the list using the Tail method.
        //
        // Result: Each entry must match and at the end the list must be empty.
        //

        TEST_METHOD(PopAllFromTail)
        {
            PSTR elements[] =
            {
                "E",
                "D",
                "C",
                "B",
                "A"
            };

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
            // Pop each item and verify that it is the same as the
            // corresponding entry in the elements list.
            //

            size_t nItem;

            nItem = 0;
            while (true)
            {
                //
                // Pop the tail and verify it is expected value.
                //

                bResult = List::GetMemberInvoker(source, "Tail", nullptr, dest);
                Assert::IsTrue(bResult, L"Tail invocation failed.");

                Assert::AreEqual(
                    (const char *) elements[nItem],
                    (const char *) dest.Ptr,
                    false,
                    L"Tail is not equal to the expected value.");

                ++nItem;

                if (nItem == (sizeof(elements) / sizeof(elements[0])))
                {
                    break;
                }
            }

            //
            // The list should be empty once all of the elements are removed.
            //

            Assert::AreEqual((size_t) 0, pl->Count(), L"List must be empty.");
        }

        //
        // Retrieve the CountOf on an empty list.
        //
        // Result: The value returned should be zero.
        //

        TEST_METHOD(CountOfOnEmptyList)
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
            // Return the CountOf an element in an empty list..
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "A", dest);
            Assert::IsTrue(bResult, L"CountOf invocation failed.");
            Assert::AreEqual(0, dest.Int, L"CountOf should return zero.");
        }

        //
        // Retrieve CountOf for an element that does not exist.
        //
        // Result: the value should be zero.
        //

        TEST_METHOD(CountOfNonexistantElement)
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
            // Count an element we know is not in the list.
            //

            bResult = List::GetMemberInvoker(source, "CountOf", "F", dest);
            Assert::IsTrue(bResult, L"CountOf invocation failed.");
            Assert::AreEqual(0, dest.Int, L"CountOf should return zero.");
        }

        //
        // Retrieve the CountOf for each element in the list.
        //
        // Result: each element should occur once in the list.
        //

        TEST_METHOD(CountOfKnownElements)
        {
            PSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

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
            // Count each element in the list.
            //

            for (auto item : elements)
            {
                bResult = List::GetMemberInvoker(source, "CountOf", item, dest);
                Assert::IsTrue(bResult, L"CountOf invocation failed.");
                Assert::AreEqual(1, dest.Int, L"CountOf should return one.");
            }
        }

    private:

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
            Assert::AreEqual(0, dest.Int, L"Count should be zero.");

            //
            // Append a single item to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A", dest);
            Assert::IsTrue(bResult, L"Append invocation failed");
            Assert::AreEqual(1, dest.Int, L"Append should return True");

            //
            // Append a sequence of items to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "B,C,D,E", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return True");

            //
            // Count should return 5.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed");
            Assert::AreEqual(5, dest.Int, L"GetMember Count should be five");

            return pl;
        }

        //
        // Create a list and append the quoted item elements using the GetMember interface.
        //

        std::unique_ptr<List> CreateAndAppendQuotedItemsUsingGetMember() const
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = { 0 };
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
            Assert::AreEqual(0, dest.Int, L"Count should be zero.");

            //
            // Append a single item to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "A", dest);
            Assert::IsTrue(bResult, L"Append invocation failed");
            Assert::AreEqual(1, dest.Int, L"Append should return True");

            //
            // Append a sequence of items to the list.
            //

            bResult = List::GetMemberInvoker(source, "Append", "B,\"C, is an item\",D,\"E has a quoted quote\\\" and comma (,)\"", dest);
            Assert::IsTrue(bResult, L"Append invocation failed.");
            Assert::AreEqual(1, dest.Int, L"Append should return True");

            //
            // Count should return 5.
            //

            bResult = List::GetMemberInvoker(source, "Count", nullptr, dest);
            Assert::IsTrue(bResult, L"Count invocation failed");
            Assert::AreEqual(5, dest.Int, L"GetMember Count should be five");

            return pl;
        }

        //
        // Compare a list with a sequence of strings.
        //

        void CompareListToElements(MQ2VARPTR & source, const PCHAR elements[], size_t numElements) const
        {
            MQ2TYPEVAR dest = {0};
            bool bResult;
            //
            // Retrieve the item at a position in the list.
            //

            for (size_t index = 0; index < numElements; ++index)
            {
                std::string item_s = std::to_string(index);
                bResult = List::GetMemberInvoker(source, "Item", (PCHAR) item_s.c_str(), dest);
                Assert::IsTrue(bResult, L"Item invocation failed.");
                Assert::IsNotNull(dest.Ptr, L"Returned item is a null pointer.");

                auto compare_result = strcmp((const char *) dest.Ptr, elements[index]);
                Assert::AreEqual(0, compare_result, L"Returned string does not match expected string.");
            }
        }
    };
}