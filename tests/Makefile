# Copyright (C) 2014 Tony Allevato
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include ../Make.config

SRCS	 = runner.cpp
SRCS	+= test.cpp

OBJS	 = $(patsubst %.cpp,%.o,$(SRCS))

RUNTESTS = run-tests

$(RUNTESTS): BUILD_OBJS = $(addprefix $(BUILD)/,$(OBJS))
$(RUNTESTS): BUILD_ARTIFACT = $(addprefix $(BUILD)/,$(RUNTESTS))

$(RUNTESTS): $(OBJS)
	$(CC) -o $(BUILD_ARTIFACT) $(BUILD_OBJS)
