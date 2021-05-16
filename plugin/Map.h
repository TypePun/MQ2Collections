//
// Map - map (associative store) from a key to a value for MQ2.
//

#pragma once
#include "DebugMemory.h"

#include <string>
#include <map>

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
        // An iterator on a map.
        //

        class MapIterator : public KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>,
                            public ReferenceType<MapIterator, std::map<std::string, std::string>>
        {
        public:
            //
            // MQ2Type Members
            //

            enum class MapIteratorMembers
            {
                Reset = 1,
                Advance,
                IsEnd,
                Value,
                Key,
                Clone
            };

            //
            // Constructor.
            //

            explicit MapIterator(const std::map<std::string, std::string> & refCollection);

            //
            // Constructor - find a particular element, position to the end
            // if the element does not exist.
            //

            explicit MapIterator(
                            const std::map<std::string, std::string> & refCollection,
                            const std::string & refKey);

            //
            // Copy Constructor from an existing iterator.
            //

            explicit MapIterator(const MapIterator & original);

            //
            // Destructor.
            //

            ~MapIterator();

            //
            // Don't permit assignment since the MQ2Type does
            // implement it.
            //

            const MapIterator &operator=(const MapIterator &) = delete;

            //
            // Return the name of this type - mapiterator.
            //

            static const char *GetTypeName();

            //
            // Cloned iterators can be deleted.
            //

            const bool CanDelete() const;

            //
            // Return the value in the map under the current iterator.  
            //

            bool Value(const std::string ** const item) const;

            //
            // Return the key in the map under the current iterator.  
            //

            bool Key(const std::string ** const key) const;

            //
            // Clone this iterator, creating a new one.
            //

            std::unique_ptr<MapIterator> Clone() const;

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar &Dest) override;

            //
            // Convert the map to a string -- output the current item.
            //

            bool ToString(MQVarPtr VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  
            //

            virtual bool FromString(MQVarPtr &VarPtr, const char* Source) override;

        protected:

            //
            // Return an iterator on the map for a particular key.  Return
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

            static const MQTypeMember MapIteratorMembers[];

        };

        //
        // A map is a collection that associates a key with a value.  There is
        // only one of each key in the map.
        //

        class Map : public Collection<
                            std::map<std::string, std::string>,
                            std::string,
                            std::string,
                            KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>>,
                    public ObjectType<Map>
        {
        public:

            //
            // MQ2Type Members
            //

            enum class MapMembers
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

            Map();

            //
            // Destructor.
            //

            ~Map();

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            Map(const Map &) = delete;
            const Map &operator=(const Map &) = delete;

            //
            // Return the name of this type - map.
            //

            static const char *GetTypeName();

            //
            // Return true if a key is in the collection.
            //

            bool Contains(const std::string &key) const;

            //
            // Add a new element to the map.  If he key already exists, the
            // value is overwritten.
            //

            void Add(const std::string &key, const std::string &item);

            //
            // Remove an element from the map.  Return false if the item was not
            // in the map.
            //

            bool Remove(const std::string &item);

            //
            // Return an iterator to a requested key or to the end of the map.
            //

            KeyValueIterator<std::map<std::string, std::string>, std::string, std::string> * Find(
                                const std::string & refKey);

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest) override;

            //
            // Convert the map to a string -- output the count of items.
            //

            bool ToString(MQVarPtr VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.
            // Ignore the call.
            //

            virtual bool FromString(MQVarPtr& VarPtr, const char* Source) override;

        protected:

            //
            // Return an iterator on the map.
            //

            std::unique_ptr<KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>> GetNewIterator(
                    const std::map<std::string, std::string> & refCollection) const;

        private:

            //
            // Add a key and value to the map, returning true if they were
            // added and false otherwise.
            //

            bool AddKeyAndValue(Map * pThis, PCHAR Arguments);

            //
            // Iterator returned by Find operations.
            //

            std::unique_ptr<MapIterator> m_findIter;

            //
            // Map from member ids onto names.
            //

            static const MQTypeMember MapMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
