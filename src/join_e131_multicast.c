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
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// join UDP socket to the E1.31 multicast group of the DMX universe (239.255.hi.lo)
void join_e131_multicast(int fd, uint16_t universe) {
  struct ip_mreqn mreq;
  mreq.imr_multiaddr.s_addr = htonl(0xefff0000 | universe);
  mreq.imr_address.s_addr = htonl(INADDR_ANY);
  mreq.imr_ifindex = 0;
  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt IP_ADD_MEMBERSHIP");
    exit(EXIT_FAILURE);
  }
}
