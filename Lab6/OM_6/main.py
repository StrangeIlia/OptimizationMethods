# This Python file uses the following encoding: utf-8
from typing import List
from SimplexMethod import SimplexTable
from MainWindow import MainWindow
import sys
from PySide2.QtWidgets import QApplication

import MatrixGame

def base_calc(table: SimplexTable):
    html = "Начальное состояние системы:"
    html += table.to_html() + "<br><br>"

    iteration = 1
    while table.one_step():
        html += "Шаг номер №" + str(iteration)
        html += table.to_html() + "<br><br>"
        iteration += 1
    return html

def matrix_game_calc(matrix: List[List[float]]):
    html = "Начальное состояние системы:<br>"
    for row in matrix:
        html += str(row) + "<br>"
    system, min_value = MatrixGame.create_matrix_game(matrix)
    html += "Для решения будем использовать метод симлекс-метод в чистом виде<br>"
    table = SimplexTable(system)
    html += base_calc(table)
    p, q = MatrixGame.matrix_game_solution(table)
    html += "p = [" + '; '.join(["%8.5f" % (v) for v in p]) + " ]<br>"
    html += "q = [" + '; '.join(["%8.5f" % (v) for v in q]) + " ]<br>"
    html += "Цена игры = " 
    html += "%8.5f" % (1 / table.__objective_function__.free_member +
                       min_value) + "<br>"
    return html

if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()

    matrix = [
        [8.0, 4.0, 5.0, 9.0],
        [7.0, 10.0, 6.0, 7.0],
        [9.0, 3.0, 5.0, 6.0]
    ]

    # matrix = [
    #     [2, -3, 4],
    #     [-3, 4, -5],
    #     [4, -5, 6]
    # ]

    # matrix = [
    #     [8, 3, 10],
    #     [3, 10, 1],
    #     [10, 1, 12]
    # ]

    # matrix = [
    #     [8.0, 5.0, 7.0, 6.0],
    #     [9.0, 8.0, 10.0, 7.0],
    #     [12.0, 6.0, 4.0, 3.0],
    #     [7.0, 13.0, 5.0, 2.0]
    # ]

    html = matrix_game_calc(matrix)

    file = open("result.html", "w")
    file.write(html)
    file.close()
    widget.print_html(html)

    sys.exit(app.exec_())
