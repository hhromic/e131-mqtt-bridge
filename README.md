# E1.31 (sACN) to MQTT Bridge Server

## Compiling

To compile the bridging server, simply use the `make` utility:

    $ make clean
    $ make

You must have the `libmosquitto` ([get it here][libmosquitto]) and the `libe131` ([get it here][libe131]) development libraries installed on your system. Once compiled, you can install the output binary file wherever you want.

[libmosquitto]: http://mosquitto.org/download/
[libe131]: https://github.com/hhromic/libe131/releases/latest

## Running

To run the program, you must supply at least an MQTT topic for publishing E1.31 (sACN) data:

    Usage: e131-mqtt-bridge [-H] [-U universe] [-h host] [-p port] [-u username] [-P password] -t topic
    Forward E1.31 (sACN) network data to an MQTT broker.

    Options:
      -H            show this help screen
      -U universe   DMX universe number for Multicast (default: 1)
      -h host       MQTT broker host to use (default: localhost)
      -p port       MQTT port for broker connection (default: 1883)
      -u username   MQTT username for broker connection (default: none)
      -P password   MQTT password for broker connection (default: none)
      -t topic      MQTT topic for publishing E1.31 (sACN) data

Example invocation for MQTT broker at `test.mosquitto.org` using topic `dmx/2/data` with DMX universe `2`:

    $ ./e131-mqtt-bridge -U 2 -h test.mosquitto.org -t dmx/2/data

## License

This software is under the **Apache License 2.0**.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
