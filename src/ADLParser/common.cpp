#define ALLOCATE_STORAGE
#include "common.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
