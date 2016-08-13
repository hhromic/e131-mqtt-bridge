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
#include <e131.h>
#include <err.h>
#include "prototypes.h"

int main(int argc, char **argv) {
  int opt;
  uint16_t universe = 0x0001;
  char *mqtt_host = "localhost";
  uint16_t mqtt_port = 1883;
  char *mqtt_username = NULL;
  char *mqtt_password = NULL;
  char *mqtt_topic = NULL;
  struct mosquitto *mqtt_client = NULL;
  int e131_fd;
  e131_packet_t e131_packet;
  e131_error_t e131_error;
  uint8_t last_seq_number = 0x00;

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
  if ((mqtt_client = mosquitto_new(NULL, true, NULL)) == NULL)
    err(EXIT_FAILURE, "mosquitto_new");
  if (mqtt_username != NULL && mqtt_password != NULL)
    if (mosquitto_username_pw_set(mqtt_client, mqtt_username, mqtt_password))
      err(EXIT_FAILURE, "mosquitto_username_pw_set");
  if (mosquitto_connect(mqtt_client, mqtt_host, mqtt_port, 60))
    err(EXIT_FAILURE, "mosquitto_connect");
  fprintf(stderr, "connected to MQTT broker at '%s'\n", mqtt_host);

  // open E1.31 socket and join multicast group for the universe
  if ((e131_fd = e131_socket()) < 0)
    err(EXIT_FAILURE, "e131_socket");
  if (e131_bind(e131_fd, E131_DEFAULT_PORT) < 0)
    err(EXIT_FAILURE, "e131_bind");
  if (e131_multicast_join(e131_fd, universe) < 0)
    err(EXIT_FAILURE, "e131_multicast_join");
  fprintf(stderr, "E1.31 multicast server listening on port %d\n", E131_DEFAULT_PORT);

  // bridge E1.31 data to the MQTT broker
  fprintf(stderr, "bridging E1.31 (sACN) to MQTT, use CTRL+C to stop\n");
  for (;;) {
    if (e131_recv(e131_fd, &e131_packet) < 0)
      err(EXIT_FAILURE, "e131_recv");
    if ((e131_error = e131_pkt_validate(&e131_packet)) != E131_ERR_NONE) {
      fprintf(stderr, "e131_pkt_validate: %s\n", e131_strerror(e131_error));
      continue;
    }
    if (e131_pkt_discard(&e131_packet, last_seq_number)) {
      fprintf(stderr, "warning: out of order E1.31 packet received\n");
      last_seq_number = e131_packet.frame.seq_number;
      continue;
    }
    if (mosquitto_publish(mqtt_client, NULL, mqtt_topic, ntohs(e131_packet.dmp.prop_val_cnt) - 1, e131_packet.dmp.prop_val + 1, 0, false))
      err(EXIT_FAILURE, "mosquitto_publish");
    last_seq_number = e131_packet.frame.seq_number;
  }

  // finished
  mosquitto_destroy(mqtt_client);
  mosquitto_lib_cleanup();
  close(e131_fd);
  exit(EXIT_SUCCESS);
}
