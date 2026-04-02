# ESP Server

A template for creating ESP32 webservers.

Built using Arduino CLI

Setup

Install Arduino CLI
https://docs.arduino.cc/arduino-cli/installation/

Install Make

```
sudo apt install build-essential
```

or

```
sudo apt install make
```

Install ESP32 boards for Arduino

```
arduino-cli core install esp32:esp32
```

Libraries

ESP Async Web Server
https://github.com/ESP32Async/ESPAsyncWebServer

Async TCP
https://github.com/ESP32Async/AsyncTCP

To install a library as a zip file, create and edit an arduino-cli.yaml file.

Create yaml file

```
arduino-cli config init
```

Add the following to the yaml file.

```
library:
    enable_unsafe_install: true
```

This code is written for ESP32 S3 boards.
To compile for a different board, adjust the parameter in the compile and upload commands in the Makefile.

For example, for ESP32 S3:

```
	arduino-cli compile --fqbn esp32:esp32:esp32s3 esp_server

```

For example, for ESP32:

```
	arduino-cli compile --fqbn esp32:esp32:esp32 esp_server

```

In order to connect to a wifi network, be sure to create a secrets folder in the esp_server directory.
The secrets folder path is added to .gitignore so will not be included when committing.
Double check to make sure this folder is not commited before pushing to a remote repo.

Issues

- The IP address for the ESP32 must manually assigned to `ws_gateway` in the `<script>` section of `index.html`.
