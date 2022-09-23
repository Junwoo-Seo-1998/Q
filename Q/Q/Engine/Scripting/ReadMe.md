# Cpp_Python_Embedder
A C++17 header-only library for exposing C++ functions and types to Python, thus easing embedding Python for scripting.

## What does it do?
Normally, when you try to expose a function or a type to Python, you need quite large amount of boilerplate codes per function/type.
This library does that on behalf of you.

### Exmaple
Let's say you want to expose a global function, `int g(double, float)` to Python. **Without this library**, you'll need to type:
```c++
#define PY_SSIZE_T_CLEAN
#include <Python.h>


static PyObject* test_g(PyObject* self, PyObject* args)
{
    double d;
    float f;
  
    if (!PyArg_ParseTuple(args, "df", &d, &f))
        return nullptr;
    int toReturn = g(d, f);
    return PyLong_FromLong(toReturn);
}

static PyMethodDef test_methods[] = {
    { "g", &test_g, METH_VARARGS, nullptr },
    { nullptr, nullptr, 0, nullptr }
};

static PyModuleDef test_module = {
    PyModuleDef_HEAD_INIT,
    "test",
    nullptr,
    -1,
    test_methods;
};

PyObject* PyInit_test()
{
    return PyModule_Create(&test_module);
}


int main()
{
    if (PyImport_AppendInittab("test", &PyInit_test) == -1)
        return 1;
  
    // Initialize Python and do something...
}
```

This is too cumbersome, most of them can be automated.

**By using this library**, you can simply write:
```c++
#include "CppPythonEmbedder.hpp"


int main()
{
    PY_EXPORT_GLOBAL_FUNCTION(g, test);
    PY_EXPORT_MODULE(test);
  
    // Initialize Python and do something...
}
```

## Note
It's still being actively developed. Although most of the common features are covered by now, it's still missing some features.

Most importantly, it depends on the following external libraries:
- boost.preprocessor.seq.for_each
- boost.preprocessor.seq.pop_back
- boost.preprocessor.seq.reverse
- xxhash_cx [https://github.com/masyos/xxhash_cx]

I plan to get rid of external dependencies in the future(maybe only boost files, it's too big), but it's not on the priority list now, so you'll need to manually resolve those dependencies.

## How to Use
You'll be using the macros only, since a lot of information should be passed into the template parameters, which can also be automated! :)


### Functions
#### Basics<a name="basics"></a>
- Macros
    - `PY_EXPORT_GLOBAL_FUNCTION(func, moduleName)`: Exports a normal global function.
    - `PY_EXPORT_STATIC_FUNCTION(T, func, moduleName)`: Exports a normal static function. Just a redirection to `PY_EXPORT_GLOBAL_FUNCTION`.
    - `PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION(T, func, instanceReturner, moduleName)`: Exports a member function as a static function. `instanceReturner` will provide the required instance when the function is run.
    - `PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA(T, func, instanceReturner, moduleName)`: Same as above, but the `instanceReturner` is a lambda declared and defined inside the macro. <br /> **Ex)** `PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION_LAMBDA(Vec3, normalize, []() { static Vec3 v; return &v; }, test);`
    - `PY_EXPORT_MEMBER_FUNCTION(T, func, moduleName)`: Exports a normal member function. Note that the type should be exported later.

- Parameters
    - `func`: The name of the function.
    - `moduleName`: The name of the module the function will be exported to.
    - `T`: The name of the class.
    - `instanceReturner`: The name(non-lambda) of the function / The function(lambda) that returns an instance type of `T*`.

- Note on `_AS_STATIC_FUNCTION` macros
    - When you call the exported function in Python, you should pass an extra tuple as the first argument of the function, which will be the arguments of the `instanceReturner`. <br /> **Ex)** `exported_function((instanceReturnerArg0,), arg0, arg1)`
    - They're useful when the type couldn't be exported, or the instance can be moved around the memory.

#### Operators
- Macros
    - `PY_EXPORT_GLOBAL_OPERATOR(func, operatorType, moduleName)`: Exports a normal global operator. **NOT YET IMPLEMENTED**
    - `PY_EXPORT_MEMBER_OPERATOR(T, func, operatorType, moduleName)`: Exports a normal member operator. Note that the type should be exported later.

- Parameters
    - `func`: The name of the function.
    - `moduleName`: The name of the module the function will be exported to.
    - `T`: The name of the class.
    - `operatorType`: One from `EOperatorType`.

- Note
    - Currently only numerical operators are supported. (`+, -, *, /, %, +(unary), -(unary), ^, &, |, ~, <<, >>` and their inplace counterparts, integral casting operator and floating-point casting operator)

