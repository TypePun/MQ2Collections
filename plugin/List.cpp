//
// Implementation file for ListIterator and List.
//

#include <algorithm>
#include <memory>

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
    { (DWORD) ListIteratorMembers::Reset, "Reset" },
    { (DWORD) ListIteratorMembers::Advance, "Advance" },
    { (DWORD) ListIteratorMembers::IsEnd, "IsEnd" },
    { (DWORD) ListIteratorMembers::Value, "Value" },
    { (DWORD) ListIteratorMembers::Clone, "Clone" },
    { 0, 0 }
};

//
// Initialize the member name map for the list.  Last entry must contain a null
// string pointer.
//

const MQ2TYPEMEMBER List::ListMembers[] =
{
    { (DWORD) ListMembers::Count, "Count" },
    { (DWORD) ListMembers::Clear, "Clear" },
    { (DWORD) ListMembers::Contains, "Contains" },
    { (DWORD) ListMembers::Splice, "Splice" },
    { (DWORD) ListMembers::Index, "Index" },
    { (DWORD) ListMembers::Item, "Item" },
    { (DWORD) ListMembers::Insert, "Insert" },
    { (DWORD) ListMembers::Sort, "Sort" },
    { (DWORD) ListMembers::Reverse, "Reverse" },
    { (DWORD) ListMembers::Append, "Append" },
    { (DWORD) ListMembers::Remove, "Remove" },
    { (DWORD) ListMembers::Erase, "Erase" },
    { (DWORD) ListMembers::Replace, "Replace" },
    { (DWORD) ListMembers::First, "First" },
    { (DWORD) ListMembers::Find, "Find" },
    { (DWORD) ListMembers::Head, "Head" },
    { (DWORD) ListMembers::Tail, "Tail" },
    { (DWORD) ListMembers::CountOf, "CountOf" },
    { (DWORD) ListMembers::Delimiter, "Delimiter" },
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
                    const std::string & refKey)
    : ValueIterator<std::list<std::string>>(refCollection),
      ReferenceType(ListIteratorMembers)
{
    DebugSpew("ListIterator - %x", this);

    //
    // Position the iterator to the item or to the end of the
    // list.
    //

    Find(refKey);
}

//
// Copy constructor for a list iterator.
//

