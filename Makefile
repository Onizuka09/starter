init_project: 
	pio init

generate_commands_json:
	pio run -t compiledb
monitor:
	pio device monitor -b 115200
build:
	pio run
flash:
	pio run --target upload --upload-port /dev/ttyUSB0
littlefs_flash:
	pio run --target uploadfs --upload-port /dev/ttyUSB0
