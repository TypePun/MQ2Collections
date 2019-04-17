//
// Set - set of strings for MQ2.
//

#pragma once

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
                Value
            };

            //
            // Constructor.
            //

            explicit SetIterator(const std::set<std::string> & refCollection)
                : ValueIterator<std::set<std::string>>(refCollection),
                  ReferenceType(SetIteratorMembers)
            {
                DebugSpew("SetIterator - %x", this);
            }

            //
            // Constructor - find a particular element, position to the end
            // if the element does not exist.
            //

            explicit SetIterator(
                            const std::set<std::string> & refCollection,
                            const std::string & refKey)
                : ValueIterator<std::set<std::string>>(refCollection),
                  ReferenceType(SetIteratorMembers)
            {
                DebugSpew("SetIterator - %x", this);

                //
                // Position the iterator to the item or to the end of the
                // set.
                //

                Find(refKey);
            }

            //
            // Destructor.
            //

            ~SetIterator()
            {
                DebugSpew("~SetIterator - %x", this);
            }

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            SetIterator(const SetIterator &) = delete;
            const SetIterator &operator=(const SetIterator &) = delete;

            //
            // Return the name of this type - setiterator.
            //

            static const char *GetTypeName()
            {
                return "setiterator";
            }

            //
            // Return the value in the set under the current iterator.
            //

            bool Value(const std::string ** const item) const
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

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest);

            //
            // Convert the set to a string -- output the current item.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  
            //

            bool FromString(MQ2VARPTR &VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the set for a particular key.  Return
            // false if the key is not found.
            //

            bool Find(const std::string & refKey)
            {
                m_iterator = m_refCollection.find(refKey);

                //
                // Key was not in the collection.
                //

                return m_iterator != m_refCollection.end();
            }

        private:

            //
            // Internal character buffer for an iterated item to return.
            //

            BufferManager<char> m_Buffer;

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER SetIteratorMembers[];
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

            Set()
                :ObjectType(SetMembers)
            {
                DebugSpew("Set - %x", this);
            }

            //
            // Destructor.
            //

            ~Set()
            {
                DebugSpew("~Set - %x", this);
            }

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            Set(const Set &) = delete;
            const Set &operator=(const Set &) = delete;

            //
            // Return the name of this type - set.
            //

            static const char *GetTypeName()
            {
                return "set";
            }

            //
            // Return true if a key is in the collection.
            //

            bool Contains(const std::string &key) const
            {
                return m_coll.find(key) != m_coll.end();
            }

            //
            // Add a new element to the set.
            //

            void Add(const std::string &item)
            {
                m_coll.insert(item);
            }

            //
            // Add a sequence of items to the set.
            //

            void AddItems(const std::string &items);

            //
            // Remove an element from the set.  Return false if the item was not
            // in the set.
            //

            bool Remove(const std::string &item)
            {
                if (!Contains(item))
                {
                    return false;
                }

                m_coll.erase(item);
                return true;
            }

            //
            // Return an iterator to a requested key or to the end of the set.
            //

            std::unique_ptr<ValueIterator<std::set<std::string>>> Find(
                            const std::string & refKey) const;

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest);

            //
            // Convert the set to a string -- output the count of items.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  Treat
            // this as a set Add call.
            //

            bool FromString(MQ2VARPTR &VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the set.
            //

            std::unique_ptr<ValueIterator<std::set<std::string>>> GetNewIterator(
                                    const std::set<std::string> & refCollection) const
            {
                return std::make_unique<SetIterator>(refCollection);
            }

        private:

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER SetMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
