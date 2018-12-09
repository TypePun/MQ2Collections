//
// Implementation file for ListIterator and List.
//

#include <algorithm>
#include <memory>
#include <MQ2Plugin.h>

#include "List.h"
#include "StringExtensions.h"
#include "Conversions.h"

using namespace Collections::Containers;
using namespace Extensions::Strings;
using namespace Conversions;

//
// Initialize the member name map for the list iterator.  Last entry must
// contain a null string pointer.
//

const MQ2TYPEMEMBER ListIterator::ListIteratorMembers[] =
{
	{ (DWORD) ListIteratorMemberEnums::Reset, "Reset" },
	{ (DWORD) ListIteratorMemberEnums::Advance, "Advance" },
	{ (DWORD) ListIteratorMemberEnums::IsEnd, "IsEnd" },
	{ (DWORD) ListIteratorMemberEnums::Value, "Value" },
	{ 0, 0 }
};

//
// Initialize the member name map for the list.  Last entry must contain a null
// string pointer.
//

const MQ2TYPEMEMBER List::ListMembers[] =
{
	{ (DWORD) ListMemberEnums::Count, "Count" },
	{ (DWORD) ListMemberEnums::Clear, "Clear" },
	{ (DWORD) ListMemberEnums::Contains, "Contains" },
	{ (DWORD) ListMemberEnums::Splice, "Splice" },
	{ (DWORD) ListMemberEnums::Index, "Index" },
	{ (DWORD) ListMemberEnums::Item, "Item" },
	{ (DWORD) ListMemberEnums::Insert, "Insert" },
	{ (DWORD) ListMemberEnums::Sort, "Sort" },
	{ (DWORD) ListMemberEnums::Reverse, "Reverse" },
	{ (DWORD) ListMemberEnums::Append, "Append" },
	{ (DWORD) ListMemberEnums::Remove, "Remove" },
	{ (DWORD) ListMemberEnums::Erase, "Erase" },
	{ (DWORD) ListMemberEnums::Replace, "Replace" },
	{ (DWORD) ListMemberEnums::First, "First" },
	{ (DWORD) ListMemberEnums::Find, "Find" },
	{ (DWORD) ListMemberEnums::Head, "Head" },
	{ (DWORD) ListMemberEnums::Tail, "Tail" },
	{ (DWORD) ListMemberEnums::CountOf, "CountOf" },
	{ 0, 0 }
};

//
// ListIterator methods.
//

//
// Constructor.
//

ListIterator::ListIterator(const std::list<std::string> & refCollection)
			: ValueIterator<std::list<std::string>>(refCollection),
			  ReferenceType(ListIteratorMembers)
{
	DebugSpew("ListIterator - %x", this);
}

//
// Constructor - find a particular element, position to the end
// if the element does not exist.
//

ListIterator::ListIterator(
					const std::list<std::string> & refCollection,
					const std::string & refKey
				)
			: ValueIterator<std::list<std::string>>(refCollection),
			   ReferenceType(ListIteratorMembers)
{
	DebugSpew("ListIterator - %x", this);

	//
	// Position the iterator to the item or to the end of the
	// set.
	//

	Find(refKey);
}

//
// Destructor.
//

ListIterator::~ListIterator()
{
	DebugSpew("~ListIterator - %x", this);
}

//
// Return the name of this type - listiterator.
//

const char *ListIterator::GetTypeName()
{
	return "listiterator";
}

//
// Return the value in the list under the current iterator.
//

