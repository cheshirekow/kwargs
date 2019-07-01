Python-style kwargs in C++
=========

When dealing with functions that have lots of optional parameters, or at
least for which resonable defaults are readily available, it's often a bit
of a frustration in C++. Generally defaults are specified during a function
as in:


    double BinarySearch(std::function<double(double)> fn,
                        int max_depth = 16, double epsilon = 1e-9,
                        double lower_bound = 0, double upper_bound = 100);


And then if we call `BinarySearch` with only one parameter then the call will
use the default values for the rest. But what if I want to specify custom
bound, but use the defaults for the other parameters? Admittedly, this is a
contrived example since bounds are less likely to be optional then the others,
and we could reorder them better going from most-likely-to-be-specified to
least, but it's easy to see how something more flexible would be desirable.

Consider then the following two code snippets. Which is more readable?

First snippet:

    double solution = BinarySearch(fn, 0, 100);


Second snippet:

    double solution = BinarySearch(fn, lower_bound = 0, upper_bound = 100);


I really like the way that optional arguments work in python with `kwargs`. I'd
love to have that same kind of functionality in C++. `kwargs.h` implements
one mechanism of achieving this.


# How does it work
`kwargs` takes advantage of [variadic templates][href_wiki] in C++ to build
up a single data structure which contains all of the optional parameters
(I'll call this a "parameter pack"). Each optional parameter of type `T` is
stored in a structure of type `Arg<tag,T>` where `tag` is a unique numeric
identifier associated with a particular optional argument key.

The parameter pack data structure derives from `Arg<tag,T>` for each `(tag,T)`
pair that shows up in the list of optional arguments.

Overloading of the equals (`=`) operator gives us an opportunity for building
the `(tag,T)` pairs within the parameter list of the function call.

See more [documentation and examples][href_doc] on github pages.

[href_wiki]: http://en.cppreference.com/w/cpp/language/parameter_pack (Wikipedia)
[href_doc]: http://cheshirekow.github.io/kwargs_doxygen/index.html (github)

# Examples

    #include <iostream>
    #include "kwargs.h"

    // these are tags which will uniquely identify the arguments in a parameter
    // pack
    enum Keys {
      c_tag,
      d_tag
    };

    // global symbols used as keys in list of kwargs
    kw::Key<c_tag> c_key;
    kw::Key<d_tag> d_key;

    // a function taking kwargs parameter pack
    template <typename... Args>
    void foo(int a, int b, Args... kwargs) {
      // first, we construct the parameter pack from the parameter pack
      kw::ParamPack<Args...> params(kwargs...);

      std::cout << "foo:\n--------"
                << "\na: " << a
                << "\nb: " << b
      // We can attempt to retrieve a key while providing a default fallback value.
      // If c_key is in kwargs then this will return the value associated with
      // that key, and will have the correct type. Note that the type of the default
      // parameter in this case is const char*.
                << "\nc: " << kw::Get(params,c_key,"null");
      // We can also do stuff conditionally based on whether or not arg exists in
      // the param pack. We still need to provide a default value, since we need to
      // know the return type of the Get function when the key is not in kwargs.
      // In this case, the default value wont ever be used at runtime.
      if( kw::ContainsTag<c_tag,Args...>::result ) {
        std::cout << "\nd: " << kw::Get(params,d_key,0);
      }

      std::cout << "\n\n";
    }

    int main( int argc, char** argv )
    {
      foo(1, 2);
      foo(1, 2, c_key=3);
      foo(1, 2, c_key=3, d_key=4);
      foo(1, 2, d_key=4);
    }

