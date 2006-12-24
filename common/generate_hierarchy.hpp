#ifndef GENERATE_HIERARCHY_HPP
#define GENERATE_HIERARCHY_HPP

/*

File: generate_hierarchy.hpp
Author: David Bergman
Date: 2006-07-25

Copyright David Bergman 2006.

Not distributed under any specific license yet.

This is a utility to create a hierarchy from a
vector of types, using Boost.MPL and influenced
by Andrei Alexandrescu's work and, more specifically,
his implementation "HierarchyGenerators.h" being part
of his eminent Loki library.

The reason to write another generator is to leverage
the power found in MPL and use MPL-specific type 
sequences.

*/

#include <boost/mpl/apply.hpp>
#include <boost/mpl/reverse_fold.hpp>

namespace boost {

  // The generator is a meta function in that it
  // takes some types and generates a new type, but
  // it is NOT a proper MPL meta function since it
  // does not comply with the syntactic requirements
  //(::type, ::apply...)
  
  
  // We deal with two scenarios:
  // 1. The types are concrete types, which are then
  //    added in a spine-like structure, with the
  //    generator construct as the spine.
  //    These base classes are then maximal elements,
  //    and do not have access to anything further up
  //    the spine.
  // 2. The types are parameterized, with a "hole"
  //    parameter where the chain of types are being
  //    built up. The instantiated types have access to
  //    stuff further up the chain. The base types are
  //    actually MPL meta function classes.
  
  namespace {

    // We define a meta function class creating a sub
    // class of two base classes, which can be either
    // regular classes, in which case we simply create
    // a common child, or one class and one
    // transformation meta function, in which case we
    // create a child class inheriting the application
    // of that meta function onto the base class
    
    struct inherit_two {
      template <typename Spine, typename Base>
      struct apply {
	struct type : Base, Spine {};
      };
    };

    struct inherit_app {
      template <typename Spine, typename Fun>
      struct apply {
	struct type : mpl::apply<Fun, Spine>::type {};
      };
    };
  
  } // anonymous namespace
  
  // The corresponding generators are called
  // "generate_spine" and "generate_chain"
  
  // Any ForwardSequence is welcome!
  
  
  //
  // generate_spine
  //
  
  template <typename ForwardSeq>
  struct generate_spine :
    mpl::reverse_fold<
      ForwardSeq, mpl::void_, inherit_two
    >::type
  {};
  
  //
  // generate_chain
  //
  
  template <typename MetFunSeq,
	    typename BaseT = mpl::void_>
  struct generate_chain :
    mpl::reverse_fold<MetFunSeq, BaseT,
		      inherit_app>::type {};

  //
  // Utilities to create a meta function class out
  // of a template.
  // In contrast to the 'quote' function in MPL,
  // this creates a meta function NO MATTER wether a
  // 'type' happens to be defined or not in the
  // template class.
  //
  // These utilities should NOT be here, but in MPL
  // - yes, they should be generated by a
  // preprocessor tool.
  // 

  template <template <typename> class Template>
  struct quote_raw1 {
    template <typename T>
    struct apply {
      typedef Template<T> type;
    };
  };

  template <
    template <typename, typename> class Template
  >
  struct quote_raw2 {
    template <typename T1, typename T2>
    struct apply {
      typedef Template<T1, T2> type;
    };
  };

  template <
    template <typename, typename, typename> 
    class Template
  >
  struct quote_raw3 {
    template <typename T1, typename T2, typename T3>
    struct apply {
      typedef Template<T1, T2, T3> type;
    };
  };

} // namespace boost

#endif