ListIterator::ListIterator(const ListIterator & original)
    : ValueIterator<std::list<std::string>>(original),
      ReferenceType(ListIteratorMembers)
{
    DebugSpew("ListIterator copy ctor - %x", this);
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
// Cloned iterators can be deleted.
//

const bool ListIterator::CanDelete() const
{
    return Cloned();
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
    MQ2TYPEVAR typeVar;
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
    if (pMember == nullptr)
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
    if (pThis == nullptr)
    {
        DebugSpewAlways("ListIterator instance is NULL!");
        return false;
    }

    switch ((enum class ListIteratorMembers) pMember->ID)
    {
        case ListIteratorMembers::Reset:
            //
            // Reset the iterator to the start of the list.   Return the result
            // as TRUE.
            //

            pThis->Reset();

            Dest.Int = 1;
            break;

        case ListIteratorMembers::Advance:
            //
            // Advance the iterator.  Return TRUE if the iterator could be
            // advanced and FALSE otherwise.
            //

            Dest.Int = (int) pThis->Advance();
            break;

        case ListIteratorMembers::IsEnd:
            //
            // Return TRUE if we are at the Last element in the list and FALSE
            // otherwise.
            //

            Dest.Int = (int) pThis->IsEnd();
            break;

        case ListIteratorMembers::Value:
            //
            // Return the current element under the iterator or FALSE if there
            // isn't one.
            //

            if (pThis->Value(&pItem))
            {
                Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(pItem->c_str(), pItem->size() + 1);
                Dest.Type = pStringType;
            }
            break;

        case ListIteratorMembers::Clone:
            //
            //Invoke the copy constructor on the list iterarator.
            //

            Dest.Ptr = (PVOID)std::unique_ptr<ListIterator>(pThis).release();

            //
            // Get the ListIterator type and return it.
            //

            ListIterator::TypeDescriptor(0, typeVar);
            Dest.Type = typeVar.Type;
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

bool ListIterator::GetMemberInvoker(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
    ListIterator * pThis;

    DebugSpew("ListIterator::GetMemberInvoker %s", Member);

    pThis = reinterpret_cast<ListIterator *>(VarPtr.Ptr);
    if (pThis == nullptr)
    {
        DebugSpewAlways("ListIterator instance is NULL!");
        return false;
    }

    return pThis->GetMember(VarPtr, Member, Index, Dest);
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

    errno_t rc;
    if (!pThis->Value(&item))
    {
        rc = strcpy_s(Destination, BUFFER_SIZE, "FALSE");
    }
    else
    {
        rc = strcpy_s(Destination, BUFFER_SIZE, item->c_str());
    }

    return rc == 0;
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
    m_iterator = std::find(m_refCollection.cbegin(), m_refCollection.cend(), refKey);

    //
    // Key was not in the collection.
    //

    return m_iterator != m_refCollection.end();
}

//
// List Constructor.
//

List::List()
    : ObjectType(ListMembers),
      m_delimiter(",")
{
    DebugSpew("List - %x", this);
}

//
// Construct an new list from a std::list.
//

List::List(const std::list<std::string> & source)
    : ObjectType(ListMembers),
      m_delimiter(",")
{
    DebugSpew("List - %x", this);

    m_coll = source;
}

//
// Destructor.
//

List::~List()
{
    DebugSpew("~List - %x", this);
}

//
// Return the name of this type - list.
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
    return std::find(m_coll.cbegin(), m_coll.cend(), item) != m_coll.end();
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
    //
    // Count how many times item occurs in the list.
    //

    auto itemCount = std::count(m_coll.cbegin(), m_coll.cend(), item);

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

size_t List::Replace(const std::string & item, const std::string & newItem)
{
    //
    // Count the items so we can return the replacement count.
    //

    size_t replacedElements = CountOf(item);

    if (replacedElements != 0)
    {
        //
        // Replacement is done in-place.
        //

        std::replace(m_coll.begin(), m_coll.end(), item, newItem);
    }

    return replacedElements;
}

//
// Return an iterator to a requested key or to the end of the set.
//

std::unique_ptr<ValueIterator<std::list<std::string>>> List::Find(const std::string & refKey) const
{
    return std::make_unique<ListIterator>(m_coll, refKey);
}

//
// Remove and return the head of the list.  Return true if there
// was a head and false otherwise.
//

bool List::Head(std::unique_ptr<const std::string> * item)
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

        *item = std::make_unique<const std::string>(m_coll.front());
        m_coll.pop_front();

        return true;
    }

    return false;
}

//
// Remove and return the tail of the list.  Return true if there
// was a tail and false otherwise.
//

bool List::Tail(std::unique_ptr<const std::string> * item)
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

        *item = std::make_unique<const std::string>(m_coll.back());
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
    return std::count(m_coll.cbegin(), m_coll.cend(), item);
}

//
// Create a splice from a starting index to the end of the list. If startIndex is past
// the end of the list, an empty list is returned.
//

std::unique_ptr<List> List::Splice(size_t startIndex) const
{
    //
    // Return an empty list if startIndex is past the end of the list or length
    // is zero.
    //

    if (startIndex >= Count())
    {
        return std::make_unique<List>();
    }

    //
    // Return a splice from the range.
    //

    std::list<std::string> newList(FindIteratorForPosition(startIndex), m_coll.cend());

    return std::make_unique<List>(newList);
}

//
// Create a splice from a starting index for length entries. If startIndex is past
// the end of the list, an empty list is returned. If length is zero, an empty list
// is retruend. If there are less than length entries in the list, then at most
// Count() - startIndex entries will be returned.
//

