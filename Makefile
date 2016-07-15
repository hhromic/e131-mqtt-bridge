# E1.31 to MQTT Bridge Server
# Hugo Hromic - http://github.com/hhromic
#
# Copyright 2016 Hugo Hromic
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET = e131-mqtt-bridge
CPPFLAGS = -MMD -MP
CFLAGS = -Wall -O3
LDLIBS = -lmosquitto

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:%.c=%.d)

.PHONY: clean all

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(LDLIBS) -o $@
	strip -s $(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
