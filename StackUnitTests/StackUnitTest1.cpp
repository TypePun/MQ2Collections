#if defined(UNICODE)
#undef UNICODE
#endif
#if defined(_UNICODE)
#undef _UNICODE
#endif

#include "stdafx.h"
#include "CppUnitTest.h"

#include "stack.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Collections::Containers;

namespace StackUnitTests
{
    //
    // Test Stack Operations (Push, Pop, Peek, Count, IsEmpty)
    //

    TEST_CLASS(StackUnitTest1)
    {
    public:
        BEGIN_TEST_CLASS_ATTRIBUTE()
            TEST_CLASS_ATTRIBUTE(L"Collections", L"Stack")
        END_TEST_CLASS_ATTRIBUTE()

        //
        // Test the Stack constructor.
        //
        // Result: a new empty stack should be constructed.
        //

        TEST_METHOD(Constructor)
        {
            Stack s;

            EmptyTest(s);
        }

        //
        // Push one element onto a stack.
        //
        // Result: the count is one and IsEmpty is false.
        //

        TEST_METHOD(TestPushOfOneElement)
        {
            Stack s;

            s.Push("A");

            Assert::AreEqual(s.Count(), (size_t) 1);
            Assert::IsFalse(s.IsEmpty());
        }

        //
        // Push three elements onto a stack.
        //
        // Result: the count is three and IsEmpty is false.
        //

        TEST_METHOD(TestPushOfThreeElements)
        {
            Stack s;

            PushThree(&s);

            Assert::AreEqual(s.Count(), (size_t) 3);
            Assert::IsFalse(s.IsEmpty());
        }

        //
        // Peek at the top of an empty stack.
        //
        // Result: the Peek should fail.
        //

        TEST_METHOD(TestPeekEmptyStack)
        {
            Stack s;
            std::unique_ptr<std::string> top;

            Assert::IsFalse(s.Peek(&top));
        }

        //
        // Peek at an item after pushing it onto a stack.
        //
        // Result: the pushed element and value returned by peek should be
        // the same.
        //

        TEST_METHOD(TestPeek)
        {
            Stack s;
            std::string element("A");
            std::unique_ptr<std::string> top;

            s.Push(element);
            Assert::IsTrue(s.Peek(&top));
            Assert::AreEqual(element, *top);
        }

        //
        // Pop elements off a stack.
        //
        // Result: the elements returned should be the same as those pushed
        // and the stack should be empty when they are popped off.
        //

        TEST_METHOD(TestPop)
        {
            Stack s;
            std::unique_ptr<std::string> top;

            PushThree(&s);

            Assert::IsTrue(s.Pop(&top));
            Assert::AreEqual(std::string("C"), *top);
            Assert::IsTrue(s.Pop(&top));
            Assert::AreEqual(std::string("B"), *top);
            Assert::IsTrue(s.Pop(&top));
            Assert::AreEqual(std::string("A"), *top);

            EmptyTest(s);
        }

        //
        // Pop an empty stack.
        //
        // Result: Pop should fail and the stack should be empty.
        //

        TEST_METHOD(TestPopEmptyStack)
        {
            Stack s;
            std::unique_ptr<std::string> top;

            Assert::IsFalse(s.Pop(&top));
            EmptyTest(s);
        }

        //
        // Pop more entries than are on a stack.
        //
        // Result: last pop should fail and the stack should be empty.
        //

        TEST_METHOD(TestStackUnderflow)
        {
            Stack s;
            std::unique_ptr<std::string> top;

            s.Push("A");

            Assert::IsTrue(s.Pop(&top));
            Assert::IsFalse(s.Pop(&top));
            EmptyTest(s);
        }

    private:
        //
        // Push three elements onto a stack.
        //

        void PushThree(Stack * s)
        {
            s->Push("A");
            s->Push("B");
            s->Push("C");
        }

        //
        // Test if a stack is empty -- s.Count() == 0 and s.IsEmpty.
        //

        void EmptyTest(const Stack &s)
        {
            Assert::AreEqual(s.Count(), (size_t) 0);
            Assert::IsTrue(s.IsEmpty());
        }
    };
}