from threading import Thread
from dataclasses import dataclass
from typing import Union
import logging
import serial
import time
import csv
import threading


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
        self._thread = Thread(target=self._readData)
        self._thread.start()
        self._isRun = True
        self._isReceiving = False
        self._receivedData = list()

    def _initSerial():
        """
        Opens a session on the serial port stipulated by the user in
        the construtor  with transmission speed also defined by the user.
        """
        try:
            self._serialConnection = serial.Serial(self._serialPort, self._baudRate)
            logger.info('Connected to : ' + str(self._serialPort) + 'with baud rate: ' + str(self._baudRate))
        except serial.SerialException:
            logger.error('Fail to connect with port: ' + str(serialPort))
            sys.exit(1)

     def _writeData(self, command: Union[bytes, str]):
        """
        Sends command to serial port
        :param command: 
        """
        if isinstance(command, str):
            command = command.encode()
        
        self.serialConnection.write(command.encode())

    def _readData(self):
        """
        Reads the data received by the thread that is listening to the serial
        """
        self.serialConnection.reset_input_buffer()
        while (self._isRun):
            try:
                self._receivedData = self.serialConnection.readline().decode().split('\r\n')
                self.isReceiving = True
                self.print(f'{self._receivedData[0])} + \n')
            except serial.SerialException:
                logger.error('Fail read data')

    def _closeConnection(self):
        """
        Stops the thread that reads the serial and closes the session
        """
        self._isRun = False
        self._thread.join()
        self._serialConnection.close()
        logger.info('Serial disconnected')
    
   
def main():
    fieldnames = ["Temperatura"]

    with open('datafile.csv', 'w') as dataFile:
        csv_writer = csv.DictWriter(dataFile, fieldnames=fieldnames)
        csv_writer.writeheader()

    comport = 'COM17'
    baudrate = 38400

    user_input = int(input("Digite 1 para ler os dados da memória EEPROM ou digite 2 para apagar os dados da memória!"))

    if(user_input ==  1):
        
        s = serialread(comport, baudrate)
        sent_value = str("read")
        s.write_command(sent_value)
        s.readserialThread()
    
    if(user_input == 2):

        user_confirmation = input("Deseja mesmo apagar? Não será possível resgatar os dados. Digite 2 se deseja confirmar")
        
        if(user_confirmation == 2):

            s = serialread(comport, baudrate)
            sent_value = str("eras").enconde('ascii')
            s.write_command(sent_value)
    else:
        print("Valor inválido!!!")





if __name__ == '__main__':
    main()