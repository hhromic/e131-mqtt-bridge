/**
 * E1.31 (sACN) to MQTT Bridge Server
 * Hugo Hromic - http://github.com/hhromic
 *
 * Copyright 2016 Hugo Hromic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

// display program options and usage
void show_usage(char *prog_name) {
  fprintf(stderr, "Usage: %s [-H] [-U universe] [-h host] [-p port] [-u username] [-P password] -t topic\n", prog_name);
  fprintf(stderr, "Forward E1.31 (sACN) network data to an MQTT broker.\n");
  fprintf(stderr, "\nOptions:\n");
  fprintf(stderr, "  -H            show this help screen\n");
  fprintf(stderr, "  -U universe   DMX universe number for Multicast (default: 1)\n");
  fprintf(stderr, "  -h host       MQTT broker host to use (default: localhost)\n");
  fprintf(stderr, "  -p port       MQTT port for broker connection (default: 1883)\n");
  fprintf(stderr, "  -u username   MQTT username for broker connection (default: none)\n");
  fprintf(stderr, "  -P password   MQTT password for broker connection (default: none)\n");
  fprintf(stderr, "  -t topic      MQTT topic for publishing E1.31 (sACN) data\n");
  fprintf(stderr, "\n");
}
