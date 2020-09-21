from threading import Thread
from dataclasses import dataclass
from typing import Union
import logging
import serial
import time
import threading
import sys


__author__ = "Arthur.Amaral"

@dataclass
class Serial:
    """
    Class used to communicate with the microcontroller serial.
    """
    _serialPort: str
    _baudRate: int = 38400
    
    def __post_init__(self):
        self.print = print
        self._isRun = True
        self._initSerial()
        self._thread = None
        self._isRun = True
        self._isReceiving = False
        self._receivedData = list()

    def _initSerial(self):
        """
        Opens a session on the serial port stipulated by the user in
        the construtor  with transmission speed also defined by the user.
        """
        try:
            self._serialConnection = serial.Serial(self._serialPort, self._baudRate, timeout=10)
            logging.info('Connected to: ' + str(self._serialPort) + ' with baud rate: ' + str(self._baudRate))
        except serial.SerialException:
            logging.error('Fail to connect with port: ' + str(self._serialPort))
            sys.exit(1)

    def writeData(self, payload: Union[bytes, str]):
        """
        Sends command to serial port
        :param command: 
        """
        if isinstance(payload, str):
            payload = payload.encode()
        
        logging.info('Comando enviado: ' + str(payload))
        self._serialConnection.write(payload)

    def readSerial(self):
        """
        Reads the data received the serial receiver
        """
        while (self._isRun):
            try:
                self._receivedData = self._serialConnection.readline().decode().split('\r\n')
                self._isReceiving = True
                if self._receivedData[0] in [None, '']:
                    self._isRun = False
                    break
                self.print(self._receivedData[0])
            except serial.SerialException:
                logging.error('Fail to read data')
            except KeyboardInterrupt:
                self.closeConnection()
    
    def closeConnection(self):
        """
        Stops the thread that reads the serial and closes the session
        """
        self._isRun = False
        self._serialConnection.close()
        logging.info('Serial disconnected')

if __name__ == '__main__':

    value = "READ"
    value = value.encode('ascii')
    serial = Serial('COM3', 38400)
    time.sleep(2)
    serial.writeData(value)
    time.sleep(1)
    
  