# PTL (Property Tree Library)
A flexible and feature-rich script format designed for use in scientific computing applications.

Now supporting syntax highlighting for the Atom editor via the [language-ptl](https://github.com/wvannoordt/language-ptl) package
(as well as the [tree-sitter-ptl](https://github.com/wvannoordt/tree-sitter-ptl) parser).

## Description
This project was inspired by extensive work with a number of scientific computing codes.
A typical structure for one of these codes is to have an input file that contains every runtime parameter.
The solver then reads this file and sets parameters accordingly.

The very nature of scientific computing means that every run counts. If you aren't debugging, the code is likely to run for
days, possibly even weeks, at a time. This costs a lot of money and effort, so it makes sense to make sure that every parameter is correct!

Sometimes, however, deadlines can get to us. Small mistakes are made, such as a typo on one input option that causes it to be ignored and/or
set to a default value. In some cases, this can cause an entire simulation to be garbage.

## Purpose
PTL is designed to alleviate symptoms of more primitive user input approaches while making it easier to:

* script multiple runs of your code,

* document the input interface, and

* increase the reliability of your code.

## Features
The development of PTL is an ongoing process and new features are constantly being developed. Currently, the main features of PTL include:

* Lightweight and memory-safe object deserialization

* Support of a large range of basic data types:

    * numerics
    
    * strings
    
    * enumerations
    
    * static / dynamic arrays
    
    * logicals

    * user-defined types

* Automatic documentation (WIP)

* Command-line tools for parsing files externally

* Syntax highlighting through the Atom editor (emacs version WIP)

* Preproccessor definitions and feature-rich scripting format for in-place calculations

* Command-line preproccessor definitions

* C and Fortran interfaces WIP

## Building
PTL is current built with GNU make. A Cmake build system may come in the future.

To build and test, simply run `make test` from the top directory. If successful, the following message will appear following the build output:

```
PTL passed all tests.
```

All header files are placed in `include` and can be included from there, while the bindary is stored in `lib`. The command-line build of a simple application might look like this:

```
g++ -I/path/to/PTL/include main.cc -o program -L/path/to/PTL/lib -lPTL
```

## Usage
There are two diffrent ways to use PTL:

* **Declarative mode**: This is the main way of using PTL. In declarative mode, a receiving structure is defined by the user. File data is read into the tree object and parsed. During the parsing stage, each variable is parsed as the user has defined. If any variable cannot be parsed or if any 

* **Query mode**: this method of using PTL is very simple and straightforward, although does not provide a way of identifying unused variables. (**Note that this is not the recommended usage of PTL, as it does not provide all features.**) Query mode the mode that the C and Fortran interfaces (WIP) use. Query mode is used by simply reading a `.ptl` file into a tree object and querying that object.

### Basic Example
This example shows how to read a simple integer value from a file `input.ptl`:

`main.cc`

```c++
#include "PTL.h"
int main(int argc, char** argv)
{
    PTL::PropertyTree input;
    int myInteger;

    //Bind string "myInteger" to integer myInteger. The string can be any string,
    //not just the variable name. The default value of myInteger is 1. Note the
    //string description is the second argument.
    input["myInteger"].MapTo(&myInteger)
        = new PTL::PTLInteger(1, "Some integer value");

    //This function actually populates the tree structure with string data but
    //does not parse anything. Reading and parsing are kept separate so that the
    //tree can be used to read arbitrary data if need be.
    input.Read("input.ptl");

    //After this function call, myInteger has the value read from the file (5).
    //This function will throw an error if any input option cannot be
    //successfully parsed.
    input.StrictParse();

    //This simply prints every value in the tree to the screen.
    input.DebugPrint();

    //Every resource used by input is cleaned up when the object goes out of
    //scope. The object is memory-safe.
    return 0;
}
```

`input.ptl`

```
myInteger = 5
```

### Supported Input Types
Any input type can be implemented by inheriting the `PTL::InputVariable` class. PTL currently has implementations for:

* `PTL::PTLInteger`: An integer value.

* `PTL::PTLBoolean`: A boolean value.

* `PTL::PTLDouble`: A double-precision value.

* `PTL::PTLString`: A string (`std::string`) value.

* `PTL::PTLEnum`: A string-based enumeration that maps to an integer value. Options are passed to the constructor as a colon-delimited list,
i.e. `"option1:option2:option3"`. It is recommended that each option has some corresponding, meaningful comparison when used to branch.

* `PTL::PTLAutoEnum`: An enumeration defined by an `enum` implementation, e.g. `enum myType{option1,option2}`. Since C++ is not reflective,
this type's constructor also takes a function pointer that returns the string representation of each option, e.g:

```c++
std::string myTypeStr(int i)
{
    switch (i)
    {
        case option1: return "option1";
        case option2: return "option2";
    }
    return PTL_AUTO_ENUM_TERMINATOR;
}
```

* `PTL::PTLDynamicDoubleArray`: An arbitrary-size array of double-precision values. This object maps to two outputs: an integer representing
the size of the array, and a `double*` that is heap-allocated, then deallocated when the relevant `PTL::PropertyTree` object goes out of
scope (**this is very important!**).

* `PTL::PTLStaticDoubleArray`: A fixed-size array of double-precision values. This object maps only to a `double*`, and is assumed to have a
fixed size known to the user at compile time. Other than the fact that this only maps to a single value, this object behaves identically to a
`PTL::PTLDynamicDoubleArray`.

* `PTL::PTLDynamicIntegerArray`: This object is identical to a `PTL::PTLDynamicDoubleArray`, just with an integer element type.

* `PTL::PTLStaticIntegerArray`: This object is identical to a `PTL::PTLStaticIntegerArray`, just with an integer element type.

Other types are still in development as needed, but the reader is encouraged to contribute implementations of more (preferably standard) types.

### User-Defined Types

PTL provids a base class `PTL::PTLGenericObject` for generic objects that can be constructed from a `.ptl` file. The following usage example demonstrates this.
Note that for a class that inherits `PTL::PTLGenericObject`, it is necessary to use the base constructor via the `using PTL::PTLGenericObject::PTLGenericObject` statement, and to define the `Definitions` function. Note that in the current implementation, dynamically-allocated types such as `PTL::PTLDynamicDoubleArray`, `PTL::PTLStaticIntegerArray`, etc. are deallocated when the tree object is destroyed.

`main.cc`

```c++
#include "PTL.h"

class MyClass : public PTL::PTLGenericObject
{
    using PTL::PTLGenericObject::PTLGenericObject; // required
    private:
        std::string strval;
        int intval;
    public:
        // required function
        void Definitions(PTL::PropertySection& section)
        {
            section["name"].MapTo(&strval)
            = new PTL::PTLString("default value", "The name of the object");
            section["value"].MapTo(&intval)
            = new PTL::PTLInteger(0, "The value of the object");
        }
        void Print(void)
        {
            std::cout     <<
            "name: "      <<
            strval        <<
            "\nvalue: "   <<
            intval        <<
            "\nsection: " <<
            objName       << std::endl;
        }
};

int main(void)
{
    std::string filename = "iter.ptl";
    PTL::PropertyTree input;
    input.Read(filename);
    std::vector<MyClass> objs;

    // Note that PTL::PropertyTree and
    // PTL::PropertySection support range loops
    for (const auto& s : input["Objects"])
    {
        MyClass obj(s);
        obj.Parse();
        objs.push_back(obj);
    }
    for (auto& o:objs) o.Print();
    return 0;
}
```

`input.ptl`

```
Objects
{
    Object1
    {
        name = thisisobject1
        value = 1
    }
    Object2
    {
        name = thisisobject2
        value = 2
    }
}
```

### Runtime Evaluation
PTL offers preprocessor contexts that allow users to do some simple scripting. The context is resolved when a `.ptl`
file is read. Supported operations include reading from files, simple calculations, logicals, array indexing, and user-defined functions.
The following example demonstrates some simple preprocessor evaluation:

`input.ptl`

```
#define thetaDegrees 25
#define radius 1.45
#define xcoord @prod($(radius), @cosd($(thetaDegrees)))
#define ycoord @prod($(radius), @sind($(thetaDegrees)))

#define outputDirectory output
#define outputFileName data.dat

SamplePoint
{
    filename = @cat($(outputDirectory),/,$(outputFileName)) //evaluates to output/data.dat
    coordinates = [$(xcoord), $(ycoord)] // evaluates to [1.314147,0.612796]
    x = $(xcoord); y=$(ycoord) // semicolon can be used for multiple statements on a line
    rad = @sqrt(@sum(@prod($(xcoord),$(xcoord)), @prod($(ycoord),$(ycoord)))) // 1.450000
}
```

Note that the `//` symbol denotes a comment. One can also define user-defined functions within an application as follows:

```c++
std::string udf(std::vector<std::string>& args)
{
	std::string output = "UDFVALUE:";
	for (const auto a:args) output += a;
	return output;
}

int main(void)
{
	std::string filename = "udf.ptl";
	PTL::PropertyTree input;
	PTL::AddUserDefinedFunction("udf", udf);
    input.Read(filename);
}
```

The above will allow you to use the `@udf(...)` function in the input file. Here are the preprocessor functions
that PTL currently supports (WIP):

* `env(variable)`: retrieves the environment variable `variable`. If it is
not defined, an empty string is returned.

* `cat(expr1, expr2, ...)`: concatenates the values `expr1`, `expr2` and so 
forth.

* `choose(n, option0, option1, ...)`: selects the n-th element of the list
of arguments that follow, i.e. n=0 gives `option0`.

* `sum(num1, num2, ...)`: returns the sum of all arguments, which must be
parseable as numerical types.

* `prod(num1, num2, ...)`: returns the product of all arguments, which must be
parseable as numerical types.

* `sub(a, b)`: returns `a-b`. `a` and `b` must be parseable as numerical types.

* `div(a, b)`: returns `a/b`. `a` and `b` must be parseable as numerical types.

* `fread(file, n)`: returns the nth line of the file `file`.

* `round(num)`: rounds the numeric `num` to the nearest integer.

* `sin(num)`: returns the sine of the numeric `num`, assumed in radians.

* `cos(num)`: returns the cosine of the numeric `num`, assumed in radians.

* `sind(num)`: returns the sine of the numeric `num`, assumed in degrees.

* `cosd(num)`: returns the cosine of the numeric `num`, assumed in degrees.

* `max(num1, num2, ...)`: returns the maximum value of a list of numerics.

* `min(num1, num2, ...)`: returns the minimum value of a list of numerics.

* `atan2(y, x)`: returns the quadrant-aware arctangent of `y/x`, in radians.

* `atan2d(y, x)`: returns the quadrant-aware arctangent of `y/x`, in degrees.

* `sqrt(num)`: returns the square root of the numeric `num`.

* `lt(num1, num2)`: returns the logical-string `1` if `num1<num2`.

* `gt(num1, num2)`: returns the logical-string `1` if `num1>num2`.

### Syntax Highlighting
Syntax highlighting is currently a work in progress, and only supported through the Atom editor. In theory, you can simply type 

```
apm install language-ptl
```

in a bash terminal and it should install. Below is a screenshot, check it out!

![PTL syntax highlighting](https://i.imgur.com/OQnrEWA.png)

The intention is to expand the syntax highlighting capabilities to other editors as well.
There is a `tree-sitter` grammar package for PTL [here](https://github.com/wvannoordt/tree-sitter-ptl):
    
### Other Types
The reader is directed to [examples](https://github.com/wvannoordt/PTL/tree/master/examples) and [testing](https://github.com/wvannoordt/PTL/tree/master/testing)
for more examples of usage.

### Defaults
This example shows how to create a default-filled file called `def.ptl`:

`main.cc`

```c++
//Compiles to a.out
#include <iostream>
#include "PTL.h"
int main(int argc, char** argv)
{
    //This is the simplest example of a PTL input.

    PTL::PropertyTree input;

    //Define an interactive object
    PTL::Interactive i(argc, argv, &input);

    //define some input data
    std::string str;
    int aa, ab, ba, bb;
    double* ar;
    int n;

    input["SectionA"]["ValueA"].MapTo(&aa) = new PTL::PTLInteger(1, "aa");
    input["SectionA"]["ValueB"].MapTo(&ab) = new PTL::PTLInteger(1, "ab");
    input["SectionA"]["StringValue"].MapTo(&str) = new PTL::PTLString("defaultVal", "str");
    input["SectionB"]["ValueA"].MapTo(&ba) = new PTL::PTLInteger(1, "ba");
    input["SectionB"]["ValueB"].MapTo(&bb) = new PTL::PTLInteger(1, "bb");
    input["SectionB"]["arr"].MapTo(&ar, &n) = new PTL::PTLDynamicDoubleArray("array");

    //Outputs file "def.ptl" and kills program if program is run as "./a.out PTL:defaultVals"
    i.Run();

    return 0;
}
```

`def.ptl`:
```
// This file was generated with default values
SectionA
{
    StringValue = defaultVal
    ValueA = 1
    ValueB = 1
}
SectionB
{
    ValueA = 1
    ValueB = 1
    arr = []
}
```

### Query Mode

This basic example shows simple usage of query mode. Again, this is not the recommended usage of PTL.

`main.cc`

```c++
// This is the only header needed for C++ programs.
#include "PTL.h"

int main(void)
{
    // Create tree object
    PTL::PropertyTree input;

    // Read the input file
    input.Read("input.ptl");

    // Query the content of the tree
    int intValue  = input.Query("intValue");
    int intValue2 = input.Query("SomeSection.intValue2");

    // print the value
    std::cout << intValue << std::endl;
    std::cout << intValue2 << std::endl;
    return 0;
}
```

`input.ptl`

```
intValue = 13
SomeSection
{
    intValue2 = 14
}
```

### Fortran Interface

PTL now supports a basic interface for Fortran 90. An example can be found in `PTL/fortran/testing/main.F90`. Currently,
the fortran interface only supports the PTL format through query mode.


## License
[GNU GENERAL PUBLIC LICENSE](https://choosealicense.com/licenses/gpl-3.0/)
