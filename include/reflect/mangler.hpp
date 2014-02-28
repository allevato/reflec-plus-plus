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

#ifndef reflect_mangler_hpp
#define reflect_mangler_h

#include <string>
#include <sstream>

namespace reflect {

/**
 * The mangler templates provide an interface for retrieving the mangled
 * name of a C++ function by accessing its type_info. The various
 * specializations allow the compiler to select the proper mangling
 * technique based on the function type being requested (for example,
 * an ordinary function, or a pointer to member, etc.).
 */

// --------------------------------------------------------------

template <typename>
class mangler_impl;

template <typename R, typename... A>
class mangler_impl<R(A...)> {
private:
  /** See the name() member function for why this is necessary. */
  typedef void (void_function_type)(A...);
  
  std::string unmangled_name;
  std::string modifiers;

public:
  /** Constructs a mangler for the given unmangled function name. */
  mangler_impl(const std::string& u, const std::string& m) :
      unmangled_name(u), modifiers(m) {}
  
  /**
   * Returns the mangled name of the function to which this object refers.
   *
   * Since functions in C/C++ cannot be overloaded on return type alone,
   * the return type R is not encoded in the function's mangled name. It
   * is, however, encoded in typeid(function_type).name(), so we must
   * remove it when we compose the function's mangled name. This is made
   * easier if we force the return type to be void -- then the mangled
   * function signature is known to be of the form "Fv<mangled arguments>E"
   * and simple string manipulation can be used to convert it to
   * "_Z<function name length><function name><mangled arguments>".
   */
  std::string name() const {
    std::string mangled_type = typeid(void_function_type).name();
    std::ostringstream stream;
    stream << "_Z" << modifiers;

    size_t begin = 0;
    size_t end = unmangled_name.find("::");
    if (end == std::string::npos) {
      stream << unmangled_name.length() << unmangled_name;
    } else {
      stream << 'N';
      while (end != std::string::npos) {
        // begin == end for valid names only if the name of begins with
        // "::" to explicitly reference the global namespace. We put this
        // check here because there's no reason to disallow this.
        if (begin != end) {
          stream << (end - begin);
          stream << unmangled_name.substr(begin, end - begin);
        }
        begin = end + 2;
        end = unmangled_name.find("::", begin);
      }
      stream << (unmangled_name.length() - begin);
      stream << unmangled_name.substr(begin);
      stream << 'E';
    }

    stream << mangled_type.substr(2, mangled_type.length() - 3);
    return stream.str();
  }
};

template <typename R, typename C, typename... A>
class mangler_impl<R (C::*)(A...) const> {
private:
  /** See the name() member function for why this is necessary. */
  typedef void (C::*void_function_type)(A...) const;
  
  std::string unmangled_name;
  std::string modifiers;
  
public:
  /** Constructs a mangler for the given unmangled function name. */
  mangler_impl(const std::string& u, const std::string& m) :
      unmangled_name(u), modifiers(m) {}
  
  /**
   * Returns the mangled name of the function to which this object refers.
   *
   * Since functions in C/C++ cannot be overloaded on return type alone,
   * the return type R is not encoded in the function's mangled name. It
   * is, however, encoded in typeid(function_type).name(), so we must
   * remove it when we compose the function's mangled name. This is made
   * easier if we force the return type to be void -- then the mangled
   * function signature is known to be of the form "Fv<mangled arguments>E"
   * and simple string manipulation can be used to convert it to
   * "_Z<function name length><function name><mangled arguments>".
   */
  std::string name() const {
    std::string mangled_type = typeid(void_function_type).name();
    std::ostringstream stream;
    stream << "_Z" << modifiers;
    
    size_t begin = 0;
    size_t end = unmangled_name.find("::");
    if (end == std::string::npos) {
      stream << unmangled_name.length() << unmangled_name;
    } else {
      stream << 'N';
      while (end != std::string::npos) {
        // begin == end for valid names only if the name of begins with
        // "::" to explicitly reference the global namespace. We put this
        // check here because there's no reason to disallow this.
        if (begin != end) {
          stream << (end - begin);
          stream << unmangled_name.substr(begin, end - begin);
        }
        begin = end + 2;
        end = unmangled_name.find("::", begin);
      }
      stream << (unmangled_name.length() - begin);
      stream << unmangled_name.substr(begin);
      stream << 'E';
    }
    
    stream << mangled_type.substr(2, mangled_type.length() - 3);
    return stream.str();
  }
};

// --------------------------------------------------------------

template <typename>
class mangler;

/**
 * Mangles non-const non-member functions.
 */
template <typename R, typename... A>
class mangler<R(A...)> : public mangler_impl<R(A...)> {
public:
  mangler(const std::string& u) : mangler_impl<R(A...)>(u, "") {}
};

/**
 * Mangles const non-member functions.
 */
template <typename R, typename... A>
class mangler<R(A...) const> : public mangler_impl<R(A...)> {
public:
  mangler(const std::string& u) : mangler_impl<R(A...)>(u, "K") {}
};

} // end namespace reflect

#endif