bool ListIterator::Value(const std::string ** const item) const
{
	//
	// Return false if we are after the end of the list.
	//

	if (IsEnd())
	{
		return false;
	}

	*item = &(*m_iterator);
	return true;
}

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool ListIterator::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
	ListIterator * pThis;
	const std::string * pItem;

	DebugSpew("ListIterator::GetMember %s", Member);

	//
	// Default return value is FALSE.
	//

	Dest.Int = 0;
	Dest.Type = pBoolType;

	//
	// Map the member name to the id.
	//

	PMQ2TYPEMEMBER pMember = ListIterator::FindMember(Member);
	if (!pMember)
	{
		//
		// No such member.
		//

		return false;
	}

	//
	// Member ID is an ListIteratorMemberEnums enumeration.
	//

	pThis = reinterpret_cast<ListIterator *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("ListIterator instance is NULL!");
		return false;
	}

	switch ((ListIteratorMemberEnums) pMember->ID)
	{
		case ListIteratorMemberEnums::Reset:
			//
			// Reset the iterator to the start of the list.   Return the result
			// as TRUE.
			//

			pThis->Reset();

			Dest.Int = 1;
			break;

		case ListIteratorMemberEnums::Advance:
			//
			// Advance the iterator.  Return TRUE if the iterator could be
			// advanced and FALSE otherwise.
			//

			Dest.Int = (int) pThis->Advance();
			break;

		case ListIteratorMemberEnums::IsEnd:
			//
			// Return TRUE if we are at the Last element in the list and FALSE
			// otherwise.
			//

			Dest.Int = (int) pThis->IsEnd();
			break;

		case ListIteratorMemberEnums::Value:
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

bool ListIterator::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
	ListIterator * pThis;
	const std::string * item;

	pThis = reinterpret_cast<ListIterator *>(VarPtr.Ptr);
	if (Destination == nullptr)
	{
		return false;
	}

	if (!pThis->Value(&item))
	{
        std::uninitialized_copy_n("FALSE", strlen ("FALSE") + 1, Destination);
	}
	else
	{
        std::uninitialized_copy(item->cbegin(), item->cend(), Destination);
        Destination[item->length () + 1] = 0;

	}

	return true;
}

//
// This method is executed when the /varset statement is executed.
// Return false because this operation is not supported.
//

bool ListIterator::FromString(MQ2VARPTR & VarPtr, PCHAR Source)
{
	return false;
}

//
// Return an iterator on the list for a particular key.  Return
// false if the key is not found.
//

bool ListIterator::Find(const std::string & refKey)
{
	m_iterator = std::find(
						m_refCollection.cbegin(),
						m_refCollection.cend(), 
						refKey
					);

	//
	// Key was not in the collection.
	//

	return m_iterator != m_refCollection.end();
}

//
// Constructor.
//

//
// Constructor.
//

List::List()
	: ObjectType(ListMembers)
{
	DebugSpew("List - %x", this);
}

//
// Construct a new List from a list of strings.
//

List::List(const std::list<std::string> & list)
	: ObjectType(ListMembers)
{
	DebugSpew("List - %x", this);

	//
	// Copy the collection to our instance.
	//

	m_coll = list;
}

//
// Destructor.
//

List::~List()
{
	DebugSpew("~List - %x", this);
}

//
// Return the name of this type - set.
//

const char *List::GetTypeName()
{
	return "list";
}

//
// Return true if a key is in the list.
//

bool List::Contains(const std::string & item) const
{
	return std::find(
					m_coll.cbegin(),
					m_coll.cend(),
					item
				) != m_coll.end();
}

//
// Return the index of an item in the list.  -1 is returned if the
// item is not found.  The index is 0 based.  If multiple items
// match item then the index of only the first is returned.
//

long List::Index(const std::string & item) const
{
	long nPosition;

	//
	// Compare each element in the list until key is found or we reach the
	// end of the list.
	//

	nPosition = 0;
	auto it = m_coll.cbegin();
	while (it != m_coll.cend())
	{
		if (*it == item)
		{
			//
			// Key found.
			//

			return nPosition;
		}

		++it;
		++nPosition;
	}

	//
	// Key not found.
	//

	return -1;
}

//
// Retrieve the item at position index in the list.  True is
// returned if the item exists and false otherwise.
//

bool List::Item(const size_t index, const std::string ** const item) const
{
	//
	// Find the item at position index.
	//

	auto it = FindIteratorForPosition(index);
	if (it != m_coll.cend())
	{
		//
		// Return a reference to the item.
		//

		*item = &(*it);
		return true;
	}

	//
	// The index was not found in the list.
	//

	return false;
}

