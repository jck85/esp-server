# Makefile to handle Arduino commands
#

compile:
	./esp_server/scripts/build_html.sh
	arduino-cli compile --fqbn esp32:esp32:esp32s3 esp_server

upload: 
	arduino-cli upload --fqbn esp32:esp32:esp32s3 esp_server -p /dev/ttyACM1

monitor:
	arduino-cli monitor -c 115200 -p /dev/ttyACM1

all: compile upload monitor
