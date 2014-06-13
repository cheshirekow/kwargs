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

class Test {
 public:
  Test(const Test& other) { Construct(); }
  Test() { Construct(); }

  ~Test() {
    std::cout << "Destroyed object " << obj_id_ << "\n";
  }

  int GetId() const { return obj_id_; }

 private:
  void Construct() {
    obj_id_ = n_objs_++;
    std::cout << "Created object " << obj_id_ << "\n";
  }
  int obj_id_; 
  static int n_objs_;
};

int Test::n_objs_ = 0;

std::ostream& operator<<( std::ostream& out, const Test& test ){
  out << test.GetId();
  return out;
}

std::ostream& operator<<( std::ostream& out, const Test* test ){
  out << "*" << test->GetId();
  return out;
}

template <typename... Args>
void foo(int a, int b, Args... kwargs) {
  kw::ParamPack<Args...> params(kwargs...);

  std::cout << "foo:\n----\n"
            << "\na: " << a
            << "\nb: " << b
            << "\ne: " << kw::Get<c_tag>(params,"null")
            << "\nf: " << kw::Get<d_tag>(params,"null")
            << "\n\n";
}

/// usage of foo in various different ways
int main( int argc, char** argv )
{
  foo(1, 2, c_key=Test());
  foo(1, 2, d_key=kw::ConstRef(Test()));
  Test test_obj;  
  foo(1, 2, d_key=kw::ConstRef(test_obj));
  foo(1, 2, d_key=kw::Ref(test_obj));
  foo(1, 2, d_key=&test_obj);
}

