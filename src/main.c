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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mosquitto.h>
#include "prototypes.h"
#include "e131.h"

int main(int argc, char **argv) {
  int opt;
  uint16_t universe = 0x0001;
  char *mqtt_host = "localhost";
  uint16_t mqtt_port = 1883;
  char *mqtt_username = NULL;
  char *mqtt_password = NULL;
  char *mqtt_topic = NULL;
  struct mosquitto *mqtt_client = NULL;
  int socket_udp_fd;
  e131_packet_t e131_packet;
  uint8_t curr_sequence = 0x00;

  // program options
  while ((opt = getopt(argc, argv, "HU:h:p:u:P:t:")) != -1) {
    switch (opt) {
      case 'H':
        show_usage(argv[0]);
        exit(EXIT_SUCCESS);
      case 'U':
        sscanf(optarg, "%" SCNu16, &universe);
        if (universe < 0x0001 || universe > 0xf9ff) {
          fprintf(stderr, "error: universe must be between 1-63999\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'h':
        mqtt_host = optarg;
        break;
      case 'p':
        sscanf(optarg, "%" SCNu16, &mqtt_port);
        break;
      case 'u':
        mqtt_username = optarg;
        break;
      case 'P':
        mqtt_password = optarg;
        break;
      case 't':
        mqtt_topic = optarg;
        break;
      default:
        fprintf(stderr, "Try '%s -H' for more information.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (mqtt_topic == NULL) {
    fprintf(stderr, "error: you must specify an MQTT topic for publishing\n");
    show_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  // start connection with the MQTT broker
  mosquitto_lib_init();
  if ((mqtt_client = mosquitto_new(NULL, true, NULL)) == NULL) {
    perror("mosquitto_new");
    exit(EXIT_FAILURE);
  }
  if (mqtt_username != NULL && mqtt_password != NULL) {
    if (mosquitto_username_pw_set(mqtt_client, mqtt_username, mqtt_password)) {
      perror("mosquitto_username_pw_set");
      exit(EXIT_FAILURE);
    }
  }
  if (mosquitto_connect(mqtt_client, mqtt_host, mqtt_port, 60)) {
    perror("mosquitto_connect");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "connected to MQTT broker at '%s'\n", mqtt_host);

  // open udp socket, initialise and join multicast group
  if ((socket_udp_fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  init_socket_udp(socket_udp_fd, E131_DEFAULT_PORT);
  join_e131_multicast(socket_udp_fd, universe);
  fprintf(stderr, "multicast UDP server listening on port %d\n", E131_DEFAULT_PORT);

  // bridge E1.31 data to the MQTT broker
  fprintf(stderr, "bridging E1.31 (sACN) to MQTT, use CTRL+C to stop\n");
  for (;;) {
    if (recv(socket_udp_fd, e131_packet.raw, sizeof(e131_packet.raw), 0) < 0) {
      perror("recv");
      exit(EXIT_FAILURE);
    }
    if (e131_validate_packet(&e131_packet) != E131_ERR_NONE) {
      fprintf(stderr, "warning: invalid E1.31 packet received\n");
      continue;
    }
    if (e131_packet.sequence_number != curr_sequence++) {
      fprintf(stderr, "warning: out of order E1.31 packet received\n");
      curr_sequence = e131_packet.sequence_number + 1;
      continue;
    }
    if (mosquitto_publish(mqtt_client, NULL, mqtt_topic, htons(e131_packet.property_value_count) - 1, e131_packet.property_values + 1, 0, false)) {
      perror("mosquitto_publish");
      exit(EXIT_FAILURE);
    }
  }

  // finished
  mosquitto_destroy(mqtt_client);
  mosquitto_lib_cleanup();
  close(socket_udp_fd);
  exit(EXIT_SUCCESS);
}
