
//
// Allocate the static type and class maps.
//

#include "Types.h"

using namespace Types;

//
// Map from type names onto a single instance of the type that indicates
// whether a variable is of that type.
//

std::map<std::string, MQ2Type *> TypeMap::typeMap;

//
// Add a new type name and type instance to the type map.  If the
// type name already exists, return false.  Otherwise, return true.
//

bool TypeMap::AddType(const char *typeName, MQ2Type *typeInstance)
{
	std::string sTypeName(typeName);

	//
	// If the entry already exists, don't add it.
	//

	if (typeMap.find(sTypeName) == typeMap.end())
	{
		typeMap[sTypeName] = typeInstance;
		return true;
	}

	return false;
}

//
// Return true if a name is in the map and false otherwise.
//

bool TypeMap::HasName(const char *typeName)
{
	return typeMap.find(std::string(typeName)) != typeMap.end();
}

//
// Return the type instance for a type name or null if it was never
// registered.
//

MQ2Type * TypeMap::GetTypeInstanceForTypeName(const char *typeName)
{
	std::string sTypeName(typeName);
	std::map<std::string, MQ2Type *>::iterator itTypes;

	//
	// Look up the instance in the map from type names onto instances.
	//

	itTypes = typeMap.find(sTypeName);
	if (itTypes == typeMap.end())
	{
		//
		// The type was never registered.
		//

		return 0;
	}

	return itTypes->second;
}

//
// Remove the typename and delete the type instance.  Return true if
// the type name was found and removed and false otherwise.
//

bool TypeMap::RemoveType(const char *typeName)
{
	std::string sTypeName(typeName);
	std::map<std::string, MQ2Type *>::iterator it;

	//
	// See if the type name exists in the map.  If it does, remove
	// it and return true.
	//

	it = typeMap.find(sTypeName);

	//
	// If the entry already exists, don't add it.
	//

	if (it != typeMap.end())
	{
		//
		// Remove the type from the map.
		//

		delete it->second;
		typeMap.erase(it);
		return true;
	}

	return false;
}
