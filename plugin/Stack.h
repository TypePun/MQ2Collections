//
// Stack - Stack of strings for MQ2.
//

#pragma once
#include "DebugMemory.h"

#include <string>
#include <stack>

#include "BufferManager.h"
#include "Types.h"

using namespace Utilities::Buffers;
using namespace Types;

namespace Collections
{
    namespace Containers
    {
        //
        // A stack is a collection where an entry is inserted (pushed) and
        // popped off the top.
        //

        class Stack : public ObjectType<Stack>
        {
        public:

            //
            // MQ2Type Members
            //

            enum class StackMembers
            {
                Count = 1,
                Push,
                Pop,
                IsEmpty,
                Peek
            };

            //
            // Constructor.
            //

            Stack()
                : ObjectType(StackMembers)
            {
                DebugSpew("Stack - %x", this);
            }

            //
            // Destructor.
            //

            ~Stack()
            {
                DebugSpew("~Stack - %x", this);
            }

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            Stack(const Stack &) = delete;
            const Stack &operator=(const Stack &) = delete;

            //
            // Return the name of this type - stack.
            //

            static const char *GetTypeName()
            {
                return "stack";
            }

            //
            // Return the number of entries on the stack.
            //

            size_t Count() const
            {
                return m_coll.size();
            }

            //
            // Push an entry onto the stack.
            //

            void Push(const std::string &item)
            {
                m_coll.push(item);
            }

            //
            // If the stack isn't empty, return the top element.  And remove
            // it from the stack.  The method returns true if the stack is
            // empty and false otherwise.
            //

            bool Pop(std::unique_ptr<std::string> *item)
            {
                if (IsEmpty())
                {
                    return false;
                }

                *item = std::make_unique<std::string>(m_coll.top());
                m_coll.pop();
                return true;
            }

            //
            // If the stack is empty, return true.  Otherwise return false.
            //

            bool IsEmpty() const
            {
                return m_coll.empty();
            }

            //
            // If the stack isn't empty, return the top element.  The method
            // returns true if the stack is empty and false otherwie.
            //

            bool Peek(std::unique_ptr<std::string> *item) const
            {
                if (IsEmpty())
                {
                    return false;
                }

                *item = std::make_unique<std::string>(m_coll.top());
                return true;
            }

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQVarPtr VarPtr, PCHAR Member, PCHAR Index, MQTypeVar& Dest);

            //
            // Convert the stack to a string -- output the count of items.
            //

            bool ToString(MQVarPtr VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  Treat
            // this as a stack push call.
            //

            bool FromString(MQVarPtr& VarPtr, PCHAR Source);

        private:

            //
            // Collection on which our stack is implemented.
            //

            std::stack<std::string> m_coll;

            //
            // Buffer containing the value of an item returned from the stack.
            //

            BufferManager<char> m_Buffer;

            //
            // Map from member ids onto names.
            //

            static const MQTypeMember StackMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
