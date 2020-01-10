//
// Set - set of strings for MQ2.
//

#pragma once
#include "DebugMemory.h"

#include <string>
#include <set>

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
        // An iterator on a set.
        //

        class SetIterator : public ValueIterator<std::set<std::string>>,
                            public ReferenceType<SetIterator, std::set<std::string>>
        {
        public:
            //
            // MQ2Type Members
            //

            enum class SetIteratorMembers
            {
                Reset = 1,
                Advance,
                IsEnd,
                Value,
                Clone
            };

            //
            // Constructor.
            //

            explicit SetIterator(const std::set<std::string> & refCollection);

            //
            // Constructor - find a particular element, position to the end
            // if the element does not exist.
            //

            explicit SetIterator(
                        const std::set<std::string> & refCollection,
                        const std::string & refKey);

            //
            // Copy constructor for an existing set iterator.
            //

            explicit SetIterator(const SetIterator & original);

            //
            // Destructor.
            //

            ~SetIterator();

            //
            // Don't permit assignment since the MQ2Type does
            // implement it.
            //

            const SetIterator &operator=(const SetIterator &) = delete;

            //
            // Return the name of this type - setiterator.
            //

            static const char *GetTypeName();

            //
            // Cloned iterators can be deleted.
            //

            const bool CanDelete() const;

            //
            // Return the value in the set under the current iterator.
            //

            bool Value(const std::string ** const item) const;

            //
            // Clone this iterator, creating a new one.
            //

            std::unique_ptr<SetIterator> Clone() const;


            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQVarPtr VarPtr, PCHAR Member, PCHAR Index, MQTypeVar& Dest);

            //
            // Convert the set to a string -- output the current item.
            //

            bool ToString(MQVarPtr VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  
            //

            bool FromString(MQVarPtr& VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the set for a particular key.  Return
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

            static const MQTypeMember SetIteratorMembers[];
        };

        //
        // A set is a collection that contains only one of each element. Adding an
        // existing element to the set produces a set with the same elements.
        //

        class Set : public Collection<
                                std::set<std::string>,
                                std::string,
                                std::string,
                                ValueIterator<std::set<std::string>>>,
                    public ObjectType<Set>
        {
        public:

            //
            // MQ2Type Members
            //

            enum class SetMembers
            {
                Count = 1,
                Clear,
                Contains,
                Add,
                Remove,
                First,
                Find
            };

            //
            // Constructor.
            //

            Set();

            //
            // Destructor.
            //

            ~Set();

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            Set(const Set &) = delete;
            const Set &operator=(const Set &) = delete;

            //
            // Return the name of this type - set.
            //

            static const char *GetTypeName();

            //
            // Return true if a key is in the collection.
            //

            bool Contains(const std::string &key) const;

            //
            // Add a new element to the set.
            //

            void Add(const std::string &item);

            //
            // Add a sequence of items to the set.
            //

            void AddItems(const std::string &items);

            //
            // Remove an element from the set.  Return false if the item was not
            // in the set.
            //

            bool Remove(const std::string &item);

            //
            // Return an iterator to a requested key or to the end of the set.
            //

            ValueIterator<std::set<std::string>> * Find(const std::string & refKey);

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQVarPtr VarPtr, PCHAR Member, PCHAR Index, MQTypeVar& Dest);

            //
            // Convert the set to a string -- output the count of items.
            //

            bool ToString(MQVarPtr VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  Treat
            // this as a set Add call.
            //

            bool FromString(MQVarPtr& VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the set.
            //

            std::unique_ptr<ValueIterator<std::set<std::string>>> GetNewIterator(
                        const std::set<std::string> & refCollection) const;

        private:

            //
            // Iterator returned by Find operations.
            //

            std::unique_ptr<SetIterator> m_findIter;

            //
            // Map from member ids onto names.
            //

            static const MQTypeMember SetMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
