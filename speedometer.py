import sys,signal,can
from PyQt5 import QtGui, QtCore, QtWidgets
from can.interfaces.interface import Bus

#200 -> 4,5 == speed 6,7 == ?

device = sys.argv[1]

can.rc['interface'] = 'socketcan_ctypes'
can.rc['channel'] = device
bus = Bus()
 
class Main(QtWidgets.QWidget):
 
    def __init__(self):
        super().__init__()
        self.initUI()
 
    def initUI(self):
     
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.CAN)
        self.timer.start(1) 
 
        self.lcd = QtWidgets.QLCDNumber(self)
        #set widget size
        self.lcd.setFixedSize(500,200)
        self.lcd.display(0)

        self.setGeometry(50,50,500,200)
        self.setWindowTitle("Speedometer")
 
    def CAN(self):
        while True:
            msg = bus.recv()
            if msg.arbitration_id == 200:
                break
        if len(msg.data) == 8:
            self.lcd.display(int(msg.data[4]))
         
def main():
    app = QtWidgets.QApplication(sys.argv)
    main = Main()
    main.show()

    #changes SIGING(keyboard exception) to SIG_DFL(standard signall process)
    signal.signal(signal.SIGINT, signal.SIG_DFL) 

    sys.exit(app.exec_())
 
if __name__ == "__main__":
    main()
