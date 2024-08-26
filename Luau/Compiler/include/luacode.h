// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#pragma once

#include <stddef.h>

// Can be used to reconfigure visibility/exports for public APIs
#ifndef LUACODE_API
#define LUACODE_API extern
#endif

typedef struct lua_CompileOptions lua_CompileOptions;

struct lua_CompileOptions
{
    int optimizationLevel;
    int debugLevel;
    int typeInfoLevel;
    int coverageLevel;
    const char* vectorLib;
    const char* vectorCtor;
    const char* vectorType;
    const char* const* mutableGlobals;
    const char* const* userdataTypes;
};

LUACODE_API char* luau_compile(const char* source, size_t size, lua_CompileOptions* options, size_t* outsize);