#### Templates
- Macros
    - All the macros of the [Basics](#basics) part have their template counterparts. (Prefixed with `TEMPLATE_`)

- Additional Parameters
    - `templateParamSeq`: The sequence of parenthesized template parameters for instantiating the template function. <br /> **Ex)** `((int, float, double))((char, std::string, std::vector<short>))`

- Note
    - When you call the exported function in Python, you should pass an extra string as the first argument of the function, which will be used for choosing which instantiated version will be called. The string should exactly match with what you've passed in as the element of the `templateParamSeq`. <br /> **Ex)** `some_func("char, std::string, std::vector<short>", arg0, arg1)`
    - Template operators are not supported yet, although you can export by passing instantiated versions.

#### Pointer-Takers
- Macros
    - All the macros of the [Basics](#basics) part have their pointer-taking counterparts. (Suffixed with `_PTR`)
    - Since `PY_EXPORT_MEMBER_FUNCTION_AS_STATIC_FUNCTION` takes 2 functions, it has 3 different versions.

- Additional Parameters
    - `funcPtr`: A pointer to the function.
    - `instanceReturnerPtr`: A pointer to the instance-returning function.

- Note
    - It is useful when you try to export an overloaded function. <br /> **Ex)** `PY_EXPORT_MEMBER_FUNCTION_PTR(static_cast<float(T::*)(int) const>(&T::OverloadedFunc), add2, test)`

### Types
#### Classes
- Macros
    - `PY_EXPORT_TYPE(T, moduleName, fieldSeq)`: Exports a normal class.
    - `PY_EXPORT_TYPE_1FIELD(T, moduleName, field)`: You need to use it when you export the class only with 1 field exposed.
    - `PY_EXPORT_TYPE_0FIELD(T, moduleName)`: You need to use it when you export the class with no field exposed.

- Parameters
    - `T`: The name of the class.
    - `moduleName`: The name of the module the function will be exported to.
    - `fieldSeq`: The sequence of fields to be exposed. <br /> **Ex)** `(field1)(field2)(field3)`
    - `field`: The name of the field to be exposed. Note that it's not a sequence.

- Note
    - It isn't required that all of the fields to be exposed. The listed fields will be accessible in Python, so if you don't want it to be, don't list it.
    - Template classes are not supported yet, although you can export by passing instantiated versions.

#### Enums
- Macros
    - `PY_EXPORT_ENUM(E, moduleName, enumeratorSeq)`: Exports a normal enum.

- Parameters
    - `E`: The name of the enum.
    - `moduleName`: The name of the module the function will be exported to.
    - `fieldSeq`: The sequence of enumerators to be exposed. <br /> **Ex)** `(ENUMERATOR1)(ENUMERATOR2)`

- Note
    - It isn't required that all of the enumerators to be exposed. The listed enumerators will be accessible in Python, so if you don't want it to be, don't list it.
    - The exported enum will inherit from Python's `enum.IntEnum`.

### Modules
Use `PY_EXPORT_MODULE(moduleName)` to export a complete module.

### Named Counterparts
All the macros above but operators have a counterpart for giving a different name. (Suffixed with `_NAME`)



## Type Requirements
### Classes
#### Class<a name="class"></a>
The class itself should be satisfy the followings:
- default constructible
- copy assignable
- copy constructible

#### Fields<a name="fields"></a>
Plus, all of the **exposed field**s should be one of:
- fundamental type other than `long double`
- enum
- const char*
- std::string

**Note**: `char` will be treated as a number, not a character.

### Functions
#### Parameters
All of the function's parameters should be one of:
- [supported field types](#fields)
- const reference version of supported fields types
- [supported class types](#class)
- const reference version of supported class tyeps
- reference version of supported class types

#### Return Type
The function's return type should be one of:
- void
- [supported field types](#fields)
- [supported class types](#class)
- const reference version of supported class tyeps
- reference version of supported class types

**Note**: `char` will be treated as a charater, not a number.



## Ordering Requirements
### Member Functions
- All member functions must be exported before its owning class.

### Module
- All elements in a module must be exported before the module.


## Complete Example
```c++
(TODO)
```


## Future Plan
### Issues
- Resolve memory leak issue

### Easier Debugging
- More thorough error checking

### Feature Expansion
#### Classes
- Support user defined data types as fields
- Support arrays as fields
- Support template (maybe?)


#### Operators
- Support global operators
- Support comparison operators
- Support call(`()`) and subscript(`[]`) opeartors
- Support template (maybe?)

#### Dependencies
- Remove boost dependency
