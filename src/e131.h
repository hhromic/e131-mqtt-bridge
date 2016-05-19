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

#ifndef _E131_H
#define _E131_H

#include <stdint.h>

#define E131_DEFAULT_PORT 5568

typedef union {
  struct {
    /* Root Layer */
    uint16_t preamble_size;
    uint16_t postamble_size;
    uint8_t  acn_id[12];
    uint16_t root_flength;
    uint32_t root_vector;
    uint8_t  cid[16];

    /* Frame Layer */
    uint16_t frame_flength;
    uint32_t frame_vector;
    uint8_t  source_name[64];
    uint8_t  priority;
    uint16_t reserved;
    uint8_t  sequence_number;
    uint8_t  options;
    uint16_t universe;

    /* DMP Layer */
    uint16_t dmp_flength;
    uint8_t  dmp_vector;
    uint8_t  type;
    uint16_t first_address;
    uint16_t address_increment;
    uint16_t property_value_count;
    uint8_t  property_values[513];
  } __attribute__((packed));

  uint8_t raw[638];
} e131_packet_t;

/* Error Types */
typedef enum {
  E131_ERR_NONE,
  E131_ERR_ACN_ID,
  E131_ERR_VECTOR_ROOT,
  E131_ERR_VECTOR_FRAME,
  E131_ERR_VECTOR_DMP
} e131_error_t;

extern const uint8_t E131_ACN_ID[];
extern const uint32_t E131_ROOT_VECTOR;
extern const uint32_t E131_FRAME_VECTOR;
extern const uint8_t E131_DMP_VECTOR;
extern e131_error_t e131_validate_packet(e131_packet_t *packet);

#endif
