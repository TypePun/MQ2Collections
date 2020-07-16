
//
// Implementation file for SetIterator and Set.
//


#include "Set.h"
#include "Conversions.h"
#include "StringExtensions.h"
#include "Macros.h"

using namespace Extensions::Strings;
using namespace Collections::Containers;

//
// Initialize the member name map for the set iterator.  Last entry must
// contain a null string pointer.
//

const MQTypeMember SetIterator::SetIteratorMembers[] =
{
    { (DWORD) SetIteratorMembers::Reset, "Reset" },
    { (DWORD) SetIteratorMembers::Advance, "Advance" },
    { (DWORD) SetIteratorMembers::IsEnd, "IsEnd" },
    { (DWORD) SetIteratorMembers::Value, "Value" },
    { (DWORD) SetIteratorMembers::Clone, "Clone" },
    { 0, 0 }
};

//
// Initialize the member name map for the set.  Last entry must contain a null
// string pointer.
//

const MQTypeMember Set::SetMembers[] =
{
    { (DWORD) SetMembers::Count, "Count" },
    { (DWORD) SetMembers::Clear, "Clear" },
    { (DWORD) SetMembers::Contains, "Contains" },
    { (DWORD) SetMembers::Add, "Add" },
    { (DWORD) SetMembers::Remove, "Remove" },
    { (DWORD) SetMembers::First, "First" },
    { (DWORD) SetMembers::Find, "Find" },
    { 0, 0 }
};

//
// SetIterator methods.
//
            //
            // Constructor.
            //

SetIterator::SetIterator(const std::set<std::string> & refCollection)
        : ValueIterator<std::set<std::string>>(refCollection),
          ReferenceType(SetIteratorMembers)
{
}

//
// Constructor - find a particular element, position to the end
// if the element does not exist.
//

SetIterator::SetIterator(
                const std::set<std::string> & refCollection,
                const std::string & refKey)
        : ValueIterator<std::set<std::string>>(refCollection),
          ReferenceType(SetIteratorMembers)
{
    //
    // Position the iterator to the item or to the end of the
    // set.
    //

    Find(refKey);
}

//
// Copy constructor for an existing set iterator.
//

SetIterator::SetIterator(const SetIterator & original)
        : ValueIterator<std::set<std::string>>(original),
          ReferenceType(SetIteratorMembers)
{
}

//
// Destructor.
//

SetIterator::~SetIterator()
{
}

//
// Return the name of this type - setiterator.
//

const char *SetIterator::GetTypeName()
{
    return "setiterator";
}

//
// Cloned iterators can be deleted.
//

const bool SetIterator::CanDelete() const
{
    return Cloned();
}

//
// Clone this iterator, creating a new one.
//

std::unique_ptr<SetIterator> SetIterator::Clone() const
{
    return std::make_unique<SetIterator>(*this);
}

//
// Return the value in the set under the current iterator.
//

