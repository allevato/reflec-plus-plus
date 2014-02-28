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

#include <cassert>
#include <iostream>
#include <string>
#include <reflect/reflect.hpp>
#include "test.hpp"
#include "test_macros.hpp"

// Tracks the number of tests executed and passed.
static int tests_passed = 0;
static int total_tests = 0;

// Instances of reflection objects used in testing.
reflect::function<int ()>
    function_no_args("function_no_args");

reflect::function<int (int, double, std::string)>
    function_with_args("function_with_args");

reflect::function<void (char)>
    function_in_namespace("test::function_in_namespace");

reflect::function<int (double, int, std::string)>
    function_with_wrong_args("function_with_args");

reflect::function<int ()>
    function_with_wrong_name("function_no_args_wrong_name");

/**
 * Runs the tests.
 */
int main(int argc, char** argv) {
  ASSERT_CALLED(function_no_args());
  ASSERT_CALLED(function_with_args(5, 10.0, "foo"));
  ASSERT_CALLED(function_in_namespace('x'));
  ASSERT_NOT_FOUND(function_with_wrong_args(10.0, 5, "foo"));
  ASSERT_NOT_FOUND(function_with_wrong_name());
  
  PRINT_SUMMARY();
  
  return 0;
}
