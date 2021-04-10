#pragma once

#if not defined(PYMDSPAN_LOG_ENABLED)
#define PYMDSPAN_LOG_ENABLED 0
#endif

#if PYMDSPAN_LOG_ENABLED
#include <stdio.h>
#define PYMDSPAN_LOG(...) printf(__VA_ARGS__);
#else
#define PYMDSPAN_LOG(...)
#endif
