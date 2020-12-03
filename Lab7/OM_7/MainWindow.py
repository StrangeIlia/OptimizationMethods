# This Python file uses the following encoding: utf-8
from PySide2.QtWidgets import QWidget
from ui_mainwindow import Ui_MainWindow

class MainWindow(QWidget):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)        

    def print_html(self, html: str):
        self.ui.console.document().setHtml(html)
