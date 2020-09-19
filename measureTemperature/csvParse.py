from datetime import datetime
import os

class SaveDataToFile():
    """
    Class used to save data in csv format
    """
    def __init__(self):
        self._createLogFile()

    def saveToLogFile(self, packages):
        """
        Function that saves all bytes exchanged in a file
        :param packages: Packages that will be saved in the file
        :return: None
        """
        print(packages.__str__())
        with open(self._logPathCommand, '+a') as lp:
            lp.write(datetime.datetime.now().strftime("[%y/%m/%d %H:%M:%S]") + packages.__str__())
            lp.write("\n")
            lp.close()

    def _createLogFile(self):
        """
        Function that creates a path to save received commands results
        :return: path to log file
        """
        file_name = f'temperature_{datetime.datetime.now().strftime("%y%m%d_%H%M%S")}.log'
        if not os.path.exists('Temperature'):
            os.mkdir(os.path.join('.', 'Temperature'))
            self._logPathCommand = os.path.join('.', 'Temperature', file_name)
        else:
            self._logPathCommandlogPathCommand = os.path.join('.', 'Temperature', file_name)
      