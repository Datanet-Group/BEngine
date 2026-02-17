#pragma once
#include "Allocator.h"

// Returns a process-wide default allocator instance (owned by the module).
IMalloc* CreateDefaultAllocator();
