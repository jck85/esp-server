# Makefile to handle Arduino commands
#

compile:
	cd esp_server && ./build_html.sh
	arduino-cli compile --fqbn esp32:esp32:esp32s3 esp_server

upload: 
	arduino-cli upload --fqbn esp32:esp32:esp32s3 esp_server -p /dev/ttyACM0

monitor:
	arduino-cli monitor -c 115200 -p /dev/ttyACM0

all: compile upload monitor
