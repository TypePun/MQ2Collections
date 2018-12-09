//
// Queue - Queue of strings for MQ2.
//

#pragma once

#if !defined(__QUEUE__)
#define __QUEUE__

#include <string>
#include <queue>

#include "BufferManager.h"
#include "Types.h"

using namespace Utilities::Buffers;
using namespace Types;

namespace Collections
{
	namespace Containers
	{
		//
		// A queue is a collection where an entry is inserted (pushed) onto the
		// end and popped off the front.
		//

		class Queue : public ObjectType<Queue>
		{
		public:

			//
			// MQ2Type Members
			//

			enum QueueMemberEnums
			{
				xCount = 1,
				xClear,
				xPush,
				xPop,
				xIsEmpty,
				xPeek
			};

			//
			// Constructor.
			//

			Queue()
				: ObjectType(QueueMembers)
			{
				DebugSpew("Queue - %x", this);
			}

			//
			// Destructor.
			//

			~Queue()
			{
				DebugSpew("~Queue - %x", this);
			}

			//
			// Return the name of this type - queue.
			//

			static const char *GetTypeName()
			{
				return "queue";
			}


			//
			// Return the number of entries on the queue.
			//

			size_t Count() const
			{
				return m_coll.size();
			}

			//
			// Push an entry onto the queue.
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

			bool Pop(std::string **item)
			{
				if (IsEmpty())
				{
					return false;
				}

				*item = new std::string(m_coll.front());
				m_coll.pop();
				return true;
			}

			//
			// If the queue is empty, return true.  Otherwise return false.
			//

			bool IsEmpty() const
			{
				return m_coll.empty();
			}

			//
			// If the queue isn't empty, return the front element.  The method
			// returns true if the queue is empty and false otherwie.
			//

			bool Peek(std::string **item) const
			{
				if (IsEmpty())
				{
					return false;
				}

				*item = new std::string(m_coll.front());
				return true;
			}

			//
			// When a member function is called on the type, this method is called.
			// It returns true if the method succeeded and false otherwise.
			//

			bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest);

			//
			// Convert the queue to a string -- output the count of items.
			//

			bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

			//
			// This method is executed when the /varset statement is executed.  Treat
			// this as a queue push call.
			//

			bool FromString(MQ2VARPTR &VarPtr, PCHAR Source);

		private:

			//
			// Don't permit copy construction and assignment since the MQ2Type does
			// implement them.
			//

            Queue (const Queue &) = delete;
            const Queue &operator=(const Queue &) = delete;

			//
			// Collection on which our queue is implemented.
			//

			std::queue<std::string> m_coll;

			//
			// Buffer containing the value of an item returned from the queue.
			//

			BufferManager<char> m_Buffer;

			//
			// Map from member ids onto names.
			//

			static const MQ2TYPEMEMBER QueueMembers[];

		};
	}  // namespace Containers
}  // namespace Collections
#endif