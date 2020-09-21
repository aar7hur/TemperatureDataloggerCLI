from enum import Enum
from typing import Optional

class Commands(Enum):
    """
    Class used to represent all commands accepted by the datalogger
    """
    FLASH_DATA = "READ"
    ERASE_ENTIRE_FLASH = "ERAS"


class ReadingInterval(Enum):
    """
    Classe used to represent all possible temperature reading ranges
    from the datalogger flash memory
    """
    ENTIRE_FLASH = 1
    TEN_DAYS = 2
    TWENTY_FOUR_HOURS = 3
    TWELVE_HOURS = 4


class DataloggerBuilder:
    """
    Class used to transform datalogger request into a list
    of bytes to be send to serial
    """
    @staticmethod
    def createHeader(command: Commands, readingInterval: ReadingInterval = None):
        """
        Create datalogger header to send data to serial
        :param command: command to be send. INSTANTANOUS_DATA or FLASH_DATA
        :para readingInterval: flash measurement reading range
        """
        command = command.value.encode('ascii')
        if readingInterval is not None:
            readingInterval = readingInterval.value.to_bytes(1, 'big')
            return b''.join([command, readingInterval])
        else:
            return command
