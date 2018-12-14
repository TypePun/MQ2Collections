//
// Map - map (associative store) from a key to a value for MQ2.
//

#pragma once

#if !defined(__MAP__)
#define __MAP__

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

            enum MapIteratorMemberEnums
            {
                xReset = 1,
                xAdvance,
                xIsEnd,
                xValue,
                xKey
            };

            //
            // Constructor.
            //

            explicit MapIterator(const std::map<std::string, std::string> & refCollection)
                : KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>(refCollection),
                ReferenceType(MapIteratorMembers)
            {
                DebugSpew("MapIterator - %x", this);
            }

            //
            // Constructor - find a particular element, position to the end
            // if the element does not exist.
            //

            explicit MapIterator(
                const std::map<std::string, std::string> & refCollection,
                const std::string & refKey
            )
                : KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>(refCollection),
                ReferenceType(MapIteratorMembers)
            {
                DebugSpew("MapIterator - %x", this);

                //
                // Position the iterator to the item or to the end of the
                // set.
                //

                Find(refKey);
            }

            //
            // Destructor.
            //

            ~MapIterator()
            {
                DebugSpew("~MapIterator - %x", this);
            }

            //
            // Return the name of this type - mapiterator.
            //

            static const char *GetTypeName()
            {
                return "mapiterator";
            }

            //
            // Return the value in the map under the current iterator.  
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

                *item = &m_iterator->second;
                return true;
            }

            //
            // Return the key in the map under the current iterator.  
            //

            bool Key(const std::string ** const key) const
            {
                //
                // Return false if we are after the end of the set.
                //

                if (IsEnd())
                {
                    return false;
                }

                *key = &m_iterator->first;
                return true;
            }

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest);

            //
            // Convert the map to a string -- output the current item.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.  
            //

            bool FromString(MQ2VARPTR &VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the map for a particular key.  Return
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
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            MapIterator(const MapIterator &) = delete;
            const MapIterator &operator=(const MapIterator &) = delete;

            //
            // Internal character buffer for an iterated item to return.
            //

            BufferManager<char> m_Buffer;

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER MapIteratorMembers[];

        };

        //
        // A map is a collection that associates a key with a value.  There is
        // only one of each key in the map.
        //

        class Map : public Collection<
            std::map<std::string, std::string>,
            std::string,
            std::string,
            KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>
        >,
            public ObjectType<Map>
        {
        public:

            //
            // MQ2Type Members
            //

            enum MapMemberEnums
            {
                xCount = 1,
                xClear,
                xContains,
                xAdd,
                xRemove,
                xFirst,
                xFind
            };

            //
            // Constructor.
            //

            Map()
                : ObjectType(MapMembers)
            {
                DebugSpew("Map - %x", this);
            }

            //
            // Destructor.
            //

            ~Map()
            {
                DebugSpew("~Map - %x", this);
            }

            //
            // Return the name of this type - map.
            //

            static const char *GetTypeName()
            {
                return "map";
            }

            //
            // Return true if a key is in the collection.
            //

            bool Contains(const std::string &key) const
            {
                return m_coll.find(key) != m_coll.end();
            }

            //
            // Add a new element to the map.  If he key already exists, the
            // value is overwritten.
            //

            void Add(const std::string &key, const std::string &item)
            {
                m_coll[key] = item;
            }

            //
            // Remove an element from the map.  Return false if the item was not
            // in the map.
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
            // Return an iterator to a requested key or to the end of the map.
            //

            KeyValueIterator<std::map<std::string, std::string>, std::string, std::string> * Find(
                const std::string & refKey) const;

            //
            // When a member function is called on the type, this method is called.
            // It returns true if the method succeeded and false otherwise.
            //

            bool GetMember(MQ2VARPTR VarPtr, PCHAR Member, PCHAR Index, MQ2TYPEVAR &Dest);

            //
            // Convert the map to a string -- output the count of items.
            //

            bool ToString(MQ2VARPTR VarPtr, PCHAR Destination);

            //
            // This method is executed when the /varset statement is executed.
            // Ignore the call.
            //

            bool FromString(MQ2VARPTR &VarPtr, PCHAR Source);

        protected:

            //
            // Return an iterator on the map.
            //

            std::unique_ptr<KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>> GetNewIterator(
                const std::map<std::string, std::string> & refCollection
            ) const
            {
                return std::unique_ptr<KeyValueIterator<std::map<std::string, std::string>, std::string, std::string>>(
                    new MapIterator(refCollection)
                    );
            }

        private:

            //
            // Don't permit copy construction and assignment since the MQ2Type does
            // implement them.
            //

            Map(const Map &) = delete;
            const Map &operator=(const Map &) = delete;

            //
            // Add a key and value to the map, returning true if they were
            // added and false otherwise.
            //

            bool AddKeyAndValue(Map * pThis, PCHAR Arguments);

            //
            // Map from member ids onto names.
            //

            static const MQ2TYPEMEMBER MapMembers[];
        };
    }  // namespace Containers
}  // namespace Collections
#endif