# This Python file uses the following encoding: utf-8
from ConsistentRefinementOfEstimates import ConsistentRefinementOfEstimates
import copy
from os import system
from typing import List
from Gomori import TreeElem
from SimplexMethodExtensions import ArtificialBasis
from RestrictionSystem import Purpose, RestrictionSystem, Variable
from SimplexMethod import SimplexTable
from MainWindow import MainWindow
import sys
from PySide2.QtWidgets import QApplication

import Gomori

def base_calc(table: SimplexTable):
    html = "Начальное состояние системы:"
    html += table.to_html() + "<br><br>"

    iteration = 1
    while table.one_step():
        html += "Шаг номер №" + str(iteration)
        html += table.to_html() + "<br><br>"
        iteration += 1
    return html

def gomori_calc(system: RestrictionSystem):
    table = ArtificialBasis(system)
    html = base_calc(table)
    while not Gomori.integer_check(table):
        html += "Решение не является целочисленным, поэтому добавляем сечение<br>"
        system = Gomori.create_slice(table)
        old_basis = table.__basis__
        table = ConsistentRefinementOfEstimates(system)    
        # table = ArtificialBasis(system)
        for i in range(len(old_basis)):
            table.__basis__[i] = old_basis[i]

        html += base_calc(table)
    return html


def branches_and_borders_calc(system: RestrictionSystem):
    queue: List[TreeElem] = []
    req_variables = []
    for i in range(len(system.__objective_function__.coefficients)):
        req_variables.append(i)
    html = ""
    best_table = None
    best_value = -sys.float_info.max
    queue.append(TreeElem(system))  
    iteration = 1
    while len(queue) != 0:
        elem = queue.pop(0)
        table = elem.table
        html += "Шаг номер №" + str(iteration)
        html += table.to_html() + "<br><br>"
        iteration += 1
        if Gomori.integer_check(table):
            if table.__objective_function__.free_member < best_value:
                continue
    
            if table.__objective_function__.free_member > best_value:
                best_value = table.__objective_function__.free_member
                best_table = table

        queue += Gomori.create_branches(elem, req_variables)
    html += "Лучшая таблица - это:<br>"
    html += best_table.to_html() + "<br><br>"
    return html

if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()

    system = RestrictionSystem()
    function = system.__objective_function__
    for _ in range(5):
        system.add_variable(Variable())

    # function.free_member = 0
    # function.coefficients = [3, 1, 0, 0, -1]
    # function.purpose = Purpose.MAX
    
    # function.free_member = 6
    # function.coefficients = [0, 5, 0, 0, 0]
    # function.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [7, 5, 1, 0, 0]
    # row.free_member = 28

    # row = system.create_restriction()
    # row.coefficients = [4, -6, 0, 3, 0]
    # row.free_member = 14

    # row = system.create_restriction()
    # row.coefficients = [4/3, -2, 0, 1, 0]
    # row.free_member = 14/3

    # row = system.create_restriction()
    # row.coefficients = [-3, 4, 0, 0, 1]
    # row.free_member = 6
#----------------------------------------    
    # function.free_member = 0
    # function.coefficients = [9, -4, 0, 0, 3]
    # function.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [10, 3, 1, 0, 0]
    # row.free_member = 93

    # row = system.create_restriction()
    # row.coefficients = [14, -5, 0, -1, 0]
    # row.free_member = 26

    # row = system.create_restriction()
    # row.coefficients = [2, -9, 0, 0, -1]
    # row.free_member = 18
#-----------------------------------------
    function.free_member = 0
    function.coefficients = [1, 2, -1, 0, 0]
    function.purpose = Purpose.MAX

    row = system.create_restriction()
    row.coefficients = [7, 4, 1, 0, 0]
    row.free_member = 25

    row = system.create_restriction()
    row.coefficients = [3, -4, 0, 1, 0]
    row.free_member = 10

    row = system.create_restriction()
    row.coefficients = [-1, 3, 0, 0, 1]
    row.free_member = 3
#-----------------------------------------
    # function.free_member = 0
    # function.coefficients = [10, -5, 0, 4, 0]
    # function.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [9, 3, 1, 0, 0]
    # row.free_member = 75

    # row = system.create_restriction()
    # row.coefficients = [18, -4, 0, -3, 0]
    # row.free_member = 22

    # row = system.create_restriction()
    # row.coefficients = [2, -10, 0, 0, 1]
    # row.free_member = 18

#-----------------------------------------

    # for _ in range(2):
    #     system.add_variable(Variable())

    # function.free_member = 0
    # function.coefficients = [1, 4]
    # function.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [-1, 2]
    # row.free_member = 2
    # row.sign = Sign.LESS_OR_EQUAL

    # row = system.create_restriction()
    # row.coefficients = [3, 2]
    # row.free_member = 6
    # row.sign = Sign.LESS_OR_EQUAL

    html = gomori_calc(system)
    # html = branches_and_borders_calc(system)

    file = open("result.html", "w")
    file.write(html)
    file.close()
    widget.print_html(html)

    sys.exit(app.exec_())
