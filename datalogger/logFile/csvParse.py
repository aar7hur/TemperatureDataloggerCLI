import pandas as pd
import datetime
import csv
import os


class SaveDataToFile:
    """
    Class used to save data in csv format
    """
    def __init__(self):
        self._createLogFile()
        self._headerFile()

    def saveToLogFile(self, packages):
        """
        Function that saves all bytes exchanged in a file
        :param packages: Packages that will be saved in the file
        :return: None
        """
        with open(self._logPathCommand, '+a') as lp:
            lp.write(packages)
            lp.write("\n")
            lp.close()

    def _createLogFile(self):
        """
        Function that creates a path to save received commands results
        :return: path to log file
        """
        file_name = f'temperature_{datetime.datetime.now().strftime("%y%m%d_%H%M%S")}.csv'
        if not os.path.exists('Temperature'):
            os.mkdir(os.path.join('.', 'Temperature'))
            self._logPathCommand = os.path.join('.', 'Temperature', file_name)
        else:
            self._logPathCommand = os.path.join('.', 'Temperature', file_name)
        
    def _headerFile(self):
        """
        Create header in file
        """
        fieldnames = ["Temperatura"]
        with open(self._logPathCommand, 'w') as dataFile:
            csv_writer = csv.DictWriter(dataFile, fieldnames=fieldnames)
            csv_writer.writeheader()
    
    def readFile(self):
        """
        Read file and saves data into a list object
        """
        dataframe = pd.read_csv(self._logPathCommand)
    
        # Gets only temperature above 50ºC
        newDataframe = dataframe[dataframe.Temperatura <= 50]
        newDataframe = newDataframe.reset_index(drop=True)

        return newDataframe
      