bool SetIterator::Value(const std::string ** const item) const
{
    //
    // Return false if we are after the end of the set.
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

bool SetIterator::GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest)
{
    SetIterator *pThis;
    MQTypeVar typeVar;
    const std::string *pItem;

    //
    // Default return value is FALSE.
    //

    Dest.Int = 0;
    Dest.Type = mq::datatypes::pBoolType;

    //
    // Map the member name to the id.
    //

    auto pMember = SetIterator::FindMember(Member);
    if (pMember == nullptr)
    {
        //
        // No such member.
        //

        return false;
    }

    //
    // Member ID is an SetIteratorMembers enumeration.
    //

    pThis = reinterpret_cast<SetIterator *>(VarPtr.Ptr);
    if (pThis == nullptr)
    {
        return false;
    }

    switch ((enum class SetIteratorMembers) pMember->ID)
    {
        case SetIteratorMembers::Reset:
            //
            // Reset the iterator to the start of the set.   Return the result as
            // TRUE.
            //

            pThis->Reset();

            Dest.Int = 1;
            break;

        case SetIteratorMembers::Advance:
            //
            // Advance the iterator.  Return TRUE if the iterator could be advanced
            // and FALSE otherwise.
            //

            Dest.Int = (int) pThis->Advance();
            break;

        case SetIteratorMembers::IsEnd:
            //
            // Return TRUE if we are at the last element in the set and FALSE
            // otherwise.
            //

            Dest.Int = (int) pThis->IsEnd();
            break;

        case SetIteratorMembers::Value:
            //
            // Return the current element under the iterator or FALSE if there
            // isn't one.
            //

            if (pThis->Value(&pItem))
            {
                Dest.Ptr = (PVOID) pThis->m_Buffer.SetBuffer(pItem->c_str(), pItem->size() + 1);
                Dest.Type = mq::datatypes::pStringType;
            }
            break;

        case SetIteratorMembers::Clone:
            //
            // Clone the iterator.
            //

            Dest.Ptr = (PVOID)pThis->Clone().release();

            //
            // Get the SetIterator type and return it.
            //

            SetIterator::TypeDescriptor(0, typeVar);
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
// Convert the iterator to a string.  Output the current item or FALSE if there
// isn't one.
//

bool SetIterator::ToString(MQVarPtr VarPtr, PCHAR Destination)
{
    SetIterator *pThis;
    const std::string *item;

    pThis = reinterpret_cast<SetIterator *>(VarPtr.Ptr);
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

bool SetIterator::FromString(MQVarPtr&VarPtr, const char* Source)
{
    return false;
}

//
// Return an iterator on the set for a particular key.  Return
// false if the key is not found.
//

bool SetIterator::Find(const std::string & refKey)
{
    m_iterator = m_refCollection.find(refKey);

    //
    // Key was not in the collection.
    //

    return m_iterator != m_refCollection.end();
}

//
// Constructor.
//

Set::Set()
    :ObjectType(SetMembers)
{
}

//
// Destructor.
//

Set::~Set()
{
}

//
// Return the name of this type - set.
//

const char *Set::GetTypeName()
{
    return "set";
}

//
// Return true if a key is in the collection.
//

bool Set::Contains(const std::string &key) const
{
    return m_coll.find(key) != m_coll.end();
}

//
// Add a new element to the set.
//

void Set::Add(const std::string &item)
{
    m_coll.insert(item);
}

//
// Remove an element from the set.  Return false if the item was not
// in the set.
//

bool Set::Remove(const std::string &item)
{
    if (!Contains(item))
    {
        return false;
    }

    m_coll.erase(item);
    return true;
}

//
// Add a sequence of items to the set.
//

void Set::AddItems(const std::string &items)
{
    //
    // Split the string into extents.  Each extent represents an argument
    // to insert.  An empty collection will append no strings.
    //

    auto arguments = std::make_unique<StringExtensions>(items);
    auto coll = arguments->Split(StringExtensions::string_type(","));

    std::for_each(coll->cbegin(),
        coll->cend(),
        [this](const std::string & item)
        {
            Add(item);
        }
    );
}

//
// Return an iterator to a requested key or to the end of the set.
//

ValueIterator<std::set<std::string>> * Set::Find(const std::string & refKey)
{
    m_findIter = std::make_unique<SetIterator>(m_coll, refKey);

    return m_findIter.get();
}

//
// When a member function is called on the type, this method is called.
// It returns true if the method succeeded and false otherwise.
//

bool Set::GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest)
{
    Set *pThis;
    MQTypeVar iteratorTypeVar;
    std::string value;

    //
    // Default return value is FALSE.
    //

    Dest.Int = 0;
    Dest.Type = mq::datatypes::pBoolType;

    //
    // Map the member name to the id.
    //

    auto pMember = Set::FindMember(Member);
    if (pMember == nullptr)
    {
        //
        // No such member.
        //

        return false;
    }

    //
    // Member ID is an SetMembers enumeration.
    //

    pThis = reinterpret_cast<Set *>(VarPtr.Ptr);
    if (pThis == nullptr)
    {
        return false;
    }

    switch ((enum class SetMembers) pMember->ID)
    {
        case SetMembers::Count:
            //
            // Count of items in the set.
            //

            Dest.Int = (int) pThis->Count();
            Dest.Type = mq::datatypes::pIntType;
            break;

        case SetMembers::Clear:
            //
            // Clear the set.  Return the result as TRUE.
            //

            pThis->Clear();

            Dest.Int = 1;
            break;

        case SetMembers::Contains:
            //
            // Does the set contain a key?
            //

            //
            // Check for a valid Index value.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->Contains(std::string(Index));
            }
            break;

        case SetMembers::Add:
            //
            // Add an item to the set.  Return TRUE if the item was added.
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
                    pThis->AddItems(value);
                    Dest.Int = 1;
                }
            }
            break;

        case SetMembers::Remove:
            //
            // Remove an item from the set.  Return TRUE if it was removed and
            // FALSE otherwise.
            //

            //
            // Only attempt a remove if there is a string.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Int = (int) pThis->Remove(std::string(Index));
            }
            break;

        case SetMembers::First:
            //
            // Return an iterator on the first element.
            //

            Dest.Ptr = (PVOID) pThis->First();

            //
            // Get the SetIterator type and return it.
            //

            SetIterator::TypeDescriptor(0, iteratorTypeVar);
            Dest.Type = iteratorTypeVar.Type;
            break;

        case SetMembers::Find:
            //
            // Return an iterator on a key.
            //

            //
            // It only makes sense to find a non-null key.
            //

            if (NOT_EMPTY(Index))
            {
                Dest.Ptr = (PVOID) pThis->Find(std::string(Index));

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

bool Set::ToString(MQVarPtr VarPtr, PCHAR Destination)
{
    Set *pThis;

    pThis = reinterpret_cast<Set *>(VarPtr.Ptr);
    if (Destination == nullptr)
    {
        return false;
    }

    return Conversions::ToString(pThis->Count(), Destination, BUFFER_SIZE) == 0;
}

//
// This method is executed when the /varset statement is executed.  Treat
// this as a set Add call.
//

bool Set::FromString(MQVarPtr &VarPtr, const char* Source)
{
    Set *pDest;

    pDest = reinterpret_cast<Set *>(VarPtr.Ptr);

    //
    // Don't add null or empty strings!
    //

    if ((pDest != nullptr) && NOT_EMPTY(Source))
    {
        pDest->Add(std::string(Source));
    }

    return true;
}

//
// Return an iterator on the set.
//

std::unique_ptr<ValueIterator<std::set<std::string>>> Set::GetNewIterator(
    const std::set<std::string> & refCollection) const
{
    return std::make_unique<SetIterator>(refCollection);
}

