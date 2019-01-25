#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

//
// Global lock used to access the Member map. The Mutex is initialized
// inside MQ2 when it is running. In our unit tests, the initialization
// code is not run so initialize it before the unit test.
//

EQLIB_VAR HANDLE ghMemberMapLock;

namespace QueueUnitTests
{
    TEST_CLASS(QueueUnitTest1)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Queue")
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
        // Test the Queue constructor.
        //
        // Result: a new empty queue should be constructed.
        //

        TEST_METHOD(Constructor)
        {
            Queue q;

            EmptyTest(q);
        }

        //
        // Push one element onto a queue.
        //
        // Result: the count is one and IsEmpty is false.
        //

        TEST_METHOD(TestPushOfOneElement)
        {
            Queue q;

            q.Push("A");

            Assert::AreEqual(q.Count(), (size_t) 1);
            Assert::IsFalse(q.IsEmpty());
        }

        //
        // Push three elements onto a queue.
        //
        // Result: the count is three and IsEmpty is false.
        //

        TEST_METHOD(TestPushOfThreeElements)
        {
            Queue q;

            PushThree(&q);

            Assert::AreEqual(q.Count(), (size_t) 3);
            Assert::IsFalse(q.IsEmpty());
        }

        //
        // Peek at the front of an empty queue.
        //
        // Result: the Peek should fail.
        //

        TEST_METHOD(TestPeekEmptyQueue)
        {
            Queue q;
            std::unique_ptr<std::string> front;

            Assert::IsFalse(q.Peek(&front));
        }

        //
        // Peek at an item after pushing it onto a queue.
        //
        // Result: the pushed element and value returned by peek should be
        // the same.
        //

        TEST_METHOD(TestPeek)
        {
            Queue q;
            std::string element("A");
            std::unique_ptr<std::string> front;

            q.Push(element);
            Assert::IsTrue(q.Peek(&front));
            Assert::AreEqual(element, *front);
        }

        //
        // Pop elements from a queue.
        //
        // Result: the elements returned should be the same as those pushed
        // and the queue should be empty when they are popped off.
        //

        TEST_METHOD(TestPop)
        {
            Queue q;
            std::unique_ptr<std::string> front;

            PushThree(&q);

            Assert::IsTrue(q.Pop(&front));
            Assert::AreEqual(std::string("A"), *front);
            Assert::IsTrue(q.Pop(&front));
            Assert::AreEqual(std::string("B"), *front);
            Assert::IsTrue(q.Pop(&front));
            Assert::AreEqual(std::string("C"), *front);

            EmptyTest(q);
        }

        //
        // Pop an empty queue.
        //
        // Result: Pop should fail and the queue should be empty.
        //

        TEST_METHOD(TestPopEmptyQueue)
        {
            Queue q;
            std::unique_ptr<std::string> front;

            Assert::IsFalse(q.Pop(&front));
            EmptyTest(q);
        }

        //
        // Pop more entries than are on a stack.
        //
        // Result: last pop should fail and the stack should be empty.
        //

        TEST_METHOD(TestQueueUnderflow)
        {
            Queue q;
            std::unique_ptr<std::string> front;

            q.Push("A");

            Assert::IsTrue(q.Pop(&front));
            Assert::IsFalse(q.Pop(&front));
            EmptyTest(q);
        }

    private:
        //
        // Push three elements onto a queue.
        //

        void PushThree(Queue * q)
        {
            q->Push("A");
            q->Push("B");
            q->Push("C");
        }

        //
        // Test if a queue is empty -- q.Count() == 0 and q.IsEmpty.
        //

        void EmptyTest(const Queue &q)
        {
            Assert::AreEqual(q.Count(), (size_t) 0);
            Assert::IsTrue(q.IsEmpty());
        }
    };
}