//
// Sort the list into ascending order.  The list is altered in
// place.
//

void List::Sort()
{
	m_coll.sort();
}

//
// The elements in the list are reversed.  The list is reversed in
// place.
//

void List::Reverse()
{
	m_coll.reverse();
}

//
// Append an item to the end of the list.
//

void List::Append(const std::string & item)
{
	m_coll.push_back(item);
}

//
// Remove an element from the list.   Return a count of the
// number of items that matched item.
//

size_t List::Remove(const std::string & item)
{
	size_t itemCount;

	//
	// Count how many times item occurs in the list.
	//

	itemCount = 0;
	for (auto it = m_coll.cbegin(); it != m_coll.cend(); ++it)
	{
		if (*it == item)
		{
			++itemCount;
		}
	}

	//
	// Remove the item if there are any in the list.  This test saves an
	// additional pass over the list for the removal.
	//

	if (itemCount != 0)
	{
		m_coll.remove(item);
	}

	return itemCount;
}

//
// Erase an item at a position in the list.  Return true if the
// index was in the list and false otherwise.
//

bool List::Erase(const size_t index)
{
	//
	// If index is after the end of the list, don't erase anything.
	//

	auto it = FindIteratorForPosition(index);
	if (it == m_coll.cend())
	{
		return false;
	}

	//
	// Otherwise, remove the element.
	//

	m_coll.erase(it);
	return true;
}

//
// Replace an item in the list.  Return a count of the number
// of items replaced.
//

size_t List::Replace(
				const std::string & item,
				const std::string & newItem
			)
{
	size_t replacedElements;

	//
	// If the current item in the list matches item, replace the value
	// with newItem.  This replacement is done in-place.  No new
	// item is created.
	//

	replacedElements = 0;
	for (auto it = m_coll.begin(); it != m_coll.end(); ++it)
	{
		if (*it == item)
		{
			//
			// Replace this item.
			//

			*it = newItem;
			++replacedElements;
		}
	}

	return replacedElements;
}

//
// Return an iterator to a requested key or to the end of the set.
//

std::unique_ptr<ValueIterator<std::list<std::string>>> List::Find(
								const std::string & refKey
							) const
{
	return std::unique_ptr<ValueIterator<std::list<std::string>>>(
								new ListIterator(m_coll, refKey)
						);
}

//
// Remove and return the head of the list.  Return true if there
// was a head and false otherwise.
//

bool List::Head(const std::string ** item)
{
	//
	// Only return the head if there are entries in the list.
	//

	if (Count() > 0)
	{
		//
		// Copy the head of the list to a new std::string and return
		// that string to the caller.  Then remove the head and
		// return true.
		//
		// The caller is responsible for deleting the string.
		//

		*item = new std::string(m_coll.front());
		m_coll.pop_front();

		return true;
	}

	return false;
}

//
// Remove and return the tail of the list.  Return true if there
// was a tail and false otherwise.
//

bool List::Tail(const std::string ** item)
{
	//
	// Only return the tail if there are entries in the list.
	//

	if (Count() > 0)
	{
		//
		// Copy the tail of the list to a new std::string and return
		// that string to the caller.  Then remove the tail and
		// return true.
		//
		// The caller is responsible for deleting the string.
		//

		*item = new std::string(m_coll.back());
		m_coll.pop_back();

		return true;
	}

	return false;
}

//
// Return a count of how many times item occurs in the list.
//

size_t List::CountOf(const std::string & item) const
{
	return std::count(
				m_coll.cbegin(),
				m_coll.cend(),
				item
			);
}

//
// Return a splice of a list based upon a starting position.  If
// position is beyond the end of the list, an empty list is
// returned.  A negative index is an offset from the end
// of the list.
//

std::unique_ptr<List> List::Splice(long index) const
{
	std::list<std::string> newList(
								FindIteratorForPosition(
										NormalizeIndex(index)
									),
								m_coll.cend()
							);
	return std::unique_ptr<List>(new List(newList));
}

//
// Create a splice from a starting to an ending offset.
//

