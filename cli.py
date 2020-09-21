from dataclasses import dataclass
from datalogger.serialComm.serialComm import Serial
from datalogger.logFile.csvParse import SaveDataToFile
from datalogger.plotGraphic.graphic import PlotGraphic
from datalogger import DataloggerBuilder, Commands, ReadingInterval
import argparse
import logging


@dataclass
class CommandLineInterface:
    """
    Classe used to create an command line interface
    to be able to get/send data to the datalogger microcontroler
    using only the terminal
    """
    _serialPortTx: str
    _serialPortRx: str
    _command: str
    _readingInterval: int = None
    _verbose: bool =  False
    _outputFile: bool = True

    def __post_init__(self):
        logging.basicConfig(level = logging.INFO)
        self._serialTx = Serial(self._serialPortTx)
        self._serialRx = Serial(self._serialPortRx)
        self._logFile = SaveDataToFile()
        if self._outputFile:
            self._serialTx.print = self._logFile.saveToLogFile
            self._serialRx.print = self._logFile.saveToLogFile

    def run(self):
        """
        Sends data to serial and receive data 
        """
        payload = self._parseCliArgs()
        logging.info('Sending data')
        self._serialTx.writeData(payload)
        self._serialRx.readSerial()
        logging.info('Data received')
        if self._outputFile:
            data = self._logFile.readFile() 
            PlotGraphic.createGraphic(data, "Medição de temperatura do datalogger",
                                    "Número de medições",
                                    "Temperatura [ºC]",
                                    "Temperatura ao longo do tempo")

    def _parseCliArgs(self):
        """
        Transform user commands passed through the CLI into
        data to send to the datalogger microcontroler
        :return dataToSend: payload in bytes to be sent to serial
        """
        for command in Commands:
            if (self._command in [command.value]): 
                if self._readingInterval:
                    for interval in ReadingInterval:
                        if (self._readingInterval == interval):
                            dataToSend = DataloggerBuilder.createHeader(command, interval)
                            return dataToSend
                else:
                    dataToSend = DataloggerBuilder.createHeader(command)
                    return dataToSend
                    

"""
The purpose of this driver is to be able to communicate with a serial device,
send commands and read commands from it. A Command line interface has been developed 
and can be used according to the parameters below.
"""
parser = argparse.ArgumentParser(description='Command line interface used to communicate with temperature sensor')
parser.add_argument('serial_tx', type=str, help='Serial Port to send data')
parser.add_argument('serial_rx', type=str, help='Serial Port to receive data')
parser.add_argument('-verbose', '-v', help='Print the bytes exchanged', action='store_true', default=False)
parser.add_argument('-output_file', '-o', help='Saves the received results in an output file', action='store_true',
                        default=False)
parser.add_argument('-command', '-c', type=str, help='Use READ to read eeprom memory.' 
                                                    'Use ERAS to erase entire externam eeprom')
parser.add_argument('-interval', '-i', type=int, default=None, help='Additional command to read from flash.' 
                                                                    'Use 1 to read the entire flash.' 
                                                                    'Use 2 to read the last 10 days of reading.'
                                                                    'Use 3 to read last twenty four hours.'
                                                                    'Use 4 to read all exterrnal flash')

argsParser = parser.parse_args()
serialPortTx = argsParser.serial_tx
serialPortRx = argsParser.serial_rx
outputFile = argsParser.output_file
verbose =  argsParser.verbose
command = argsParser.command
interval = argsParser.interval

cli = CommandLineInterface(serialPortTx, serialPortRx, command, interval, verbose, outputFile)
cli.run()
