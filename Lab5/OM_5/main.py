# This Python file uses the following encoding: utf-8
from typing import List
from ConsistentRefinementOfEstimates import ConsistentRefinementOfEstimates
from SimplexMethodExtensions import ArtificialBasis, LargePenaltyMethod
from SimplexMethod import SimplexTable
from os import system
from MainWindow import MainWindow
import sys

from PySide2.QtWidgets import QApplication
from RestrictionSystem import Purpose, RestrictionRow, RestrictionSystem, Sign, Variable
from DualSimplexMethod import DualSimpleMethod, DualSolutonCreator

def system_prepare(system: RestrictionSystem):
    rows: List[RestrictionRow] = []
    for v in system.__rows__:
        if v.sign == Sign.EQUALS:
            rows.append(v)
            v.sign = Sign.LESS_OR_EQUAL
    for v in rows:
        row = system.create_restriction()
        row.coefficients = [-value for value in v.coefficients]
        row.free_member = -v.free_member
        row.sign = Sign.LESS_OR_EQUAL

def base_calc(table: SimplexTable):
    html = "Начальное состояние системы:<br>"
    html += table.to_html() + "<br><br>"

    iteration = 1
    while table.one_step():
        html += "Шаг номер №" + str(iteration) + "<br>"
        html += table.to_html() + "<br><br>"
        iteration += 1
    return html


def simplex_method_calc(system: RestrictionSystem):
    return "Решение системы симплекс методом в чистом виде<br>" + base_calc(SimplexTable(system))

def artificial_basis_calc(system: RestrictionSystem):
    return "Решение системы методом искусственного базиса<br>" + base_calc(ArtificialBasis(system))

def large_penalty_method_calc(system: RestrictionSystem):
    return "Решение системы методом большого штрафа<br>" + base_calc(LargePenaltyMethod(system))

def dual_task_solution(table: SimplexTable):
    solution = DualSolutonCreator.dual_task_solution(table)
    html = "Решение двойственной задачи: f<span style=\" vertical-align:sub;\">min</span> = "
    html += "%8.5f" % (solution.free_member) + "<br>"
    for i in range(len(solution.coefficients)):
        html += "y<span style=\" vertical-align:sub;\">" + \
            str(i + 1) + "</span> = "
        html += "%8.5f" % (solution.coefficients[i]) + "; "
    return html

def dual_task_calc(system: RestrictionSystem):
    table = DualSimpleMethod(system)
    return "Решение системы двойственным симплекс методом<br>" + base_calc(table) + dual_task_solution(table)
    

def consistent_refinement_of_estimates_calc(system: RestrictionSystem):
    table = ConsistentRefinementOfEstimates(system)
    return "Решение системы методом последовательного уточнения оценок<br>" + base_calc(table) + dual_task_solution(table)

if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()

    system = RestrictionSystem()
    system.add_variable(Variable())
    system.add_variable(Variable())
    system.add_variable(Variable())
    system.add_variable(Variable())
    func = system.__objective_function__
    
    # func.coefficients = [-2, 7, 1, 4]
    # func.purpose = Purpose.MIN

    # row = system.create_restriction()
    # row.coefficients = [-4, 1, -4, 1]
    # row.free_member = 16
    # row.sign = Sign.EQUALS

    # row = system.create_restriction()
    # row.coefficients = [2, 3, 4, 2]
    # row.free_member = 32
    # row.sign = Sign.MORE_OR_EQUAL

    # row = system.create_restriction()
    # row.coefficients = [3, 5, 2, -3]
    # row.free_member = 22
    # row.sign = Sign.EQUALS



    # func.coefficients = [-1, -1]
    # func.free_member = -16
    # func.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [1, 1]
    # row.free_member = 8
    # row.sign = Sign.LESS_OR_EQUAL

    # row = system.create_restriction()
    # row.coefficients = [1, -1]
    # row.free_member = 4
    # row.sign = Sign.MORE_OR_EQUAL

    # row = system.create_restriction()
    # row.coefficients = [1, 2]
    # row.free_member = 6
    # row.sign = Sign.MORE_OR_EQUAL



    # func.coefficients = [2, -7, -1, -4]
    # func.purpose = Purpose.MAX

    # row = system.create_restriction()
    # row.coefficients = [-3, 1, -4, 1]
    # row.free_member = 20
    # row.sign = Sign.EQUALS

    # row = system.create_restriction()
    # row.coefficients = [2, 3, 4, 2]
    # row.free_member = 32
    # row.sign = Sign.MORE_OR_EQUAL

    # row = system.create_restriction()
    # row.coefficients = [4, 5, 2, -3]
    # row.free_member = 26
    # row.sign = Sign.MORE_OR_EQUAL

    func.coefficients = [-3, -5, -7, -6]
    func.purpose = Purpose.MAX

    row = system.create_restriction()
    row.coefficients = [-5, 1, -6, 1]
    row.free_member = 20
    row.sign = Sign.MORE_OR_EQUAL

    row = system.create_restriction()
    row.coefficients = [1, 3, 4, 5]
    row.free_member = 27
    row.sign = Sign.MORE_OR_EQUAL

    row = system.create_restriction()
    row.coefficients = [4, 7, 2, -3]
    row.free_member = 24
    row.sign = Sign.EQUALS

    system_prepare(system)

    dual_task_html = dual_task_calc(system)
    consistent_refinement_of_estimates_html = consistent_refinement_of_estimates_calc(
        system)

    file = open("result.html", "w")
    file.write(dual_task_html + "<br><br><br>" +
                      consistent_refinement_of_estimates_html)
    file.close()
    widget.print_html(dual_task_html + "<br><br><br>" +
                      consistent_refinement_of_estimates_html)

    sys.exit(app.exec_())
