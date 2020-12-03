import sys
from typing import List
from sympy import *
import numpy as np
import numpy.linalg as lg
from MainWindow import MainWindow
from PySide2.QtWidgets import QApplication

import Derivative

def to_html(symbol: Symbol):
    html = str(symbol.name)
    for i in range(len(html)):
        substr = html[i:]
        if substr.isnumeric():
            html = html[:i] + "<span style=\" vertical-align:sub;\">" + \
                substr + "</span>"
            break
    return html



def replace_symbols(funct: Symbol, variables: List[Symbol]):
    html = str(funct)
    for arg in funct.args:
        arg_str = str(arg)
        index = arg_str.find("**")
        if index != -1:
            new_arg = arg_str[:index] + \
                "<span style=\" vertical-align:super;\">"
            new_arg += arg_str[index + 2:] + "</span>"
            html = html.replace(arg_str, new_arg)
    for var in variables:
        html = html.replace(var.name, to_html(var))
    html = html.replace("*", "")
    return html


def calculate(funct: Symbol, variables: List[Symbol], value_array):
    value = y
    for i in range(len(variables)):
        value = value.subs(variables[i], value_array[i])
    return value


def deriav_to_html(deriav: Derivative.PartialDerivatives, variables: List[Symbol]):
    html = ""
    for var in variables:
        partial_derivative = deriav.__partial_derivatives__[0]
        for pd in deriav.__partial_derivatives__:
            if pd.__variables__[0] == var:
                partial_derivative = pd
                break
        html += "<p>𝜕y/𝜕" + to_html(var) + " = "
        html += replace_symbols(partial_derivative.__function__, variables)
        html += "</p>"
    return html


def hess_to_html(hess: Derivative.HessMatrix, variables: List[Symbol]):
    list_index = []
    for var in variables:
        for i in range(len(hess.__hess_matrix__)):
            if hess.__hess_matrix__[i][i].__variables__[0] == var:
                list_index.append(i)
                break
    html = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
    for row_index in range(len(list_index)):
        i = list_index[row_index]
        html += "<tr>"
        for col_index in range(len(list_index)):
            j = list_index[col_index]
            first_var = variables[row_index]
            second_var = variables[col_index]
            html += "<td>"
            html += "<p>𝜕<span style=\" vertical-align:super;\">2</span>y"
            if i == j:
                html += "/𝜕" + to_html(first_var)
                html += "<span style=\" vertical-align:super;\">2</span>"
            else:
                html += "/𝜕" + to_html(first_var)
                html += "𝜕" + to_html(second_var)
            funct = hess.__hess_matrix__[i][j].__function__
            html += " = " + replace_symbols(funct, variables)
            html += "</td>"
        html += "</tr>"
    html += "</table>"
    return html

def row_to_html(row):
    html = ""
    for v in row:
        html += "%8.5f" % (v) + "; "
    return html


def matrix_to_html(matrix):
    html = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
    for i in range(matrix.shape[0]):
        html += "<tr>"
        for j in range(matrix.shape[1]):
            html += "<td>"
            html += "%8.5f" % (matrix[i][j]) 
            html += "</td>"
        html += "</tr>"
    html += "</table>"
    return html

if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()

    x1, x2, x3 = symbols("x1,x2,x3")

    variables: List[Symbol] = []
    variables.append(x1)
    variables.append(x2)
    variables.append(x3)
    value_array = np.zeros(len(variables))

    y  = x1**2 + 2*x2**2 + 6*x3**2
    y += -2*x1*x2 + x1*x3 - 3*x2*x3
    y += 10*x1 - 7*x2 - x3 - 236

    html = "<html><body><div span style=\"font-size: 9pt\""   
    html += "<p>Начальное условие системы: </p>"
    html += "<p>y = " + replace_symbols(y, variables) + "</p>"

    difference = 1
    hess = Derivative.HessMatrix(y)
    deriav = Derivative.PartialDerivatives(y)  

    html += "<p>Частные производные: </p>"
    html += deriav_to_html(deriav, variables)

    html += "<p>Матрица Гессе: </p>"
    html += hess_to_html(hess, variables)
    html += "<br />"

    iteration_number = 1
    while difference > 1e-6:
        html += "<p>Номер итерации №" + str(iteration_number) + "</p>"

        difference = 0
        value_dir = {}
        for i in range(len(variables)):
            value_dir[variables[i]] = value_array[i]
        hess_matrix = hess(value_dir)      
        #print("Hess: " + str(hess_matrix))
        correct = lg.inv(hess_matrix)
        print(correct)
        grad = deriav(value_dir)
        html += "<p>∇y = { " + row_to_html(grad) + "}</p>"
        html += "<p>M:</p>"
        html += matrix_to_html(hess_matrix)

        #print("Grad: " + str(grad))
        grad = np.dot(correct, grad)
        for i in range(len(grad)):
            difference += grad[i] ** 2
        difference **= 0.5
        value_array -= grad
        html += "<p>x = { " + row_to_html(value_array) + "}</p>"
        html += "<br />"
        iteration_number += 1

    html += "<p>Значение функции y: "
    html += str(calculate(y, variables, value_array)) + "</p>"
    html += "<p>x = { " + row_to_html(value_array) + "}</p>"
    html += "</body></html>"
    file = open("result.html", "w", encoding="utf-8")
    
    file.write(html)
    file.close()
    widget.print_html(html)

    sys.exit(app.exec_())
