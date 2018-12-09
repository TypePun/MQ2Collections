
//
// Implementation file for SetIterator and Set.
//


#include <MQ2Plugin.h>

#include "Set.h"
#include "Conversions.h"

using namespace Collections::Containers;

//
// Initialize the member name map for the set iterator.  Last entry must
// contain a null string pointer.
//

const MQ2TYPEMEMBER SetIterator::SetIteratorMembers[] =
{
	{ (DWORD) xReset, "Reset" },
	{ (DWORD) xAdvance, "Advance" },
	{ (DWORD) xIsEnd, "IsEnd" },
	{ (DWORD) xValue, "Value" },
	{ 0, 0 }
};

//
// Initialize the member name map for the set.  Last entry must contain a null
// string pointer.
//

const MQ2TYPEMEMBER Set::SetMembers[] =
{
	{ (DWORD) xCount, "Count" },
	{ (DWORD) xClear, "Clear" },
	{ (DWORD) xContains, "Contains" },
	{ (DWORD) xAdd, "Add" },
	{ (DWORD) xRemove, "Remove" },
	{ (DWORD) xFirst, "First" },
	{ (DWORD) xFind, "Find" },
	{ 0, 0 }
};

//
// SetIterator methods.
//

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool SetIterator::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest)
{
	SetIterator *pThis;
	const std::string *pItem;

	DebugSpew("SetIterator::GetMember %s", Member);

	//
	// Default return value is FALSE.
	//

	Dest.Int = 0;
	Dest.Type = pBoolType;

	//
	// Map the member name to the id.
	//

	PMQ2TYPEMEMBER pMember = SetIterator::FindMember(Member);
	if (!pMember)
	{
		//
		// No such member.
		//

		return false;
	}

	//
	// Member ID is an SetIteratorMemberEnums enumeration.
	//

	pThis = reinterpret_cast<SetIterator *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("SetIterator instance is NULL!");
		return false;
	}

	switch ((SetIteratorMemberEnums) pMember->ID)
	{
		case xReset:
			//
			// Reset the iterator to the start of the set.   Return the result as
			// TRUE.
			//

			pThis->Reset();

			Dest.Int = 1;
			break;

		case xAdvance:
			//
			// Advance the iterator.  Return TRUE if the iterator could be advanced
			// and FALSE otherwise.
			//

			Dest.Int = (int) pThis->Advance();
			break;

		case xIsEnd:
			//
			// Return TRUE if we are at the last element in the set and FALSE
			// otherwise.
			//

			Dest.Int = (int) pThis->IsEnd();
			break;

		case xValue:
			//
			// Return the current element under the iterator or FALSE if there
			// isn't one.
			//

			if (pThis->Value(&pItem))
			{
				Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(
													pItem->c_str(),
													pItem->size() + 1
												);
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
// Convert the iterator to a string.  Output the current item or FALSE if there
// isn't one.
//

bool SetIterator::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
	SetIterator *pThis;
	const std::string *item;

	pThis = reinterpret_cast<SetIterator *>(VarPtr.Ptr);
	if (Destination == 0)
	{
		return false;
	}

	if (!pThis->Value(&item))
	{
        if (strcpy_s (Destination, BUFFER_SIZE, "FALSE") != 0)
            return false;
	}
	else
	{
		if (strcpy_s(Destination, BUFFER_SIZE, item->c_str()) != 0)
            return false;
	}

	return true;
}

//
// This method is executed when the /varset statement is executed.
// Return false because this operation is not supported.
//

bool SetIterator::FromString(MQ2VARPTR &VarPtr, PCHAR Source)
{
	return false;
}

//
// Return an iterator to a requested key or to the end of the set.
//

ValueIterator<std::set<std::string>> * Set::Find(
								const std::string & refKey
							) const
{
	return new SetIterator(m_coll, refKey);
}

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool Set::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest)
{
	Set *pThis;
	MQ2TYPEVAR iteratorTypeVar;
	std::string value;

	DebugSpew("Set::GetMember %s", Member);

	//
	// Default return value is FALSE.
	//

	Dest.Int = 0;
	Dest.Type = pBoolType;

	//
	// Map the member name to the id.
	//

	PMQ2TYPEMEMBER pMember = Set::FindMember(Member);
	if (!pMember)
	{
		//
		// No such member.
		//

		return false;
	}

	//
	// Member ID is an SetMemberEnums enumeration.
	//

	pThis = reinterpret_cast<Set *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("Set instance is NULL!");
		return false;
	}

	switch ((SetMemberEnums) pMember->ID)
	{
		case xCount:
			//
			// Count of items in the set.
			//

			Dest.Int = (int) pThis->Count();
			Dest.Type = pIntType;
			return true;

		case xClear:
			//
			// Clear the set.  Return the result as TRUE.
			//

			pThis->Clear();

			Dest.Int = 1;
			break;

		case xContains:
			//
			// Does the set contain a key?
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->Contains(std::string(Index));
			}
			break;

		case xAdd:
			//
			// Add an item to the set.  Return TRUE if the item was added.
			//

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
					pThis->Add(value);
					Dest.Int = 1;
				}
			}
			break;

		case xRemove:
			//
			// Remove an item from the set.  Return TRUE if it was removed and
			// FALSE otherwise.
			//

			//
			// Only attempt a remove if there is a string.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->Remove(std::string(Index));
			}
			break;

		case xFirst:
			//
			// Return an iterator on the first element.
			//

			Dest.Ptr = (PVOID) pThis->First().release();

			//
			// Get the SetIterator type and return it.
			//

			SetIterator::TypeDescriptor(0, iteratorTypeVar);
			Dest.Type = iteratorTypeVar.Type;
			break;

		case xFind:
			//
			// Return an iterator on a key.
			//

			//
			// It only makes sense to find a non-null key.
			//

			if (*Index)
			{
				Dest.Ptr = (PVOID)pThis->Find(std::string(Index));

				//
				// Get the SetIterator type and return it.
				//

				SetIterator::TypeDescriptor(0, iteratorTypeVar);
				Dest.Type = iteratorTypeVar.Type;
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
// Convert the set to a string -- output the count of items.
//

bool Set::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
	Set *pThis;

	pThis = reinterpret_cast<Set *>(VarPtr.Ptr);
	if (Destination == 0)
	{
		return false;
	}

    return Conversions::ToString (pThis->Count (), Destination, BUFFER_SIZE) == 0;
}


//
// This method is executed when the /varset statement is executed.  Treat
// this as a set Add call.
//

bool Set::FromString(MQ2VARPTR &VarPtr, PCHAR Source)
{
	Set *pDest;

	pDest = reinterpret_cast<Set *>(VarPtr.Ptr);

	//
	// Don't add null or empty strings!
	//

	if ((pDest != 0) && (Source != 0) && *Source)
	{
		pDest->Add(std::string(Source));
	}

	return true;
}
