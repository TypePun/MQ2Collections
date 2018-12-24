//
// List - list of strings for MQ2.
//

#pragma once

#include <string>
#include <list>

#include "Types.h"
#include "Collections.h"
#include "BufferManager.h"

using namespace Types;
using namespace Collections;
using namespace Utilities::Buffers;

namespace Collections
{
    namespace Containers
    {
        //
        // An iterator on a list.
        //

        class ListIterator : public ValueIterator<std::list<std::string>>,
            public ReferenceType<ListIterator, std::list<std::string>>
        {
        public:
            //
            // MQ2Type Members
            //

            enum class ListIteratorMembers
            {
                Reset = 1,
                Advance,
                IsEnd,
                Value
            };

            //
            // Constructor.
            //

            explicit ListIterator(const std::list<std::string> & refCollection);

            //
            // Constructor - find a particular element, position to the end
            // if the element does not exist.
            //

            explicit ListIterator(
                const std::list<std::string> & refCollection,
                const std::string & refKey);

            //
            // Destructor.
            //

            ~ListIterator();

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            ListIterator(const ListIterator &) = delete;
            const ListIterator &operator=(const ListIterator &) = delete;

            //
            // Return the name of this type - listiterator.
            //

            static const char *GetTypeName();

            //
            // Return the value in the list under the current iterator.
            //

            bool Value(const std::string ** const item) const;

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest);

            //
            // Convert the list to a string -- output the current item.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  
            //

            bool FromString(MQ2VARPTR & VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the list for a particular key.  Return
            // false if the key is not found.
            //

            bool Find(const std::string & refKey);

        private:

            //
            // Internal character buffer for an iterated item to return.
            //

            BufferManager<char> m_Buffer;

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER ListIteratorMembers[];
        };

        //
        // A list is a flexible sequence of items.  Items can be inserted
        // anywhere in the sequence and arbitrary slices may be retrieved.
        //

        class List : public Collection <
                                std::list<std::string>,
                                std::string,
                                std::string,
                                ValueIterator<std::list<std::string>>>,
                    public ObjectType<List>
        {
        public:

            //
            // MQ2Type Members
            //

            enum class ListMembers
            {
                Count = 1,
                Clear,
                Contains,
                Splice,
                Index,
                Item,
                Insert,
                Sort,
                Reverse,
                Append,
                Remove,
                Erase,
                Replace,
                First,
                Find,
                Head,
                Tail,
                CountOf
            };

            //
            // Constructor.
            //

            List();

            //
            // Create a new list from an input std::list.
            //

            List(const std::list<std::string> & source);


            //
            // Destructor.
            //

            ~List();

            //
            // Don't permit copy construction and assignment since the MQ2Type
            // doesn't implement them.
            //

            List(const List &) = delete;
            const List &operator=(const List &) = delete;


            //
            // Return the name of this type - list.
            //

            static const char *GetTypeName();

            //
            // Return true if an item is in the list.
            //

            bool Contains(const std::string & item) const;

            //
            // Return a splice of a list from a starting index to the end.  If position is beyond
            // the end of the list, an empty list is returned.
            //

            std::unique_ptr<List> Splice(size_t index) const;

            //
            // Create a splice from a starting to an ending offset.
            //

            std::unique_ptr<List> Splice(size_t startIndex, size_t length) const;

            //
            // Return the index of an item in the list.  -1 is returned if the
            // item is not found.  The index is 0 based.  If multiple items
            // match item then the index of only the first is returned.
            //

            long Index(const std::string & item) const;

            //
            // Retrieve the item at position index in the list.  True is
            // returned if the item exists and false otherwise.
            //

            bool Item(const size_t index, const std::string ** const item) const;

            //
            // Insert a sequence from one container into this list before a
            // position.  Position 0 means before the start of the list.
            // Return true if we can insert the sequence and false otherwise.
            //

            template <class Container>
            bool Insert(const size_t position, const Container & sequence)
            {
                //
                // Don't insert if position is beyond the end of the list.
                //

                if (position > Count())
                {
                    return false;
                }

                //
                // Find an iterator corresponding to the position.
                //

                auto it = FindIteratorForPosition(position);

                //
                // And splice all of the elements of the container into the
                // list before the iterator.
                //

                m_coll.insert(it, sequence.cbegin(), sequence.cend());

                return true;
            }

            //
            // Sort the list into ascending order.  The list is altered in
            // place.
            //

            void Sort();

            //
            // The elements in the list are reversed.  The list is reversed in
            // place.
            //

            void Reverse();

            //
            // Append an item to the end of the list.
            //

            void Append(const std::string & item);

            //
            // Remove an element from the list.   Return a count of the
            // number of items that matched item.
            //

            size_t Remove(const std::string & item);

            //
            // Erase an item at a position in the list.  Return true if the
            // index was in the list and false otherwise.
            //

            bool Erase(const size_t index);

            //
            // Replace an item in the list.  Return a count of the number
            // of items replaced.
            //

            size_t Replace(const std::string & item, const std::string & newItem);

            //
            // Return an iterator to a requested key or to the end of the list.
            //

            std::unique_ptr<ValueIterator<std::list<std::string>>> Find(
                const std::string & refKey) const;

            //
            // Remove and return the head of the list.  Return true if there
            // was a head and false otherwise.
            //

            bool Head(std::unique_ptr<const std::string> * item);

            //
            // Remove and return the tail of the list.  Return true if there
            // was a tail and false otherwise.
            //

            bool Tail(std::unique_ptr<const std::string> * item);

            //
            // Return a count of how many times item occurs in the list.
            //

            size_t CountOf(const std::string & item) const;

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest);

            //
            // Convert the list to a string -- output the count of items.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  Treat
            // this as a list Append call.
            //

            bool FromString(MQ2VARPTR & VarPtr, PCHAR Source);

            //
            // Used by the testing framework to call GetMember through the
            // source variable.
            //

            static bool GetMemberInvoker(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR & Dest);

        protected:

            //
            // Return an iterator on the list.
            //

            std::unique_ptr<ValueIterator<std::list<std::string>>> GetNewIterator(
                const std::list<std::string> & refCollection) const;

        private:

            //
            // Retrieve the index'th value from the list.  Return true if index
            // is in the bounds of the list and false otherwise.
            //

            bool Item(const std::string & index, const std::string ** const item) const;

            //
            // Insert a sequence of items starting at index into the current
            // list.  Return true if the items could be inserted and false otherwise.
            //

            bool Insert(const std::string & args);

            //
            // Append a sequence of items onto the end of the list.
            //

            void AppendItems(const std::string & args);

            //
            // Erase an index in the list.  Return true if the item was erased (deleted)
            // and false otherwise.
            //

            bool Erase(const std::string & index);

            //
            // Replace a source item with a target item.  The argument must
            // be a list of two items.  Count is set to the number of times
            // the source was replaced by the target.  True is returned if
            // there are two arguments and false otherwise.
            //

            bool Replace(const std::string & args, size_t * count);

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

            std::unique_ptr<List> CreateSplice(const std::string & args) const;

            //
            // Return an iterator for a position index.  Position 0 means
            // the beginning of the list.  If position is beyond the end
            // of the list, return m_coll.end().
            //

            std::list<std::string>::const_iterator FindIteratorForPosition(size_t position) const;

            //
            // Convert an index value from a string.  Return true if the
            // string could be converted and false otherwise.
            //

            bool IndexValueFromString(const std::string & stringIndex,
                                      size_t * longIndex) const;

            //
            // Internal character buffer for a returned item.
            //

            BufferManager<char> m_Buffer;

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER ListMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
