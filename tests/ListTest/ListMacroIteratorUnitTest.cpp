#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include <typeinfo>
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
    // Test List Iterator Operations (Reset, Advance, IsEnd, Value).
    // List iterators are acquireed by calling Find or First on a List.
    //

    TEST_CLASS(ListMacroIteratorUnitTests)
    {
    public:

        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"List")
            TEST_CLASS_ATTRIBUTE(L"MQ2Interface", L"List")
            TEST_CLASS_ATTRIBUTE(L"Iterators", L"List")
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
        // Populate the list used by the iterator tests.
        //

        ListMacroIteratorUnitTests()
        {
            _l.Append("A");
            _l.Append("B");
            _l.Append("C");
            _l.Append("D");
            _l.Append("E");
        }

        //
        // Acquire an iterator using First on an empty collection.
        //
        // Result: a non-null iterator should be returned and IsEnd should be true.
        //

        TEST_METHOD(AcquireIteratorUsingFirstOnEmptyList)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2TYPEVAR dest = {0};
            MQ2TYPEVAR iterdest = {0};
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
            // Acquire an iterator using the First method.
            //

            bResult = List::GetMemberInvoker(source, "First", nullptr, dest);
            Assert::IsTrue(bResult, L"First invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"First should not return null.");
            Assert::IsNotNull(
                        dynamic_cast<ListIterator *>((ListIterator *) dest.Ptr),
                        L"First should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return true.");

            //
            // Free the ListIterator that was returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
            li->FreeVariable(itersource);
        }

        //
        // Acquire an iterator using First on an empty collection and Clone it.
        //
        // Result: a non-null iterator should be returned and IsEnd should be true
        // for both the initial iterator and the clone.
        //

        TEST_METHOD(CloneIteratorFromFirstOnEmptyList)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2VARPTR clonesource;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR iterdest = { 0 };
            MQ2TYPEVAR clonedest = { 0 };
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
            // Acquire an iterator using the First method.
            //

            bResult = List::GetMemberInvoker(source, "First", nullptr, dest);
            Assert::IsTrue(bResult, L"First invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"First should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)dest.Ptr),
                L"First should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
            // Clone the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "Clone", nullptr, clonedest);
            Assert::IsTrue(bResult, L"Clone invocation failed.");
            Assert::IsNotNull(clonedest.Ptr, L"Clone should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)clonedest.Ptr),
                L"First should be a list iterator.");

            clonesource.Ptr = clonedest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return true.");

            //
            // Find out if we are at the end of the cloned iterator.
            //

            bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Cloned IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Cloned IsEnd should return true.");

            //
            // Free the ListIterator and clone that were returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
            li->FreeVariable(itersource);

            li = reinterpret_cast<ListIterator *>(clonesource.Ptr);
            li->FreeVariable(clonesource);
        }

        //
        // Acquire an iterator using the First method.
        //
        // Result: a non-null iterator should be returned and IsEnd should be false.
        //

        TEST_METHOD(AcquireIteratorUsingFirst)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2TYPEVAR dest = {0};
            MQ2TYPEVAR iterdest = {0};
            bool bResult;

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using the First method.
            //

            bResult = List::GetMemberInvoker(source, "First", nullptr, dest);
            Assert::IsTrue(bResult, L"First invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"First should not return null.");
            Assert::IsNotNull(
                        dynamic_cast<ListIterator *>((ListIterator *) dest.Ptr),
                        L"First should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

            //
            // Free the ListIterator that was returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
            li->FreeVariable(itersource);
        }

        //
        // Acquire an iterator using the First method and Clone it.
        //
        // Result: a non-null iterator should be returned and IsEnd should be false
        // for both the iterator and the clone.
        //

        TEST_METHOD(CloneIteratorFromFirst)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2VARPTR clonesource;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR iterdest = { 0 };
            MQ2TYPEVAR clonedest = { 0 };
            bool bResult;

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using the First method.
            //

            bResult = List::GetMemberInvoker(source, "First", nullptr, dest);
            Assert::IsTrue(bResult, L"First invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"First should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)dest.Ptr),
                L"First should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
           // Clone the iterator.
           //

            bResult = ListIterator::GetMemberInvoker(itersource, "Clone", nullptr, clonedest);
            Assert::IsTrue(bResult, L"Clone invocation failed.");
            Assert::IsNotNull(clonedest.Ptr, L"Clone should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)clonedest.Ptr),
                L"First should be a list iterator.");

            clonesource.Ptr = clonedest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

            //
            // Find out if we are at the end of the cloned iterator.
            //

            bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"Clone IsEnd should return False.");

            //
            // Free the ListIterators that were returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
            li->FreeVariable(itersource);

            li = reinterpret_cast<ListIterator *>(clonesource.Ptr);
            li->FreeVariable(clonesource);
        }

        //
        // Acquire an iterator using Find on each element.
        //
        // Result: a non-null iterator should be returned where IsEnd is false.
        //

        TEST_METHOD(AcquireIteratorUsingFind)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2TYPEVAR dest = {0};
            MQ2TYPEVAR iterdest = {0};
            bool bResult;
            LPSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using Find on each of the items in the
            // list.
            //

            for (auto item : elements)
            {
                //
                // Acquire an iterator using the Find method.
                //

                bResult = List::GetMemberInvoker(source, "Find", item, dest);
                Assert::IsTrue(bResult, L"Find invocation failed.");
                Assert::IsNotNull(dest.Ptr, L"Find should not return null.");
                Assert::IsNotNull(
                            dynamic_cast<ListIterator *>((ListIterator *) dest.Ptr),
                            L"Find should be a list iterator.");

                //
                // Set the iterator source pointer to the returned instance.
                //

                itersource.Ptr = dest.Ptr;

                //
                // Find out if we are at the end of the iterator.
                //

                bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

                //
                // Free the ListIterator that was returned.
                //

                ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);

                li->FreeVariable(itersource);
            }
        }


        // Acquire an iterator using Find on each element and Clone them.
        //
        // Result: a non-null iterator should be returned where IsEnd is false
        // for each of the iterators and the clones.
        //

        TEST_METHOD(CloneIteratorUsingFind)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2VARPTR clonesource;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR iterdest = { 0 };
            MQ2TYPEVAR clonedest = { 0 };
            bool bResult;
            LPSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using Find on each of the items in the
            // list.
            //

            for (auto item : elements)
            {
                //
                // Acquire an iterator using the Find method.
                //

                bResult = List::GetMemberInvoker(source, "Find", item, dest);
                Assert::IsTrue(bResult, L"Find invocation failed.");
                Assert::IsNotNull(dest.Ptr, L"Find should not return null.");
                Assert::IsNotNull(
                    dynamic_cast<ListIterator *>((ListIterator *)dest.Ptr),
                    L"Find should be a list iterator.");

                //
                // Set the iterator source pointer to the returned instance.
                //

                itersource.Ptr = dest.Ptr;

                //
                // Clone the iterator.
                //

                bResult = ListIterator::GetMemberInvoker(itersource, "Clone", nullptr, clonedest);
                Assert::IsTrue(bResult, L"Clone invocation failed.");
                Assert::IsNotNull(clonedest.Ptr, L"Clone should not return null.");
                Assert::IsNotNull(
                    dynamic_cast<ListIterator *>((ListIterator *)clonedest.Ptr),
                    L"First should be a list iterator.");

                clonesource.Ptr = clonedest.Ptr;

                //
                // Find out if we are at the end of the iterator.
                //

                bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

                //
                // Find out if we are at the end of the cloned iterator.
                //

                bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"Clone IsEnd should return False.");

                //
                // Free the ListIterators that were returned.
                //

                ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
                li->FreeVariable(itersource);

                li = reinterpret_cast<ListIterator *>(clonesource.Ptr);
                li->FreeVariable(clonesource);
            }
        }

        //
        // Acquire an iterator on an element not in the list.
        //
        // Result: a non-null iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(AcquireIteratorForNonexistantElement)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2TYPEVAR dest = {0};
            MQ2TYPEVAR iterdest = {0};
            bool bResult;

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using the Find method.
            //

            bResult = List::GetMemberInvoker(source, "Find", "Z", dest);
            Assert::IsTrue(bResult, L"Find invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Find should not return null.");
            Assert::IsNotNull(
                        dynamic_cast<ListIterator *>((ListIterator *) dest.Ptr),
                        L"Find should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return True.");

            //
            // Free the ListIterator that was returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);

            li->FreeVariable(itersource);
        }

        //
        // Acquire an iterator on an element not in the list.
        //
        // Result: a non-null iterator should be returned where IsEnd is true.
        //

        TEST_METHOD(CloneIteratorForNonexistantElement)
        {
            MQ2VARPTR source;
            MQ2VARPTR itersource;
            MQ2VARPTR clonesource;
            MQ2TYPEVAR dest = { 0 };
            MQ2TYPEVAR iterdest = { 0 };
            MQ2TYPEVAR clonedest = { 0 };
            bool bResult;

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = &_l;

            //
            // Acquire an iterator using the Find method.
            //

            bResult = List::GetMemberInvoker(source, "Find", "Z", dest);
            Assert::IsTrue(bResult, L"Find invocation failed.");
            Assert::IsNotNull(dest.Ptr, L"Find should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)dest.Ptr),
                L"Find should be a list iterator.");

            //
            // Set the iterator source pointer to the returned instance.
            //

            itersource.Ptr = dest.Ptr;

            //
            // Clone the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "Clone", nullptr, clonedest);
            Assert::IsTrue(bResult, L"Clone invocation failed.");
            Assert::IsNotNull(clonedest.Ptr, L"Clone should not return null.");
            Assert::IsNotNull(
                dynamic_cast<ListIterator *>((ListIterator *)clonedest.Ptr),
                L"First should be a list iterator.");

            clonesource.Ptr = clonedest.Ptr;

            //
            // Find out if we are at the end of the iterator.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return True.");

            //
            // Find out if we are at the end of the cloned iterator.
            //

            bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Clone IsEnd should return True.");

            //
            // Free the ListIterators that were returned.
            //

            ListIterator * li = reinterpret_cast<ListIterator *>(itersource.Ptr);
            li->FreeVariable(itersource);

            li = reinterpret_cast<ListIterator *>(clonesource.Ptr);
            li->FreeVariable(clonesource);
        }

        //
        // Advance Count() times on a list.
        //
        // Result: Advance() should return true for Count() - 1 calls then
        // it should return False and IsEnd should be true.
        //

        TEST_METHOD(WalkIteratorUsingAdvance)
        {
            auto iterator = _l.First();
            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));
        }

        //
        // Advance Count() times on a list.
        //
        // Result: Advance() should return true for Count() - 1 calls then
        // it should return False and IsEnd should be true.
        //

        TEST_METHOD(CloneWalkIteratorUsingAdvance)
        {
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));
            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(clone.get()));
        }

        //
        // Reset the iterator after reaching the end and traverse through the
        // collection again.
        //
        // Result: reset should permit multiple traverals through the list.
        //

        TEST_METHOD(UseResetOnIterator)
        {
            MQ2VARPTR itersource;
            MQ2TYPEVAR iterdest = {0};
            bool bResult;
            auto iterator = _l.First();

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));

            itersource.Ptr = iterator;

            //
            // Reset the iterator using the GetMember interface.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "Reset", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Reset invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Reset should return True.");

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

            //
            // And walk over the list again.
            //

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));
        }

        //
        // Reset the iterator after reaching the end and traverse through the
        // collection again.
        //
        // Result: reset should permit multiple traverals through the list.
        //

        TEST_METHOD(CloneUseResetOnIterator)
        {
            MQ2VARPTR itersource;
            MQ2VARPTR clonesource;
            MQ2TYPEVAR iterdest = { 0 };
            bool bResult;
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));

            itersource.Ptr = iterator;

            //
            // Reset the iterator using the GetMember interface.
            //

            bResult = ListIterator::GetMemberInvoker(itersource, "Reset", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Reset invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Reset should return True.");

            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

            //
            // And walk over the list again.
            //

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(iterator));

            //
            // Do the same for the clone.
            //

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(clone.get()));

            clonesource.Ptr = clone.get();

            //
            // Reset the iterator using the GetMember interface.
            //

            bResult = ListIterator::GetMemberInvoker(clonesource, "Reset", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Clone Reset invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Clone Reset should return True.");

            bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
            Assert::IsFalse(iterdest.Int, L"Clone IsEnd should return False.");

            //
            // And walk over the list via the clone again.
            //

            WalkIteratorOverList(_l, reinterpret_cast<ListIterator *>(clone.get()));
        }

        //
        // Retrieve the value under an iterator.
        //
        // Result: the value method should return each element in the list.
        //

        TEST_METHOD(RetrieveValueUnderIterator)
        {
            MQ2VARPTR itersource;
            MQ2TYPEVAR iterdest = {0};
            bool bResult;
            auto iterator = _l.First();
            LPSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };
            size_t index;

            //
            // Set the iterator as the source interface.
            //

            itersource.Ptr = iterator;

            //
            // Retrieve each element under the iterator and ensure that we can fetch it
            // and that it matches the expected value.
            //

            index = 0;
            for (auto item : elements)
            {
                bResult = ListIterator::GetMemberInvoker(itersource, "Value", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Value invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Value should return True.");

                Assert::AreEqual(
                        (const char *) elements[index], 
                        (const char *) iterdest.Ptr,
                        false,
                        L"Expected value not found.");

                bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

                index++;

                bResult = ListIterator::GetMemberInvoker(itersource, "Advance", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Advance invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Advance should return True.");
            }

            //
            // IsEnd should be true after processing the entire list.
            //
            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return True.");
        }

        //
        // Retrieve the value under an iterator.
        //
        // Result: the value method should return each element in the list.
        //

        TEST_METHOD(CloneRetrieveValueUnderIterator)
        {
            MQ2VARPTR clonesource;
            MQ2VARPTR itersource;
            MQ2TYPEVAR iterdest = { 0 };
            bool bResult;
            auto iterator = _l.First();
            auto clone = dynamic_cast<ListIterator *>(iterator)->Clone();
            LPSTR elements[] =
            {
                "A",
                "B",
                "C",
                "D",
                "E"
            };
            size_t index;

            //
            // Set the iterator as the source interface.
            //

            itersource.Ptr = iterator;

            //
            // Retrieve each element under the iterator and ensure that we can fetch it
            // and that it matches the expected value.
            //

            index = 0;
            for (auto item : elements)
            {
                bResult = ListIterator::GetMemberInvoker(itersource, "Value", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Value invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Value should return True.");

                Assert::AreEqual(
                    (const char *)elements[index],
                    (const char *)iterdest.Ptr,
                    false,
                    L"Expected value not found.");

                bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"IsEnd should return False.");

                index++;

                bResult = ListIterator::GetMemberInvoker(itersource, "Advance", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Advance invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Advance should return True.");
            }

            //
            // IsEnd should be true after processing the entire list.
            //
            bResult = ListIterator::GetMemberInvoker(itersource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"IsEnd should return True.");

            //
            // Repeat the process for the clone.
            //

            clonesource.Ptr = clone.get();

            //
            // Retrieve each element under the iterator and ensure that we can fetch it
            // and that it matches the expected value.
            //

            index = 0;
            for (auto item : elements)
            {
                bResult = ListIterator::GetMemberInvoker(clonesource, "Value", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Clone Value invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Clone Value should return True.");

                Assert::AreEqual(
                    (const char *)elements[index],
                    (const char *)iterdest.Ptr,
                    false,
                    L"Expected clone value not found.");

                bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
                Assert::IsFalse(iterdest.Int, L"Clone IsEnd should return False.");

                index++;

                bResult = ListIterator::GetMemberInvoker(clonesource, "Advance", nullptr, iterdest);
                Assert::IsTrue(bResult, L"Clone Advance invocation failed.");
                Assert::IsTrue(iterdest.Int, L"Clone Advance should return True.");
            }

            //
            // IsEnd should be true after processing the entire list.
            //
            bResult = ListIterator::GetMemberInvoker(clonesource, "IsEnd", nullptr, iterdest);
            Assert::IsTrue(bResult, L"Clone IsEnd invocation failed.");
            Assert::IsTrue(iterdest.Int, L"Clone IsEnd should return True.");
        }

    private:
        //
        // Acquire an iterator and walk through each element using the GetMember
        // interface.
        //

        void WalkIteratorOverList(const List & l, ListIterator * it) const
        {
            MQ2VARPTR source;
            MQ2TYPEVAR dest = {0};
            bool bResult;

            Assert::IsNotNull(it, L"Iterator should not be null.");

            //
            // Set our source pointer to be the populated list.
            //

            source.Ptr = it;

            //
            // Verify the iterator is not AtEnd.
            //

            bResult = ListIterator::GetMemberInvoker(source, "IsEnd", nullptr, dest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsFalse(dest.Int, L"IsEnd should return True.");

            for (size_t i = 1; i < l.Count(); ++i)
            {
                bResult = ListIterator::GetMemberInvoker(source, "Advance", nullptr, dest);
                Assert::IsTrue(bResult, L"Advance invocation failed.");
                Assert::IsTrue(dest.Int, L"Advance should return True.");
            }

            bResult = ListIterator::GetMemberInvoker(source, "Advance", nullptr, dest);
            Assert::IsTrue(bResult, L"Advance invocation failed.");
            Assert::IsTrue(dest.Int, L"Advance should return True.");

            bResult = ListIterator::GetMemberInvoker(source, "IsEnd", nullptr, dest);
            Assert::IsTrue(bResult, L"IsEnd invocation failed.");
            Assert::IsTrue(dest.Int, L"IsEnd should return True.");
        }

        List _l;
   };
}