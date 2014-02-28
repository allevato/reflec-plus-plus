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

#include <string>
#include "test.hpp"

bool function_called;

int function_no_args() {
  function_called = true;
  return 0;
}

int function_with_args(int a, double b, std::string c) {
  function_called = true;
  return 0;
}

namespace test {

void function_in_namespace(char c) {
  function_called = true;
}

} // end namespace test