std::unique_ptr<List> List::Splice(size_t startIndex, size_t length) const
{
    //
    // Return an empty list if startIndex is past the end of the list or length
    // is zero.
    //

    if ((startIndex >= Count()) || (length == 0))
    {
        return std::make_unique<List>();
    }

    //
    // Return a splice from the range.
    //

    std::list<std::string> newList(
                            FindIteratorForPosition(startIndex),
                            FindIteratorForPosition(startIndex + length));

    return std::make_unique<List>(newList);
}

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool List::GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
    List * pThis;
    MQ2TYPEVAR typeVar;
    std::unique_ptr<const std::string> pItem;
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
    if (pMember == nullptr)
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
    if (pThis == nullptr)
    {
        DebugSpewAlways("List instance is NULL!");
        return false;
    }

    switch ((enum class ListMembers) pMember->ID)
    {
        case ListMembers::Count:
            //
            // Count of items in the list.
            //

            Dest.Int = (int) pThis->Count();
            Dest.Type = pIntType;
            break;

        case ListMembers::Clear:
            //
            // Clear the list.  Return the result as TRUE.
            //

            pThis->Clear();

            Dest.Int = 1;
            break;

        case ListMembers::Contains:
            //
            // Does the list contain a value?
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = pThis->Contains(std::string(Index)) ? 1 : 0;
            }
            break;

        case ListMembers::Splice:
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

        case ListMembers::Index:
            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                //
                // -1 is returned if the index was not found.
                //

                Dest.Int = (int) pThis->Index(std::string(Index));
                Dest.Type = pIntType;
            }
            break;

        case ListMembers::Item:
            //
            // Return the index'th item from the list.
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                //
                // Item fails if Index does not correspond to an item.
                //

                std::string const * pItem;
                if (pThis->Item(std::string(Index), &pItem))
                {
                    Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(pItem->c_str(), pItem->size() + 1);
                    Dest.Type = pStringType;
                }
            }
            break;

        case ListMembers::Insert:
            //
            // Insert the items into the index'th position in the list.
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->Insert(std::string(Index));
            }

            break;

        case ListMembers::Sort:
            //
            // Sort the list in-place.  Always return true.
            //

            pThis->Sort();

            Dest.Int = 1;
            break;

        case ListMembers::Reverse:
            //
            // Reverse the list in-place.  Always return true.
            //

            pThis->Reverse();

            Dest.Int = 1;
            break;

        case ListMembers::Append:
            //
            // Append the items to the end of the list.
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                pThis->AppendItems(std::string(Index));

                //
                // Return TRUE.
                //

                Dest.Int = 1;
            }
            break;

        case ListMembers::Remove:
            //
            // Remove an item from the list.  Return how many items were removed.
            //

            //
            // Only attempt a remove if there is a string.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->Remove(std::string(Index));
                Dest.Type = pIntType;
            }
            break;

        case ListMembers::Erase:
            //
            // Remove an item by index.  Return TRUE if it was removed and
            // FALSE otherwise.
            //

            //
            // Only attempt to erase if there is an index.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->Erase(std::string(Index));
            }
            break;

        case ListMembers::Replace:
            //
            // Replace one list item with another.  Return the number of times
            // the source item was replaced by the target.
            //

            //
            // Verify that there is an argument.
            //

            if (NOT_EMPTY(Index))
            {
                if (pThis->Replace(std::string(Index), &replacedItems))
                {
                    Dest.Int = (int) replacedItems;
                    Dest.Type = pIntType;
                }
            }
            break;

        case ListMembers::First:
            //
            // Return an iterator on the first element.
            //

            Dest.Ptr = (PVOID) pThis->First();

            //
            // Get the ListIterator type and return it.
            //

            ListIterator::TypeDescriptor(0, typeVar);
            Dest.Type = typeVar.Type;
            break;

        case ListMembers::Find:
            //
            // Return an iterator on a value in the list.
            //

            //
            // It only makes sense to find a non-null value.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Ptr = (PVOID) pThis->Find(std::string(Index)).release();

                //
                // Get the ListIterator type and return it.
                //

                ListIterator::TypeDescriptor(0, typeVar);
                Dest.Type = typeVar.Type;
            }
            break;

        case ListMembers::Head:
            //
            // Return the current element at the head or FALSE if there
            // isn't one.
            //

            if (pThis->Head(&pItem))
            {
                Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(pItem->c_str(), pItem->size() + 1);
                Dest.Type = pStringType;
            }
            break;

        case ListMembers::Tail:
            //
            // Return the current element at the tail or FALSE if there
            // isn't one.
            //

            if (pThis->Tail(&pItem))
            {
                Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(pItem->c_str(), pItem->size() + 1);
                Dest.Type = pStringType;
            }
            break;

        case ListMembers::CountOf:
            //
            // How many times does an item occur in the list?
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->CountOf(std::string(Index));
                Dest.Type = pIntType;
            }
            break;

        case ListMembers::Delimiter:
            //
            // Set the list delimiter, returning the old delimiter.
            //

            if (NOT_EMPTY(Index))
            {
                auto old_delimiter = pThis->Delimiter(std::string(Index));
                Dest.Ptr = (PVOID)pThis->m_Buffer.SetBuffer(old_delimiter.c_str(), old_delimiter.size() + 1);
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
// Used by the testing framework to call GetMember through the
// source variable.
//

bool List::GetMemberInvoker(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest)
{
    List * pThis;

    DebugSpew("List::GetMemberInvoker %s", Member);

    pThis = reinterpret_cast<List *>(VarPtr.Ptr);
    if (pThis == nullptr)
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
    rc = Conversions::ToString(pThis->Count(), (char *) Destination, BUFFER_SIZE - 1);
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

    if ((pDest != nullptr) && NOT_EMPTY(Source))
    {
        pDest->Append(std::string(Source));
    }

    return true;
}

//
// Return an iterator on the list.
//

std::unique_ptr<ValueIterator<std::list<std::string>>> List::GetNewIterator(
                    const std::list<std::string> & refCollection) const
{
    return std::make_unique<ListIterator>(refCollection);
}

//
// Return an iterator for a position index.  Position 0 means
// the beginning of the list.  If position is beyond the end
// of the list, return m_coll.end().
//

std::list<std::string>::const_iterator List::FindIteratorForPosition(size_t position) const
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
// Retrieve the index'th value from the list.  Return true if index
// is in the bounds of the list and false otherwise.  Index can
// be negative, in which case it is an offset from the end of the
// list.
//

bool List::Item(const std::string & index, const std::string ** const item) const
{
    size_t lIndex;

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

    return Item(lIndex, item);
}

//
// Insert a sequence of items starting at index into the current list. 
// Return true if the items could be inserted and false otherwise.
//

bool List::Insert(const std::string & args)
{
    size_t lIndex;

    //
    // args must be of the form Integer , Sequence.
    //
    // Find the , and then parse out the integer sequence.
    //

    auto comma_pos = args.find_first_of(',');
    if ((comma_pos == std::string::npos) || (comma_pos == 0))
    {
        //
        // No comma or no comma is at the start of the string.
        //

        return false;
    }

    //
    // Acquire the index value.
    //

    if (!IndexValueFromString(args.substr(0, comma_pos), &lIndex))
    {
        return false;
    }

    if ((comma_pos + 1) == args.size())
    {
        //
        // There aren't any strings to insert. This is a call of the form
        // Integer , Sequence where Sequence is empty, which inserts
        // nothing.
        //

        return true;
    }

    //
    // Split the string into extents.  Each extent represents an argument
    // to insert.
    //

    auto arguments = std::make_unique<StringExtensions>(args.substr(comma_pos + 1));
    auto coll = arguments->Split(StringExtensions::string_type(m_delimiter));

    return Insert(lIndex, *coll);
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

    auto arguments = std::make_unique<StringExtensions>(args);
    auto coll = arguments->Split(StringExtensions::string_type(m_delimiter));

    std::for_each(coll->cbegin(),
                  coll->cend(),
                  [this] (const std::string & item)
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
    size_t lIndex;

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

    return Erase(lIndex);
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

    auto arguments = std::make_unique<StringExtensions>(args);
    auto coll = arguments->Split(StringExtensions::string_type(","));

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
// Create a splice from a set of arguments. The arguments can be of
// the following forms:
//
//      Argument            Result
//      []                  Entire list is returned.
//      [start]             List from start to end is returned.
//      [start, length]     List from start composed of at most length
//                          entries is returned.
//

std::unique_ptr<List> List::CreateSplice(const std::string & args) const
{
    //
    // Default iterators are beginning and ending.
    //

    auto start_it = m_coll.cbegin();
    auto end_it = m_coll.cend();

    //
    // Split the strings and find out how many arguments there are, removing empty
    // strings from the output.
    //

    auto arguments = std::make_unique<StringExtensions>(args);

    //
    // Trim the string. Then if it is not empty, parse it into items.
    //

    auto trimmed_string = arguments->Trim();
    if (!trimmed_string->Contents().empty())
    {
        auto coll = trimmed_string->Split(StringExtensions::string_type(","));

        //
        // Update the iterators if there are arguments.
        //

        size_t startIndex = Count();            // Default start is the end.
        size_t endIndex = Count();              // Default end is the end.

        //
        // There is at least a starting index.
        if (coll->size() > 0)
        {
            //
            // Select from index to end if index is valid. Otherwise use the end as the
            // starting element.
            //

            if (IndexValueFromString((*coll)[0], &startIndex))
            {
                start_it = FindIteratorForPosition(startIndex);
            }
            else
            {
                start_it = m_coll.cend();
            }
        }

        //
        // Two arguments means there is a start and length. Pick up the length.
        if (coll->size() == 2)
        {
            size_t length;

            //
            // Select the length and add it to the start if it is valid.
            //

            if (IndexValueFromString((*coll)[1], &length))
            {
                end_it = FindIteratorForPosition(startIndex + length);
            }
        }
    }

    std::list<std::string> splice(start_it, end_it);
    return std::make_unique<List>(splice);
}

//
// Convert an index value from a string.  Return true if the
// string could be converted and false otherwise.
//

bool List::IndexValueFromString(const std::string & stringIndex, size_t * longIndex) const
{
    size_t lIndex;

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
        // Fail if the index is past the end of the list. If it is equal to the
        // end, treat this as an append.
        //

        if ((size_t) lIndex > Count())
        {
            return false;
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
