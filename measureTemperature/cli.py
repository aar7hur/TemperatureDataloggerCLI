from dataclasses import dataclass
from measureTemperature.serialComm import Serial
from measureTemperature.datalogger import Commands, ReadingInterval
from measureTemperature.csvParse import SaveDataToFile
from measureTemperature.datalogger import DataloggerBuilder
import logging


@dataclass
class CommandLineInterface:
    """
    Classe used to create an command line interface
    to be able to get/send data to the datalogger microcontroler
    using only the terminal
    """
    _serialPort: str
    _verbose: bool =  False
    _outputFile: bool = True
    _command: int
    _readingInterval: int = None

    def __post_init__(self):
        self._serialComm = Serial(self._serialPort)
        self._logFile = SaveDataToFile()
        if self._outputFile:
            self._serialComm.print = self._logFile.saveToLogFile
        
    def run(self):
        """
        Sends data to serial and receive data 
        """
        payload = self._parseCliArgs()
        logging.info('Sending data')
        self._serialComm.writeData(payload)
        self._serialComm.readData()
        logging.info('Data received')

    def _parseCliArgs(self):
        """
        Transform user commands passed through the CLI into
        data to send to the datalogger microcontroler
        :return dataToSend: payload in bytes to be sent to serial
        """
        for command in Commands:
            if (self._command == command.value): 
                if self._readingInterval:
                    for interval in ReadingInterval:
                        if (self._readingInterval == interval):
                            dataToSend = DataloggerBuilder.createHeader(command, interval)
                            return dataToSend
                else:
                    dataToSend = DataloggerBuilder.createHeader(command)
                    return dataToSend
                    






        
    
    

    


        


