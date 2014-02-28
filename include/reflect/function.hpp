/*
 * Copyright (C) 2014 Tony Allevato
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef reflect_function_hpp
#define reflect_function_hpp

#include <dlfcn.h>
#include <exception>
#include <string>
#include <type_traits>
#include "mangler.hpp"

namespace reflect {

// --------------------------------------------------------------

/**
 * Thrown by runtime_function if the function was not found by dlsym().
 */
class not_found : public std::exception {
private:
  std::string function_name;

public:
  not_found(const std::string& name) throw() : function_name(name) { }
  
  virtual const char* what() const throw() {
    return function_name.c_str();
  }
};

// --------------------------------------------------------------

/**
 * The runtime_function class uses dlsym() to provide a rudimentary form
 * of reflection to C++ programs. The primary use case for this is when
 * testing student-written code. If a student is missing a function or has
 * declared it with the wrong arguments, traditional techniques would cause
 * the entire suite to fail to compile. By using runtime_function, you
 * defer the existence check for the function to runtime and can fail
 * gracefully if the function does not exist.
 *
 * Various specializations of this class provide support for ordinary
 * functions and (TODO) class member functions.
 *
 * Caveat: Functions declared with internal (static) linkage cannot be
 * found using this technique.
 */
template <typename>
class function_impl;


template <typename R, typename... A>
class function_impl<R(A...)> {
private:
  typedef R function_type(A...);

public:
  typedef typename std::add_pointer<function_type>::type
      ptr_to_function_type;

private:
  /** The unmangled name of the function. */
	std::string unmangled_name;
  
  /** The pointer to the function as resolved by dlsym(). */
	ptr_to_function_type fn_ptr;
  
public:
  /**
   * Constructs a runtime_function that does not point to a valid
   * function. If an attempt is made to resolve this object's function
   * pointer (by calling fn() or using the function call operator), an
   * exception will be thrown.
   */
	function_impl() {
		fn_ptr = 0;
	}

  /**
   * Constructs a runtime_function that points to a function in the
   * current executable with the specified name.
   *
   * @param name the name of the function to look up
   */
	function_impl(const std::string& name) : unmangled_name(name) {
		void* dl_handle = dlopen(0, 0);
		fn_ptr = (ptr_to_function_type) dlsym(
        dl_handle, mangler<function_type>(name).name().c_str());
		dlclose(dl_handle);
	}

  /**
   * Returns a pointer to the function to which this object refers, or
   * throws bad_runtime_function if it was not found.
   */
	ptr_to_function_type target() const {
		if (fn_ptr == 0) {
      throw not_found(unmangled_name);
    } else {
      return fn_ptr;
    }
	}

  /**
   * Calls the function with the specified argument list, or throws
   * reflect::not_found if the function was not found.
   */
  R operator()(A... arguments) const {
    return target()(arguments...);
  }
};

// --------------------------------------------------------------

template <typename>
class function;

template <typename R, typename... A>
class function<R(A...)> : public function_impl<R(A...)> {

public:
  function(const std::string& name) : function_impl<R(A...)>(name) {}
};

template <typename R, typename... A>
class function<R(A...) const> : public function_impl<R(A...)> {

public:
  function(const std::string& name) : function_impl<R(A...)>(name) {}
};

} // end namespace reflect

#endif
