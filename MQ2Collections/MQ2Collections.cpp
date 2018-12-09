
//
// Define Collection Datatypes on strings for MQ2.
//
// The following types are defined:
//
//	Set			a collection whereby each element can occur in the sequence
//				only once.
//	SetIterator an forward iterator on a set or a range of a set.
//  Stack -		a list where an item is inserted and removed from the same end.
//  Queue -		a list where an item is inserted on the tail and removed from
//				the head.
//
// Each collection has the a base interface called "Collection".  This interface
// defines a common set of methods that the collection must implement.
//

#include <MQ2Plugin.h>

#include "Types.h"
#include "Set.h"
#include "Stack.h"
#include "Queue.h"
#include "Map.h"
#include "List.h"

using namespace Collections::Containers;
using namespace Types;

PreSetup("MQ2Collections");

// Called once, when the plugin is to initialize
PLUGIN_API VOID InitializePlugin(VOID)
{
    DebugSpewAlways("Initializing MQ2Collections");

	SetIterator::RegisterType();
	Set::RegisterType();
	Stack::RegisterType();
	Queue::RegisterType();
	MapIterator::RegisterType();
	Map::RegisterType();
	ListIterator::RegisterType();
	List::RegisterType();
}

// Called once, when the plugin is to shutdown
PLUGIN_API VOID ShutdownPlugin(VOID)
{
	DebugSpewAlways("Shutting down MQ2Collections");

	List::UnregisterType();
	ListIterator::UnregisterType();
	Map::UnregisterType();
	MapIterator::UnregisterType();
	Queue::UnregisterType();
	Stack::UnregisterType();
	Set::UnregisterType();
	SetIterator::UnregisterType();
}
