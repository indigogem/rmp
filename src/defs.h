#pragma once

#include <stdlib.h>
#include <cstdint>
#include <crtdbg.h>

#define RMP_DEVELOPMENT 1

#define RMP_STATIC_ASSERT( cond, error ) static_assert( cond, error )

#if RMP_DEVELOPMENT
#define RMP_ASSERT( cond ) _ASSERTE( cond )
#else
define RMP_ASSERT(cond) do { (void)sizeof(cond); } while (0)
#endif
