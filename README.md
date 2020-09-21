# TemperatureDataloggerCLI

## Overview
Module implemented to extract data from an external eeprom memory, coupled to an STM microcontroller where temperature data of up to 90 days are stored.
**In addition to the serial interface with the microcontroller, the module is capable of parsing the data, 
these are saved in an external file in a standard directory at the root called "Temperature" and, 
after reading and parsing the data, a graph is generated with the temperature acquisition curve obtained**

## Usage
The module contains a command line interface that can be used through commands:

```python
python cli.py -h


cli.py [-h] [-verbose] [-output_file] [-command COMMAND] [-interval INTERVAL] serial_tx serial_rx

Command line interface used to communicate with temperature sensor

positional arguments:
  serial_tx             Serial Port to send data
  serial_rx             Serial Port to receive data

optional arguments:
  -h, --help            show this help message and exit
  -verbose, -v          Print the bytes exchanged
  -output_file, -o      Saves the received results in an output file
  -command COMMAND, -c COMMAND
                        Use READ to read eeprom memory.Use ERAS to erase entire externam eeprom
  -interval INTERVAL, -i INTERVAL
                        Additional command to read from flash.Use 1 to read the entire flash.Use 2 to read the last 10 days of reading.Use 3 to read last twenty four hours.Use 4 to read all exterrnal flash

