import argparse
from measureTemperature.cli import CommandLineInterface
"""
The purpose of this driver is to be able to communicate with a serial device,
send commands and read commands from it. A Command line interface has been developed 
and can be used according to the parameters below.
"""
parser = argparse.ArgumentParser(description='Command line interface used to communicate with temperature sensor')
parser.add_argument('serial_port', type=str, help='Serial Port to communicate')
parser.add_argument('-verbose', '-v', help='Print the bytes exchanged', action='store_true', default=False)
parser.add_argument('-output_file', '-o', help='Saves the received results in an output file', action='store_true',
                        default=False)
parser.add_argument('-command', '-c', type=int, help='Command that you want to send. Use 1 to read Instantaneous data'
                                                    'and 2 to read data from external flash')
parser.add_argument('-interval', '-i', type=int, default=None, help='Additional command to read from flash.' 
                                                                    '\nUse 1 to read the entire flash. 
                                                                    '\nUse 2 to read the last 10 days of reading.'
                                                                    '\nUse 3 to read last twenty four hours.
                                                                    '\nUse 4 to read all exterrnal flash')

argsParser = parser.parse_args()
serialPort = argsParser.serial_port
outputFile = argsParser.output_file
verbose =  argsParser.verbose
command = argsParser.comand
interval = argsParser.interval

cli = CommandLineInterface(serialPort, verbose, outputFile, command, interval)
cli.run()

