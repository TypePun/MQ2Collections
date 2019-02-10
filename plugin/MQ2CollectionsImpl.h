//
// Declare implementation methods for MQ2Collections. These methods are
// declared here to move their implementation into a separate file to
// avoid circular compilation errors in the MQ2 code.
//

#pragma once

//
// Initialize MQ2 types used by the plugin.
//

void RegisterTypes(void);

//
// Remove/Tear down MQ2 types used by the plugin.
//

void UnregisterTypes(void);
