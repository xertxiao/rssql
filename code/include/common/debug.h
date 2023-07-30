#ifndef _DEBUG_
#define _DEBUG_

// To enable debug mode, compile with flag -D_DM_

#ifdef _DM_
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) {}
#endif

#endif