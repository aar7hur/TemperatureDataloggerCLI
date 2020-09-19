from dataclasses import dataclass
from TE331 import 

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
    _command: str
    _readingInterval: str = None

    def __post_init__(self):
        self._serialConnection = Serial(self._serialPort)



