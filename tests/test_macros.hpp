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

#ifndef test_macros_hpp
#define test_macros_hpp

/**
 * Prints the results of a single test. Only called by other ASSERT_ macros.
 *
 * @param result if true, the test is considered passed; otherwise, it is
 *     considered failed
 * @param message the message to print if the test failed
 */
#define PRINT_TEST_RESULT(result, message) \
  total_tests++; \
  if (result) { \
    tests_passed++; \
    std::cout << '.'; \
  } else { \
    std::cout << "\nFailed: " << (message) << '\n'; \
  }

/**
 * Verifies that the function in the specified expression was called.
 *
 * @param expression the expression containing a reflect::function call; the
 *     target function MUST set the function_called global boolean to true
 */
#define ASSERT_CALLED(expression) \
  function_called = false; \
  (expression); \
  PRINT_TEST_RESULT(function_called, #expression " should have been called, but was not")

/**
 * Verifies that the function in the specified expression could not be found,
 * throwing a reflect::not_found exception.
 *
 * @param expression the expression containing a reflect::function call
 */
#define ASSERT_NOT_FOUND(expression) \
  { \
    bool threw = false; \
    try { \
      (expression); \
    } catch (reflect::not_found) { \
      threw = true; \
    } \
    PRINT_TEST_RESULT(threw, #expression " should not have been found, but it was"); \
  }

/**
 * Prints the result summary at the end of the test run.
 */
#define PRINT_SUMMARY() \
  std::cout << '\n' << tests_passed << " of " << total_tests << " tests passed.\n";

#endif
