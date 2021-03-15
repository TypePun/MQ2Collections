//
// Provide classes and interfaces for interfacing with the MQ2 type system.
//

#pragma once
#include "DebugMemory.h"

#include <string>
#include <map>
#include <typeinfo>

#include <mq/Plugin.h>

#include "Macros.h"

namespace Types
{
    //
    // Map from type names onto their ObjectTypes.
    //

    class TypeMap
    {
    public:
        //
        // Add a new type name and type instance to the type map.  If the
        // type name already exists, return false.  Otherwise, return true.
        //

        static bool AddType(const char * typeName, MQ2Type * typeInstance);

        //
        // Return true if a name is in the map and false otherwise.
        //

        static bool HasName(const char * typeName);

        //
        // Return the type instance for a type name or null if it was never
        // registered.
        //

        static MQ2Type * GetTypeInstanceForTypeName(const char * typeName);

        //
        // Remove the typename and delete the type instance.  Return true if
        // the type name was found and removed and false otherwise.
        //

        static bool RemoveType(const char * typeName);

    private:

        //
        // One and only map of type names to type instances.
        //

        static std::map<std::string, std::unique_ptr<MQ2Type>> typeMap;
    };

    //
    // BaseType is an interface derived from MQ2Type which provides default
    // functionality based on the MQ2Type interface.
    //

    template <typename ObjectClass>
    class BaseType : public MQ2Type
    {
    public:

        // Buffer pointed to by naked PCHAR is at least this long.

        const size_t BUFFER_SIZE = MAX_STRING;

        //
        // Initialize our methods and properties in MQ2.
        //

        BaseType(const MQTypeMember aMembers[])
            : MQ2Type(const_cast<char *>(ObjectClass::GetTypeName()))
        {
            InitializeMembers(const_cast<MQTypeMember*>(aMembers));
        }

        //
        // Ensure derived class destructors are called when we are deleted.
        //

        virtual ~BaseType()
        {
        }

        //
        // By default, this type can be deleted.
        //

        virtual const bool CanDelete() const
        {
            return true;
        }

        //
        // This method is executed when the /vardata statement is executed on
        // the type.  Delete the destination instance and set the source
        // instance to it.
        //

        bool FromData(MQVarPtr& VarPtr, const MQTypeVar& Source) override
        {
            ObjectClass * pDest;

            if (Source.Type != TypeMap::GetTypeInstanceForTypeName(ObjectClass::GetTypeName()))
            {
                return false;
            }

            pDest = reinterpret_cast<ObjectClass *>(VarPtr.Ptr);

            //
            // Delete the destination object if it is not the same as the
            // source object and set the destination to the source.
            //

            if (pDest != Source.Ptr)
            {
                if (CanDelete())
                {
                    delete pDest;
                }
            }

            VarPtr.Ptr = Source.Ptr;

            return true;
        }

        //
        // Delete an instance of ObjectClass.
        //

        virtual void FreeVariable(MQVarPtr& VarPtr)
        {
            if (CanDelete())
            {
                delete reinterpret_cast<ObjectClass *>(VarPtr.Ptr);
            }

            VarPtr.Ptr = 0;
        }

        //
        // The default implementation of GetMember returns false -- there
        // aren't any members on the type.
        //

        virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest) override
        {
            return false;
        }

        //
        // Register the type with MQ2, using an instance as our unique type
        // descriptor.
        //

        static void RegisterType(ObjectClass * instance)
        {
            //
            // If we were already registered, do nothing.
            //

            if (!TypeMap::HasName(ObjectClass::GetTypeName()))
            {
                //
                // Associate the type with an instance -- this lets
                // us compare the type information.
                //

                TypeMap::AddType(ObjectClass::GetTypeName(), instance);
                AddMQ2Data(const_cast<char *>(ObjectClass::GetTypeName()), TypeDescriptor);
            }
        }

        //
        // Unregister the type with MQ2.
        //

        static void UnregisterType()
        {
            //
            // If we were never registered, do nothing.
            //

            if (TypeMap::HasName(ObjectClass::GetTypeName()))
            {
                //
                // Remove the type name from MQ2 and unregister the class name
                // and type name.
                //

                RemoveMQ2Data(const_cast<char *>(ObjectClass::GetTypeName()));
                TypeMap::RemoveType(ObjectClass::GetTypeName());
            }
        }

        //
        // Return the MQ2 Type.
        //

        static bool TypeDescriptor(const char* szName, MQTypeVar& Dest)
        {
            Dest.DWord = 1;
            Dest.Type = TypeMap::GetTypeInstanceForTypeName(ObjectClass::GetTypeName());

            return TRUE;
        }
    };

    //
    // ReferenceType is a type with methods and properties.  It can not be
    // created by a /declare statement and is returned by a method on
    // an ObjectType.  A ReferenceType takes an argument to its constructor
    // which is a reference to an ObjectClass.
    //

    template <typename ObjectClass, typename Reference>
    class ReferenceType : public BaseType<ObjectClass>
    {
    public:

        //
        // Initialize our methods and properties in MQ2.
        //

        ReferenceType(const MQTypeMember aMembers[])
            : BaseType<ObjectClass>(aMembers)
        {
        }

        //
        // Ensure derived class destructors are called when we are deleted.
        //

        ~ReferenceType()
        {
        }

        //
        // This type cannot be deleted.
        //

        const bool CanDelete() const
        {
            return false;
        }

        //
        // Register the type - reference types have an instance bound to a
        // reference of Reference.
        //

        static void RegisterType()
        {
            Reference *refClass = new Reference();
            BaseType<ObjectClass>::RegisterType(new ObjectClass(*refClass));
        }
    };

    //
    // ObjectType is a type that has methods and properties and can be
    // created by a /declare statement.
    //

    template <typename ObjectClass>
    class ObjectType : public BaseType<ObjectClass>
    {
    public:

        //
        // Initialize our methods and properties in MQ2.
        //

        ObjectType(const MQTypeMember aMembers[])
            : BaseType<ObjectClass>(aMembers)
        {
        }

        //
        // Ensure derived class destructors are called when we are deleted.
        //

        ~ObjectType()
        {
        }

        //
        // Allocate a new instance of this ObjectType.
        //

        virtual void InitVariable(MQVarPtr& VarPtr)
        {
            VarPtr.Ptr = new ObjectClass();
        }

        //
        // Register the type with no arguments.
        //

        static void RegisterType()
        {
            BaseType<ObjectClass>::RegisterType(new ObjectClass());
        }
    };
}
