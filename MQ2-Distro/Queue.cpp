//
// Implementation file for Queue.
//


#include <MQ2Plugin.h>

#include "Queue.h"
#include "Conversions.h"

using namespace Collections::Containers;

//
// Initialize the member name map.  Last entry must contain a null string
// pointer.
//

const MQ2TYPEMEMBER Queue::QueueMembers[] =
{
    { (DWORD) QueueMembers::Count, "Count" },
    { (DWORD) QueueMembers::Push, "Push" },
    { (DWORD) QueueMembers::Pop, "Pop" },
    { (DWORD) QueueMembers::IsEmpty, "IsEmpty" },
    { (DWORD) QueueMembers::Peek, "Peek" },
    { 0, 0 }
};

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool Queue::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest)
{
    Queue *pThis;
    std::string value;
    std::unique_ptr<std::string> pValue;

    DebugSpew("Queue::GetMember %s", Member);

    //
    // Default return value is FALSE.
    //

    Dest.Int = 0;
    Dest.Type = pBoolType;

    //
    // Map the member name to the id.
    //

    PMQ2TYPEMEMBER pMember = Queue::FindMember(Member);
    if (pMember == nullptr)
    {
        //
        // No such member.
        //

        return false;
    }

    //
    // Member ID is an QueueMembers enumeration.
    //

    pThis = reinterpret_cast<Queue *>(VarPtr.Ptr);
    if (pThis == nullptr)
    {
        DebugSpewAlways("Queue instance is NULL!");
        return false;
    }

    switch ((enum class QueueMembers) pMember->ID)
    {
        case QueueMembers::Count:
            //
            // Count of items in the stack.
            //

            Dest.Int = (int) pThis->Count();
            Dest.Type = pIntType;
            break;

        case QueueMembers::Push:
            //
            // Push an item onto the end of the queue.
            //

            //
            // We can only add an item if Index is a string.
            //

            if (NOT_EMPTY(Index))
            {
                value = Index;

                //
                // And the string can't be blank either.
                //

                if (value.find_first_not_of(" \t\n\r\f") != std::string::npos)
                {
                    pThis->Push(value);
                    Dest.Int = 1;
                }
            }
            break;

        case QueueMembers::Pop:
            //
            // Return the front of the stack, if it isn't empty.  If the queue is
            // empty, return FALSE.
            //

            if (pThis->Pop(&pValue))
            {
                Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(pValue->c_str(), pValue->size() + 1);
                Dest.Type = pStringType;
            }
            break;

        case QueueMembers::IsEmpty:
            //
            // Return true if the queue is empty, and false otherwise.
            //

            Dest.Int = (int) pThis->IsEmpty();
            break;

        case QueueMembers::Peek:
            //
            // Return the top of the queue, if the queue is not empty.  Don't
            // remove the entry from the queue.  If the queue is empty, return
            // FALSE.
            //

            if (pThis->Peek(&pValue))
            {
                Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(pValue->c_str(), pValue->size() + 1);
                Dest.Type = pStringType;
            }
            break;

        default:

            //
            // Unknown member!
            //

            return false;
    }

    return true;
}

//
// Convert the queue to a string -- output the count of items.
//

bool Queue::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
    Queue *pThis;

    pThis = reinterpret_cast<Queue *>(VarPtr.Ptr);
    if (Destination == nullptr)
    {
        return false;
    }

    return Conversions::ToString(pThis->Count(), Destination, BUFFER_SIZE) == 0;
}

//
// This method is executed when the /varset statement is executed.  Treat
// this as a queue Push call.
//

bool Queue::FromString(MQ2VARPTR &VarPtr, PCHAR Source)
{
    Queue *pDest;

    pDest = reinterpret_cast<Queue *>(VarPtr.Ptr);
    if ((pDest != nullptr) && NOT_EMPTY(Source))
    {
        pDest->Push(std::string(Source));
    }

    return false;
}
