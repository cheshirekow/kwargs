// compile with g++ -std=c++11 -o kwargs kwargs.cpp

#include <iostream>
#include "kwargs.h"

// these are tags which will uniquely identify the arguments in a parameter
// pack
enum Keys {
  c_tag,
  d_tag
};

// global symbols used as keys in function parameters
kw::Key<c_tag> c_key;
kw::Key<d_tag> d_key;

namespace test {

enum Tags {
  e_tag,
  f_tag
};

kw::Key<e_tag> e_key;
kw::Key<f_tag> f_key;

}  // namespace test

template <typename... Args>
void foo(int a, int b, Args... kwargs) {
  kw::ParamPack<Args...> params(kwargs...);

  std::cout << "foo3:\n---------------\n"
            << "\na: " << a
            << "\nb: " << b
            << "\ne: " << kw::Get<c_tag>(params,"null")
            << "\nf: " << kw::Get<d_tag>(params,"null")
            << "\n\n";
}


/// usage of foo in various different ways
int main( int argc, char** argv )
{
  foo(1, 2);
  foo(1, 2, c_key = 3);
  foo(1, 2, test::e_key = 3);
  foo(1, 2, kw::Key<0>() = 3);
}

