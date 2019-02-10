
#include "MQ2CollectionsImpl.h"

#include "Types.h"
#include "Set.h"
#include "Stack.h"
#include "Queue.h"
#include "Map.h"
#include "List.h"

using namespace Collections::Containers;
using namespace Types;

//
// Initialize MQ2 types used by the plugin.
//

void RegisterTypes(void)
{
    SetIterator::RegisterType();
    Set::RegisterType();
    Stack::RegisterType();
    Queue::RegisterType();
    MapIterator::RegisterType();
    Map::RegisterType();
    ListIterator::RegisterType();
    List::RegisterType();
}

//
// Remove/Tear down MQ2 types used by the plugin.
//

void UnregisterTypes(void)
{
    List::UnregisterType();
    ListIterator::UnregisterType();
    Map::UnregisterType();
    MapIterator::UnregisterType();
    Queue::UnregisterType();
    Stack::UnregisterType();
    Set::UnregisterType();
    SetIterator::UnregisterType();
}
