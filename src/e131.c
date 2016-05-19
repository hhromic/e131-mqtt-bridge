/**
 * E1.31 (sACN) to MQTT Bridge Server
 * Hugo Hromic - http://github.com/hhromic
 *
 * The contents of this file are based on:
 * https://github.com/forkineye/E131/blob/master/E131.h
 * https://github.com/forkineye/E131/blob/master/E131.cpp
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

#include "e131.h"

const uint8_t E131_ACN_ID[] = {0x41, 0x53, 0x43, 0x2d, 0x45, 0x31, 0x2e, 0x31, 0x37, 0x00, 0x00, 0x00};
const uint32_t E131_ROOT_VECTOR = 0x00000004;
const uint32_t E131_FRAME_VECTOR = 0x00000002;
const uint8_t E131_DMP_VECTOR = 0x02;

#include <string.h>
#include <arpa/inet.h>
#include "e131.h"

// validate an E1.31 packet
e131_error_t e131_validate_packet(e131_packet_t *packet) {
  if (memcmp(packet->acn_id, E131_ACN_ID, sizeof(packet->acn_id)) != 0)
    return E131_ERR_ACN_ID;
  if (htonl(packet->root_vector) != E131_ROOT_VECTOR)
    return E131_ERR_VECTOR_ROOT;
  if (htonl(packet->frame_vector) != E131_FRAME_VECTOR)
    return E131_ERR_VECTOR_FRAME;
  if (packet->dmp_vector != E131_DMP_VECTOR)
    return E131_ERR_VECTOR_DMP;
  return E131_ERR_NONE;
}
