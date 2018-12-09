//
// Implementation file for Stack.
//


#include <MQ2Plugin.h>

#include "Stack.h"
#include "Conversions.h"

using namespace Collections::Containers;

//
// Initialize the member name map.  Last entry must contain a null string
// pointer.
//

const MQ2TYPEMEMBER Stack::StackMembers[] =
{
	{ (DWORD) xCount, "Count" },
	{ (DWORD) xClear, "Clear" },
	{ (DWORD) xPush, "Push" },
	{ (DWORD) xPop, "Pop" },
	{ (DWORD) xIsEmpty, "IsEmpty" },
	{ (DWORD) xPeek, "Peek" },
	{ 0, 0 }
};

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool Stack::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest)
{
	Stack *pThis;
	std::string value;
	std::string *pValue;

	DebugSpewAlways("Stack::GetMember %s", Member);

	//
	// Default return value is FALSE.
	//

	Dest.Int = 0;
	Dest.Type = pBoolType;

	//
	// Map the member name to the id.
	//

	PMQ2TYPEMEMBER pMember = Stack::FindMember(Member);
	if (!pMember)
	{
		//
		// No such member.
		//

		return false;
	}

	//
	// Member ID is an StackCollectionMembers enumeration.
	//

	pThis = reinterpret_cast<Stack *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("Stack instance is NULL!");
		return false;
	}

	switch ((StackMemberEnums) pMember->ID)
	{
		case xCount:
			//
			// Count of items in the stack.
			//

			Dest.Int = (int) pThis->Count();
			Dest.Type = pIntType;
			return true;


		case xPush:
			//
			// We can only add an item if Index is a string.
			//

			if (*Index)
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

			case xPop:
				//
				// Return the top of the stack, if it isn't empty.  If the stack is
				// empty, return FALSE.
				//

				if (pThis->Pop(&pValue))
				{
					Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(
													pValue->c_str(),
													pValue->size() + 1
												);
					Dest.Type = pStringType;

					delete pValue;
				}
				break;

			case xIsEmpty:
				//
				// Return true if the stack is empty, and false otherwise.
				//

				Dest.Int = (int) pThis->IsEmpty();
				break;

			case xPeek:
				//
				// Return the top of the stack, if the stack is not empty.  Don't
				// remove the entry from the stack.  If the stack is empty, return
				// FALSE.
				//

				if (pThis->Peek(&pValue))
				{
					Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(
												pValue->c_str(),
												pValue->size() + 1
											);
					Dest.Type = pStringType;

					delete pValue;
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

bool Stack::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
	Stack *pThis;

	pThis = reinterpret_cast<Stack *>(VarPtr.Ptr);
	if (Destination == 0)
	{
		return false;
	}

    return Conversions::ToString (pThis->Count (), Destination, BUFFER_SIZE) == 0;
}

//
// This method is executed when the /varset statement is executed.  Treat
// this as a stack Push call.
//

bool Stack::FromString(MQ2VARPTR &VarPtr, PCHAR Source)
{
	Stack *pDest;

	pDest = reinterpret_cast<Stack *>(VarPtr.Ptr);
	if ((pDest != 0) && (Source != 0) && *Source)
	{
		pDest->Push(std::string(Source));
	}

	return false;
}

