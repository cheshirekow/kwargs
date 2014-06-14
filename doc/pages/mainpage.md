Python-style kwargs in C++ {#mainpage}
=========

When dealing with functions that have lots of optional parameters, or at
least for which resonable defaults are readily available, it's often a bit
of a frustration in C++. Generally defaults are specified during a function
as in:

@code{cc}
double BinarySearch(std::function<double(double> fn,
                    int max_depth = 16, double epsilon = 1e-9,
                    double lower_bound = 0, double upper_bound = 100);
@endcode

And then if we call `BinarySearch` with only one parameter then the call will
use the default values for the rest. But what if I want to specify custom
bound, but use the defaults for the other parameters? Admittedly, this is a
contrived example since bounds are less likely to be optional then the others,
and we could reorder them better going from most-likely-to-be-specified to
least, but it's easy to see how something more flexible would be desirable.

Consider then the following two code snippets. Which is more readable?

First snippet:
@code{cc}
double solution = BinarySearch(fn, 0, 100);
@endcode

Second snippet:
@code{cc}
double solution = BinarySearch(fn, lower_bound = 0, upper_bound = 100);
@endcode

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

[href_wiki]: http://en.cppreference.com/w/cpp/language/parameter_pack (Wikipedia)

# Examples

## Example 1
### Source
@includelineno example_1.cc

### Output
@include example_1.txt

## Example 2
### Source
@includelineno example_2.cc

### Output
@include example_2.txt

## Example 3
### Source
@includelineno example_3.cc

### Output
@include example_3.txt

