import matplotlib.pyplot as plt
import numpy as np

class PlotGraphic:
    """
    Class used to plot graphics
    """
    @staticmethod
    def createGraphic(data, title: str, xlabel: str, ylabel: str, label: str):
        """
        Method that create and plots graphic.
        :param data: data used to be plotted. Must be pandas object
        :param titte: image tittle
        :param xlabel: variable magnitute. Time, Voltage...
        :param ylabel: variable magnitute. Time, Voltage, temperature..
        :param label: label to index in image
        """
        plt.style.use('bmh')
        plt.rcParams['animation.html'] = 'jshtml'
        plt.rcParams["figure.figsize"] = (10,8)
        
        xAxis = data.index
        fig, ax0 = plt.subplots(nrows = 1, constrained_layout = True)

        ax0.set_title(title)
        ax0.set_xlabel(xlabel)
        ax0.set_ylabel(ylabel)
        line,  = ax0.plot(xAxis, data,
                        color='r',
                        label=label)
        ax0.legend()
        plt.show()

