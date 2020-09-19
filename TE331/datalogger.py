from enum import Enum
from typing import Optional


class Commands(Enum):
    """
    Class used to represent all commands accepted by the datalogger
    """
    INSTANTANEOUS_DATA = 1
    FLASH_DATA = 2


class ReadingInterval(Enum):
    """
    Classe used to represent all possible temperature reading ranges
    from the datalogger flash memory
    """
    TEN_DAYS = 1
    FIVE_DAYS = 2
    TWENTY_FOUR_HOURS = 3
    TWELVE_HOURS = 4



class DataloggerBuilder:
    """
    Class used to transform datalogger request into a list
    of bytes to be send to serial
    """
    @staticmethod
    def createHeader(self, command: Commands, readingInterval: Optional[ReadingInterval, None])
        """
        Create datalogger header to send data to serial
        :param command: command to be send. INSTANTANOUS_DATA or FLASH_DATA
        :para readingInterval: flash measurement reading range
        """
        command = command.value.to_bytes(1, 'big')
        if readingInterval is not None:
            readingInterval = readingInterval.value.to_bytes(1, 'big')
            return b''.join([command, readingInterval])
        else:
            return command
