// Unless explicitly stated otherwise all files in this repository are licensed
// under the Apache License Version 2.0.
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2019 Datadog, Inc.
#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>


#include <datadog_agent_six.h>
#include <six.h>

#if __linux__
#include <dlfcn.h>
#define DATADOG_AGENT_TWO "libdatadog-agent-two.so"
#define DATADOG_AGENT_THREE "libdatadog-agent-three.so"
#elif __APPLE__
#include <dlfcn.h>
#define DATADOG_AGENT_TWO "libdatadog-agent-two.dylib"
#define DATADOG_AGENT_THREE "libdatadog-agent-three.dylib"
#elif _WIN32
#define DATADOG_AGENT_TWO "datadog-agent-two.dll"
#define DATADOG_AGENT_THREE "datadog-agent-three.dll"
#else
#error Platform not supported
#endif

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)


#ifdef _WIN32
static HMODULE two, three;
#else
static void *two, *three;
#endif

void init(six_t* six, char* pythonHome)
{
    AS_TYPE(Six, six)->init(pythonHome);
}
#ifdef _WIN32

six_t *make2()
{
    // load library
    two = LoadLibraryA(DATADOG_AGENT_TWO);
    if (!two) {
        std::cerr << "Unable to open 'two' library: " << GetLastError() << std::endl;
        return 0;
    }


    // dlsym class factory
    create_t* create = (create_t*)GetProcAddress(two, "create");
    if (!create) {
        std::cerr << "Unable to open 'two' factory: " << GetLastError() << std::endl;
        return 0;
    }

    return AS_TYPE(six_t, create());
}

void destroy2(six_t* six)
{
    if (two) {
        // dlsym object destructor
        destroy_t* destroy = (destroy_t*)GetProcAddress(two, "destroy");
        if (!destroy) {
            std::cerr << "Unable to open 'two' destructor: " << GetLastError() << std::endl;
            return;
        }
        destroy(AS_TYPE(Six, six));
    }
}

six_t *make3()
{
    // load the library
    three = LoadLibraryA(DATADOG_AGENT_THREE);
    if (!three) {
        std::cerr << "Unable to open 'three' library: " << GetLastError() << std::endl;
        return 0;
    }


    // dlsym class factory
    create_t* create_three = (create_t*)GetProcAddress(three, "create");
    if (!create_three) {
        std::cerr << "Unable to open 'three' factory: " << GetLastError() << std::endl;
        return 0;
    }

    return AS_TYPE(six_t, create_three());
}

void destroy3(six_t* six)
{
    if (three) {
        // dlsym object destructor
        destroy_t* destroy = (destroy_t*)GetProcAddress(three, "destroy");
        
        if (!destroy) {
            std::cerr << "Unable to open 'three' destructor: " << GetLastError() << std::endl;
            return;
        }
        destroy(AS_TYPE(Six, six));
    }
}

#else
six_t *make2()
{
    // load library
    two = dlopen(DATADOG_AGENT_TWO, RTLD_LAZY);
    if (!two) {
        std::cerr << "Unable to open 'two' library: " << dlerror() << std::endl;
        return 0;
    }

    // reset dl errors
    dlerror();

    // dlsym class factory
    create_t* create = (create_t*) dlsym(two, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Unable to open 'two' factory: " << dlsym_error << std::endl;
        return 0;
    }

    return AS_TYPE(six_t, create());
}

void destroy2(six_t* six)
{
    if (two) {
        // dlsym object destructor
        destroy_t* destroy = (destroy_t*) dlsym(two, "destroy");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Unable to open 'two' destructor: " << dlsym_error << std::endl;
            return;
        }
        destroy(AS_TYPE(Six, six));
    }
}

six_t *make3()
{
    // load the library
    three = dlopen(DATADOG_AGENT_THREE, RTLD_LAZY);
    if (!three) {
        std::cerr << "Unable to open 'three' library: " << dlerror() << std::endl;
        return 0;
    }

    // reset dl errors
    dlerror();

    // dlsym class factory
    create_t* create_three = (create_t*) dlsym(three, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Unable to open 'three' factory: " << dlsym_error << std::endl;
        return 0;
    }

    return AS_TYPE(six_t, create_three());
}

void destroy3(six_t* six)
{
    if (three) {
        // dlsym object destructor
        destroy_t* destroy = (destroy_t*) dlsym(three, "destroy");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Unable to open 'three' destructor: " << dlsym_error << std::endl;
            return;
        }
        destroy(AS_TYPE(Six, six));
    }
}
#endif

int is_initialized(six_t* six)
{
    return AS_CTYPE(Six, six)->isInitialized();
}

const char *get_py_version(const six_t* six)
{
    return AS_CTYPE(Six, six)->getPyVersion();
}

int run_simple_string(const six_t* six, const char* code)
{
    return AS_CTYPE(Six, six)->runSimpleString(code);
}

six_pyobject_t* get_none(const six_t* six)
{
    return AS_TYPE(six_pyobject_t, AS_CTYPE(Six, six)->getNone());
}

void add_module_func_noargs(six_t* six, char* moduleName, char* funcName, void* func)
{
    AS_TYPE(Six, six)->addModuleFunction(moduleName, funcName, func, Six::NOARGS);
}

void add_module_func_args(six_t* six, char* moduleName, char* funcName, void* func)
{
    AS_TYPE(Six, six)->addModuleFunction(moduleName, funcName, func, Six::ARGS);
}

void add_module_func_keywords(six_t* six, char* moduleName, char* funcName, void* func)
{
    AS_TYPE(Six, six)->addModuleFunction(moduleName, funcName, func, Six::KEYWORDS);
}
