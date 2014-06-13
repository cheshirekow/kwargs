// compile with g++ -std=c++11 -o kwargs kwargs.cpp

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
            << "\nb: " << b;
  // We can do stuff conditionally based on whether or not the parameter pack
  // contains the desired kwargs. We still need to provide a default value, but
  // in this case it wont ever be used at runtime.
  if( kw::ContainsTag<c_tag,Args...>::result ) {
    std::cout << "\nc: " << kw::Get<c_tag>(params,0);
  }
  
  // Alternatively, we can just use default values. If d_key is not among the
  // kwargs, then Get() return "null" (a const char*).
  std::cout << "\nd: " << kw::Get<d_tag>(params,"null");
  std::cout << "\n\n";
}

int main( int argc, char** argv )
{
  foo(1, 2);
  foo(1, 2, c_key=3);
  foo(1, 2, c_key=3, d_key=4);
  foo(1, 2, d_key=4);
}

