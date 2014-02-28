reflec++
========

reflec++ is a small library that provides quasi-reflection to C++ programs
by using `dlsym` to access the symbol table of the executable at runtime,
and wrapping this in a convenient type-friendly API.


tl;dr example
-------------
```cpp
#include <iostream>
#include <reflect/reflect.hpp>

int foo(double bar) { ... }

int main() {
    reflect::function<int (double)> foo_function("foo");
    int result = foo_function(5.0);

    try {
        reflect::function<int (int)> not_foo("foo");
    } catch (reflect::not_found) {
        std::cout << "This will be printed." << std::endl;
    }
}
```


Introduction
------------
You should pretty much never use this in production code. It's ridiculous.
The main use case for this library is to provide a way for computer science
educators who teach C++ to write more robust tests of their students' code.

Imagine that you are an instructor and have given your students the following
assignment:

> Write a function named `perms_and_combs` that computes the number of
> possible permutations and combinations of `k` objects selected from a larger
> set of `n` objects. Your function should take integers `n` and `k` (in that
> order) followed by references to integers `p` and `c` that the function will
> use to send back the number of permutations and combinations (in that order).
> It should also have no return value.

Given that problem statement, you would expect a student to come up with the
following function:

```cpp
void perms_and_combs(int n, int k, int& p, int& c) {
    // ...
}
```

Now, you could write a test harness that declares `perms_and_combs` with this
signature and then makes calls to it. But what would happen if any of the
following were true:

* The student declared the function with the wrong name
* The student declared the function with the wrong parameters
* The student hasn't implemented the function at all

In any of these cases, the entire executable will fail to compile. If this
function is part of a larger assignment, the student won't get credit for any
of their other work because one mistake or omission caused the entire test
harness to be unusable.


The Solution
------------
The solution to this problem is to not try to bind to the student's functions
at compile-time in the test harness. Instead, you should dynamically load the
function and fail gracefully if it isn't found. Then, if a function is
unimplemented or has the wrong parameters, you can emit an error message or
test case failure and then continue execution.

This library provides a `reflect` namespace with types that make this dynamic
loading nearly as convenient as using the function itself. For example, the
`perms_and_combs` function above could be called like this:

```cpp
reflect::function<void (int, int, int&, int&)> perms_and_combs("perms_and_combs");

int n, k; // assume these initialized elsewhere
int p, c;
perms_and_combs(n, k, p, c);
```

In this scenario, `reflect::function` is a template that takes a function type
as its type parameter. When you instantiate it, you pass it the name of the
function to look up, and `dlsym` is called on the current process to try to
obtain a pointer to the function. This class automatically handles mangling
the function name using the provided type information before performing the
lookup.

Instantiating this `function` object will succeed whether or not the function
was found in the current executable. When you call the function (using the
overloaded function call operator), an exception will be thrown if no match
was found.


Known Issues
------------
* reflec++ has only been tested on recent builds of gcc and clang. It probably
  doesn't work with Visual C++.

* It isn't possible to reject a function based on its return type alone,
  because C++ compilers do not encode the return type in the mangled name.
  (This is because C++ does not allow overloading based on return type alone.)

* Likewise, functions with internal (static) linkage cannot be looked up with
  this library since they are not exported in the executable's symbol table.