std::unique_ptr<List> List::Splice(long startIndex, long endIndex) const
{
	//
	// The normalized start index must be less than or equal to the normalized
	// end index.  Otherwise, return an empty list.
	//

	size_t normalizedStartIndex;
	size_t normalizedEndIndex;

	normalizedStartIndex = NormalizeIndex(startIndex);
	normalizedEndIndex = NormalizeIndex(endIndex);

	if (normalizedStartIndex > normalizedEndIndex)
	{
		return std::unique_ptr<List>(new List());
	}

	//
	// Return a splice from the range.
	//

	std::list<std::string> newList(
								FindIteratorForPosition(
										NormalizeIndex(startIndex)
									),
								FindIteratorForPosition(
										NormalizeIndex(endIndex)
									)
							);
	return std::unique_ptr<List>(new List(newList));
}

//
// Create a splice between two offsets, selecting a stride'th
// element.  If stride is zero, return an empty list.
//

std::unique_ptr<List> List::Splice(
								long startIndex,
								long endIndex,
								long stride
							) const
{
	//
	// For a zero stride, return an empty list.
	//

	if (stride == 0)
	{
		return std::unique_ptr<List>(new List());
	}

	std::unique_ptr<List> newList = Splice(startIndex, endIndex);

	//
	// Only include the stride'th items in the output list.  Return the
	// elements in reverse order if the stride is negative.
	//
	// Therefore, we remove the elements that are NOT stride elements.
	//

	if (stride < 0)
	{
		newList->m_coll.reverse();
		newList->m_coll.remove_if(IsNotStride(std::abs(stride)));
	}
	else
	{
		newList->m_coll.remove_if(IsNotStride(stride));
	}

	return newList;
}

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool List::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
	List * pThis;
	MQ2TYPEVAR typeVar;
	const std::string * pItem;
	size_t replacedItems;

	DebugSpew("List::GetMember %s", Member);

	//
	// Default return value is FALSE.
	//

	Dest.Int = 0;
	Dest.Type = pBoolType;

	//
	// Map the member name to the id.
	//

	PMQ2TYPEMEMBER pMember = List::FindMember(Member);
	if (!pMember)
	{
		//
		// No such member.
		//

		return false;
	}

	//
	// Member ID is a ListMemberEnums enumeration.
	//

	pThis = reinterpret_cast<List *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("List instance is NULL!");
		return false;
	}

	switch ((ListMemberEnums) pMember->ID)
	{
		case ListMemberEnums::Count:
			//
			// Count of items in the list.
			//

			Dest.Int = (int) pThis->Count();
			Dest.Type = pIntType;
			return true;

		case ListMemberEnums::Clear:
			//
			// Clear the list.  Return the result as TRUE.
			//

			pThis->Clear();

			Dest.Int = 1;
			break;

		case ListMemberEnums::Contains:
			//
			// Does the list contain a key?
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->Contains(std::string(Index));
			}
			break;

		case ListMemberEnums::Splice:
			//
			// Return a splice from this list.
			//

			Dest.Ptr = (PVOID) pThis->CreateSplice(std::string(Index)).release();

			//
			// Get the List type and return it.
			//

			List::TypeDescriptor(0, typeVar);
			Dest.Type = typeVar.Type;

			break;

		case ListMemberEnums::Index:
			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				//
				// -1 is returned if the index was not found.
				//

				Dest.Int = (int) pThis->Index(std::string(Index));
			}
			break;

		case ListMemberEnums::Item:
			//
			// Return the index'th item from the list.
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				//
				// Item fails if Index does not correspond to an item.
				//

				if (pThis->Item(std::string(Index), &pItem))
				{
					Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(
											pItem->c_str(),
											pItem->size() + 1
									);
					Dest.Type = pStringType;
				}
			}
			break;

		case ListMemberEnums::Insert:
			//
			// Insert the items into the index'th position in the list.
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->Insert(std::string(Index));
			}

			break;

		case ListMemberEnums::Sort:
			//
			// Sort the list in-place.  Always return true.
			//

			pThis->Sort();

			Dest.Int = 1;
			break;

		case ListMemberEnums::Reverse:
			//
			// Reverse the list in-place.  Always return true.
			//

			pThis->Reverse();

			Dest.Int = 1;
			break;

		case ListMemberEnums::Append:
			//
			// Append the items to the end of the list.
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				pThis->AppendItems(std::string(Index));

				//
				// Return TRUE.
				//

				Dest.Int = 1;
			}
			break;

		case ListMemberEnums::Remove:
			//
			// Remove an item from the list.  Return TRUE if it was removed and
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

		case ListMemberEnums::Erase:
			//
			// Remove an item by index.  Return TRUE if it was removed and
			// FALSE otherwise.
			//

			//
			// Only attempt to erase if there is an index.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->Erase(std::string(Index));
			}
			break;

		case ListMemberEnums::Replace:
			//
			// Replace one list item with another.  Return the number of times
			// the source item was replaced by the target.
			//

			//
			// Verify that there is an argument.
			//

			if (*Index)
			{
				if (pThis->Replace(std::string(Index), &replacedItems))
				{
					Dest.Int = (int) replacedItems;
				}
			}
			break;

		case ListMemberEnums::First:
			//
			// Return an iterator on the first element.
			//

			Dest.Ptr = (PVOID) pThis->First().release();

			//
			// Get the ListIterator type and return it.
			//

			ListIterator::TypeDescriptor(0, typeVar);
			Dest.Type = typeVar.Type;
			break;

		case ListMemberEnums::Find:
			//
			// Return an iterator on a value in the list.
			//

			//
			// It only makes sense to find a non-null value.
			//

			if (*Index)
			{
				Dest.Ptr = (PVOID) pThis->Find(std::string(Index)).release();

				//
				// Get the ListIterator type and return it.
				//

				ListIterator::TypeDescriptor(0, typeVar);
				Dest.Type = typeVar.Type;
			}
			break;

		case ListMemberEnums::Head:
			//
			// Return the current element at the head or FALSE if there
			// isn't one.
			//

			if (pThis->Head(&pItem))
			{
				Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(
											pItem->c_str(),
											pItem->size() + 1
										);
				Dest.Type = pStringType;

				//
				// Returned item must be deleted.
				//

				delete pItem;
			}
			break;

		case ListMemberEnums::Tail:
			//
			// Return the current element at the tail or FALSE if there
			// isn't one.
			//

			if (pThis->Tail(&pItem))
			{
				Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(
											pItem->c_str(),
											pItem->size() + 1
										);
				Dest.Type = pStringType;

				//
				// Returned item must be deleted.
				//

				delete pItem;
			}
			break;

		case ListMemberEnums::CountOf:
			//
			// How many times does an item occur in the list?
			//

			//
			// Check for a valid Index value.
			//

			if (*Index)
			{
				Dest.Int = (int) pThis->CountOf(std::string(Index));
				Dest.Type = pIntType;
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
// Used by the testing framework to call GetMember through the
// source variable.
//

bool List::GetMemberInvoker(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
	List * pThis;

	DebugSpew("List::GetMemberInvoker %s", Member);

	pThis = reinterpret_cast<List *>(VarPtr.Ptr);
	if (!pThis)
	{
		DebugSpewAlways("List instance is NULL!");
		return false;
	}

	return pThis->GetMember(VarPtr, Member, Index, Dest);
}

//
// Convert the list to a string -- output the count of items.
//

bool List::ToString(MQ2VARPTR VarPtr, PCHAR Destination)
{
	List * pThis;

	pThis = reinterpret_cast<List *>(VarPtr.Ptr);
	if (Destination == nullptr)
	{
		return false;
	}

    errno_t rc;
    rc = Conversions::ToString (pThis->Count (),
                (char *) Destination,
                BUFFER_SIZE - 1);
    return rc == 0;
}

//
// This method is executed when the /varset statement is executed.  Treat
// this as a list Append call.
//

bool List::FromString(MQ2VARPTR & VarPtr, PCHAR Source)
{
	List * pDest;

	pDest = reinterpret_cast<List *>(VarPtr.Ptr);

	//
	// Don't add null or empty strings!
	//

	if ((pDest != nullptr) && (Source != nullptr) && *Source)
	{
		pDest->Append(std::string(Source));
	}

	return true;
}

//
// Return an iterator on the list.
//

std::unique_ptr<ValueIterator<std::list<std::string>>> List::GetNewIterator(
								const std::list<std::string> & refCollection
							) const
{
	return std::unique_ptr<ValueIterator<std::list<std::string>>>(new ListIterator(refCollection));
}

//
// Create a new list from source list by selecting items between
// two iterators.
//

std::unique_ptr<List> List::Splice(
								const std::list<std::string>::const_iterator & first,
								const std::list<std::string>::const_iterator & end
							) const
{
	std::unique_ptr<std::list<std::string>> newList(new std::list<std::string>(first, end));
	return std::unique_ptr<List>(new List(*newList));
}

//
// Return an iterator for a position index.  Position 0 means
// the beginning of the list.  If position is beyond the end
// of the list, return m_coll.end().
//

std::list<std::string>::const_iterator List::FindIteratorForPosition(
								size_t position
							) const
{
	//
	// If position is past the end of the list, return an iterator
	// to the end of the list.
	//

	if (position >= Count())
	{
		return m_coll.cend();
	}

	auto it = m_coll.cbegin();
	while (position != 0)
	{
		--position;
		++it;
	}

	return it;
}

//
// Create a splice of a list from a sequence of splice indices.
//
// Splice takes at least two arguments: a starting position
// for the returned splice, an optional ending position and
// an optional 'stride'.  If the starting position is omitted,
// the entire list is assumed.  If the ending position is omitted,
// the default is the end of the list.  If the stride is
// omitted, the default stride is 1.  If the starting
// position is negative, the offset is assumed to be from the
// end of the list.  If the ending position is negative, then
// it is also taken to be from the end of the list.
// Valid usages are:
//
//		Splice[]		- the entire list is copied
//		Splice[2]		- a copy is made from the third element
//						  to the end of the list
//		Splice[-2]		- a copy is made from the second to last
//						  entry to the end of the list.
//		Splice[0, 3]	- a copy is made of the first four elements
//						  of the list.
//		Splice[0, -2]	- a copy is made of all the elements of the
//						  list EXCEPT the last element.
//		Splice[-5, -2]	- a copy is made of the last four elements
//						  of the list.
//		Splice[0, -1, 2]
//						- a copy is made of every other element in
//						  the list.
//		Splice[-5, -2, 2]
//						- a copy is made of every other element of
//						  the last four elements of the list.
//
// If the constraint can not be satisfied, an empty list is returned.
//

std::unique_ptr<List> List::CreateSplice(const std::string & indices) const
{
	//
	// Split the string into extents.  Each extent represents an argument
	// for splice.
	//

	auto arguments = std::make_unique<StringExtensions<std::string>>(indices);
	auto coll = std::unique_ptr<StringExtensions<std::string>::container_type>(
										arguments->Split(std::string(","))
									);

	//
	// Parse the arguments and return the spliced list.
	//

	switch (coll->size())
	{
		case 0:
			//
			// No arguments.  Return a copy of the existing list.
			//

			return std::unique_ptr<List>(new List(m_coll));

		case 1:
			//
			// Return a new list from a starting bound to the end.
			//

			return CreateSpliceFrom(coll->at(0));

		case 2:
			//
			// Return a new list from a starting bound to an ending bound.
			//

			return CreateSpliceFrom(coll->at(0), coll->at(1));

		case 3:
			//
			// Return a new list from a starting bound to an ending bound
			// with a given 'stride'.
			//

			return CreateSpliceFrom(coll->at(0), coll->at(1), coll->at(2));

		default:
			//
			// Too many arguments.  Return a null pointer.
			//

			return std::unique_ptr<List>();
	}
}

//
// Create a splice of the form list[arg] or list[-arg].  This returns a splice
// from the index arg to the end of the list or from -arg to the end of the
// list.
//
// If startIndex is past the end of the list, an empty list is returned.
//

std::unique_ptr<List> List::CreateSpliceFrom(
								const std::string & startIndex
							) const
{
	long lIndex;

	//
	// Translate the string to an index value.  Return an empty list
	// if the index is invalid.
	//

	if (!Extensions::Strings::FromString(startIndex, &lIndex))
	{
		//
		// Can't convert the start index.
		//

		return std::unique_ptr<List>(new List);
	}

	//
	// Generate a splice from the index to the end [lIndex, end).
	//

	return Splice(lIndex);
}

//
// Create a splice of the form list[arg1, arg2], where arg1 and/or arg2 can be
// negative.  If either are negative, the values are taken as offsets from the
// end of the list.
//
// If startIndex > endIndex, then an empty list is returned.
//

std::unique_ptr<List> List::CreateSpliceFrom(
								const std::string & startIndex,
								const std::string & endIndex
							) const
{
	long lIndex1;
	long lIndex2;

	//
	// Translate the strings to index values.  Return an empty list if
	// either is invalid.
	//

	if (!Extensions::Strings::FromString(startIndex, &lIndex1)
	 || !Extensions::Strings::FromString(endIndex, &lIndex2))
	{
		return std::unique_ptr<List>(new List);
	}

	//
	// Generate a splice from the index in the range [lIndex1, lIndex2).
	//

	return Splice(lIndex1, lIndex2);
}

//
// Create a splice of the form list[arg1, arg2], where arg1 and/or arg2 can be
// negative.  If either are negative, the values are taken as offsets from the
// end of the list.
//
// For the resultant list, the stride'th argument is selected.  That is, for
// a list [0, 1, 2, 3, 4, 5], where arg1 is 1 and arg2 is 4 and stride is
// 2 the resultant list is [2, 4].
//
// If startIndex > endIndex, then an empty list is returned.
//

std::unique_ptr<List> List::CreateSpliceFrom(
								const std::string & startIndex,
								const std::string & endIndex,
								const std::string & stride
							) const
{
	long lIndex1;
	long lIndex2;
	long lStride;

	//
	// Translate the strings to index values.  Return an empty list if either
	// are invalid.
	//

	if (!Extensions::Strings::FromString(startIndex, &lIndex1)
	 || !Extensions::Strings::FromString(endIndex, &lIndex2))
	{
		return std::unique_ptr<List>(new List);
	}

	//
	// Convert the stride.  It must be a positive number.
	//

	if (!Extensions::Strings::FromString(stride, &lStride) && (lStride > 0))
	{
		//
		// Can't convert the stride.
		//

		return std::unique_ptr<List>(new List);
	}

	return Splice(lIndex1, lIndex2, (unsigned long) lStride);
}

//
// Retrieve the index'th value from the list.  Return true if index
// is in the bounds of the list and false otherwise.  Index can
// be negative, in which case it is an offset from the end of the
// list.
//

bool List::Item(
				const std::string & index,
				const std::string ** const item
			) const
{
	long lIndex;

	//
	// Convert the string to an index value.  Fail if we can not convert it.
	//

	if (!IndexValueFromString(index, &lIndex))
	{
		return false;
	}

	//
	// Get the item from the position.
	//

	return Item((size_t) lIndex, item);
}

//
// Insert a sequence of items starting at index into the current list.  Index
// can be negative, in which case it is an offset from the end of the list. 
// Return true if the items could be inserted and false otherwise.
//

bool List::Insert(const std::string & args)
{
	long lIndex;

	//
	// Split the arguments.  There must be at least two of them -- an index
	// and a tail, which is the sequence of items to insert.
	//

	//
	// Split the string into extents.  Each extent represents an argument
	// to insert.
	//

	auto arguments = std::make_unique<StringExtensions<std::string>>(args);
	auto coll = std::unique_ptr<StringExtensions<std::string>::container_type>(
									arguments->Split(std::string(","))
								);

	//
	// There must be at least two arguments.
	//

	if (coll->size() < 2)
	{
		return false;
	}

	//
	// Acquire the index value.
	//

	if (!IndexValueFromString(coll->at(0), &lIndex))
	{
		return false;
	}

	//
	// Remove the Index from the returned sequence and insert it in the
	// list.
	//

	coll->erase(coll->cbegin());

	return Insert((size_t) lIndex, *coll);
}

//
// Append a sequence of items onto the end of the list.
//

void List::AppendItems(const std::string & args)
{
	//
	// Split the string into extents.  Each extent represents an argument
	// to insert.  An empty collection will append no strings.
	//

	auto arguments = std::make_unique<StringExtensions<std::string>>(args);
	auto coll = std::unique_ptr<StringExtensions<std::string>::container_type>(
							arguments->Split(std::string(","))
						);

	std::for_each(coll->cbegin(),
				  coll->cend(),
				  [this](const std::string & item)
				  {
					Append(item);
				  }
			);
}

//
// Erase an index in the list.  Index can be negative, in which
// case it is an offset from the end of the list.  Return true if
// the item was erased (deleted) and false otherwise.
//

bool List::Erase(const std::string & index)
{
	long lIndex;

	//
	// Acquire the index value.
	//

	if (!IndexValueFromString(index, &lIndex))
	{
		return false;
	}

	//
	// And erase the item.
	//

	return Erase((size_t) lIndex);
}

//
// Replace a source item with a target item.  The argument must
// be a list of two items.  Count is set to the number of times
// the source was replaced by the target.  True is returned if
// there are two arguments and false otherwise.
//

bool List::Replace(const std::string & args, size_t * count)
{
	size_t replaceCount;

	//
	// Split the strings and ensure there are two arguments.
	//

	auto arguments = std::make_unique<StringExtensions<std::string>>(args);
	auto coll = std::unique_ptr<StringExtensions<std::string>::container_type>(
							arguments->Split(std::string(","))
						);

	if (coll->size() != 2)
	{
		return false;
	}

	//
	// Replace the first string with the second.
	//

	replaceCount = Replace(coll->at(0), coll->at(1));

	if (count)
	{
		*count = replaceCount;
	}

	return true;
}

//
// Convert an index value from a string.  Return true if the
// string could be converted and false otherwise.
//

bool List::IndexValueFromString(
								const std::string & stringIndex,
								long * longIndex
							) const
{
	long lIndex;

	//
	// Select all elements of the list from startIndex to the end of
	// the list.  If startIndex is negative, use it as an offset from
	// the end of the list.
	//

	if (!Extensions::Strings::FromString(stringIndex, &lIndex))
	{
		//
		// Can't convert the index.
		//

		return false;
	}

	//
	// Compute lIndex as an offset from the start of the list.
	//

	//
	// If the list is empty, then treat any index as the start
	// of the list.
	//

	if (Count() == 0)
	{
		lIndex = 0;
	}
	else
	{
		//
		// If the index is positive, select from this index to the end of the
		// list.  Otherwise, use the index as an offset from the end of the list.
		//

		if (lIndex >= 0)
		{
			//
			// Fail if the index is past the end of the list.
			//

			if ((size_t) lIndex >= Count())
			{
				return false;
			}
		}
		else
		{
			//
			// If the index would precede the list, then it is out of range.
			//

			if ((lIndex + (long) Count()) < 0)
			{
				return false;
			}

			//
			// Convert the index to an offset from the beginning of the list.
			//

			lIndex = (long) Count() + lIndex;
		}
	}

	//
	// Return the computed index.
	//

	if (longIndex)
	{
		*longIndex = lIndex;
	}

	return true;
}

//
// Undefine max function -- we want to use the template function instead.
//

#if defined(max)
#undef max
#endif

//
// Normalize a signed index to be relative to the start of the
// list.  For example, an index of -1 is the length - 1.
//

long List::NormalizeIndex(long originalIndex) const
{
	long newIndex;

	//
	// If the index is positive, use it as is.
	//

	if (originalIndex >= 0)
	{
		return originalIndex;
	}

	newIndex = ((long) m_coll.size()) + originalIndex;

	//
	// If the newIndex is less than zero, use zero.
	//

	return std::max(0L, newIndex);
}


