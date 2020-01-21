//
// Implementation file for Stack.
//


#include "Stack.h"
#include "Conversions.h"

using namespace Collections::Containers;

//
// Initialize the member name map.  Last entry must contain a null string
// pointer.
//

const MQTypeMember Stack::StackMembers[] =
{
    { (DWORD) StackMembers::Count, "Count" },
    { (DWORD) StackMembers::Push, "Push" },
    { (DWORD) StackMembers::Pop, "Pop" },
    { (DWORD) StackMembers::IsEmpty, "IsEmpty" },
    { (DWORD) StackMembers::Peek, "Peek" },
    { 0, 0 }
};

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool Stack::GetMember(MQVarPtr VarPtr, PCHAR Member, PCHAR Index, MQTypeVar& Dest)
{
    Stack *pThis;
    std::string value;
    std::unique_ptr<std::string> pValue;

    DebugSpewAlways("Stack::GetMember %s", Member);

    //
    // Default return value is FALSE.
    //

    Dest.Int = 0;
    Dest.Type = mq::datatypes::pBoolType;

    //
    // Map the member name to the id.
    //

	auto pMember = Stack::FindMember(Member);
    if (pMember == nullptr)
    {
        //
        // No such member.
        //

        return false;
    }

    //
    // Member ID is an StackMembers enumeration.
    //

    pThis = reinterpret_cast<Stack *>(VarPtr.Ptr);
    if (pThis == nullptr)
    {
        DebugSpewAlways("Stack instance is NULL!");
        return false;
    }

    switch ((enum class StackMembers) pMember->ID)
    {
        case StackMembers::Count:
            //
            // Count of items in the stack.
            //

            Dest.Int = (int) pThis->Count();
            Dest.Type = mq::datatypes::pIntType;
            break;

        case StackMembers::Push:
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

        case StackMembers::Pop:
            //
            // Return the top of the stack, if it isn't empty.  If the stack is
            // empty, return FALSE.
            //

            if (pThis->Pop(&pValue))
            {
                Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(pValue->c_str(), pValue->size() + 1);
                Dest.Type = mq::datatypes::pStringType;
            }
            break;

        case StackMembers::IsEmpty:
            //
            // Return true if the stack is empty, and false otherwise.
            //

            Dest.Int = (int) pThis->IsEmpty();
            break;

        case StackMembers::Peek:
            //
            // Return the top of the stack, if the stack is not empty.  Don't
            // remove the entry from the stack.  If the stack is empty, return
            // FALSE.
            //

            if (pThis->Peek(&pValue))
            {
                Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(pValue->c_str(), pValue->size() + 1);
                Dest.Type = mq::datatypes::pStringType;
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
// Convert the stack to a string -- output the count of items.
//

bool Stack::ToString(MQVarPtr VarPtr, PCHAR Destination)
{
    Stack *pThis;

    pThis = reinterpret_cast<Stack *>(VarPtr.Ptr);
    if (Destination == nullptr)
    {
        return false;
    }

    return Conversions::ToString(pThis->Count(), Destination, BUFFER_SIZE) == 0;
}

//
// This method is executed when the /varset statement is executed.  Treat
// this as a stack Push call.
//

bool Stack::FromString(MQVarPtr& VarPtr, PCHAR Source)
{
    Stack *pDest;

    pDest = reinterpret_cast<Stack *>(VarPtr.Ptr);
    if ((pDest != nullptr) && NOT_EMPTY(Source))
    {
        pDest->Push(std::string(Source));
    }

    return false;
}
