/**
 *  @file kwargs.h
 *  @author Josh Bialkowski <josh.bialkowski@gmail.com>
 *  @brief tooling for python-like kwargs in c++
 */

/// classes and templates used by kwargs library
namespace kw {

/// parameter storage for type T within a parameter pack
template <int tag, typename T>
struct Arg {
  T v;
  Arg(T v) : v(v) {}
};

/// signifies that the parameter should be passed by reference
template <typename T>
struct RefWrap {
  T& v;
  RefWrap(T& v) : v(v) {}
};

/// signifies the parameter should be passed by const reference
template <typename T>
struct ConstRefWrap {
  const T& v;
  ConstRefWrap(const T& v) : v(v) {}
};

/// forces an argument to be passed by reference
template <typename T>
RefWrap<T> Ref(T& v){ return RefWrap<T>(v); }

/// forces an argument to be passed by const reference
template <typename T>
const ConstRefWrap<T> ConstRef(const T& v) { return ConstRefWrap<T>(v); }

/// the type of a global symbol used as a key for key-values pairs in the 
/// kwargs parameter pack
template <int tag>
struct Key {
	template <typename T>
	Arg<tag,T> operator=(T v) { return v; }

  template <typename T>
	Arg<tag,T&> operator=(RefWrap<T> vw) { return vw.v; }

  template <typename T>
	Arg<tag,const T&> operator=(ConstRefWrap<T> vw) { return vw.v; }
};

/// storage for kwargs parameter pack
template <typename... Args>
struct ParamPack{};

/// template meta-function contains a static boolean variable 'result' which
/// is true if tag is in the list of kwargs and false if not
template <int tag, typename... Args>
struct ContainsTag {};

/// template meta function provides a member typedef Result which evaluates
/// to T if Arg<tag,T> is in Args... or Default if it is not
template <int tag, typename Default, typename... Args>
struct TypeOfTagDefault {};

/// provides static member Get() with implementations depending on whether 
/// or not tag is in Args...
template <int tag, bool exists, typename Default, typename... Args>
struct GetImpl{};

/// given a parameter pack, retrieves and return sthe parameter tagged with tag,
/// or else returns a default value if tag is not in Args...
template <int tag, typename Default, typename... Args>
typename TypeOfTagDefault<tag,Default,Args...>::Result 
    Get(ParamPack<Args...>& pack, Default d) {
  return 
    GetImpl<tag,ContainsTag<tag,Args...>::result,Default,Args...>::Get(pack,d);
}

/// given a parameter pack, retrieves and return sthe parameter tagged with tag,
/// or else returns a default value if tag is not in Args...
template <int tag, typename Default, typename... Args>
typename TypeOfTagDefault<tag,Default,Args...>::Result 
    Get(ParamPack<Args...>& pack, Key<tag> key, Default d) {
  return 
    GetImpl<tag,ContainsTag<tag,Args...>::result,Default,Args...>::Get(pack,d);
}

}  //< namespace kw


// ----------------------------------------------------------------------------
//   Danger!!! Horrible implementation details below!!!
//   Continue at your own risk!
// ----------------------------------------------------------------------------

#ifndef DOXYGEN_IGNORE

namespace kw {

/// specialization for recursion, provides storage for the current type in the
/// type list, then recursively derives from the remaining types in the type
/// list
template <typename Head, typename... Tail>
struct ParamPack<Head,Tail...>
  : Head,
    ParamPack<Tail...> {
  ParamPack( Head head, Tail... tail )
    : Head(head),
      ParamPack<Tail...>(tail...) {}
};

/// specialization for base case, provides storage for the last type in the
/// type list
template <typename Tail>
struct ParamPack<Tail> : Tail {
  ParamPack( Tail tail ) : Tail(tail) {}
};

/// specialization for recursion
template <int tag, typename First, typename... Rest>
struct ContainsTag<tag,First,Rest...> {
  static const bool result = ContainsTag<tag, Rest...>::result;
};

/// specialization for when the Arg<tag,T> type is found
template <int tag, typename T, typename... Rest>
struct ContainsTag<tag, Arg<tag,T>, Rest...> {
  static const bool result = true;
};

/// specialization for when tag is not in the type list
template <int tag>
struct ContainsTag<tag> {
  static const bool result = false;
};

/// specialization for recursion
template <int tag, typename Default, typename Head, typename... Tail>
struct TypeOfTagDefault<tag, Default, Head, Tail...> {
  typedef typename TypeOfTagDefault<tag, Default, Tail...>::Result Result;
};

/// specialization for when Arg<tag,T> is found
template <int tag, typename Default, typename T, typename... Tail>
struct TypeOfTagDefault<tag, Default, Arg<tag, T>, Tail...> {
  typedef T Result;
};

/// specialization for when Arg<tag,T> is not in the type list
template <int tag, typename Default>
struct TypeOfTagDefault<tag,Default> {
  typedef Default Result;
};

/// specialization for when tag is not in Args..., returns the default value
template <int tag, typename Default, typename... Args>
struct GetImpl<tag,false,Default,Args...>{
  static Default Get(ParamPack<Args...>& pack, Default d) {
    return d;
  }
};

/// specialization for when tag is in Args... returns the value corresponding
/// to tag
template <int tag, typename Default, typename... Args>
struct GetImpl<tag,true,Default,Args...>{
  static typename TypeOfTagDefault<tag,Default,Args...>::Result Get(
      ParamPack<Args...>& pack, Default d) {
    typedef typename TypeOfTagDefault<tag,Default,Args...>::Result StorageType;
    return static_cast<Arg<tag,StorageType>&>(pack).v;
  }
};

}  //< namespace kw

#endif  // DOXYGEN_IGNORE
