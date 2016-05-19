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

#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H

#include <stdint.h>
#include <termios.h>
#include "e131.h"

extern int e131_validate(e131_packet_t *packet);
extern void init_socket_udp(int fd, uint16_t port);
extern void join_e131_multicast(int fd, uint16_t universe);
extern void show_usage(char *prog_name);

#endif
