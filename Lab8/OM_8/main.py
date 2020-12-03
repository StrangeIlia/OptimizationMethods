from os import system
from RestrictionSystem import ObjectiveFunction, Purpose, RestrictionSystem, Sign, Variable
from SimplexMethod import SimplexTable
from MainWindow import MainWindow
import sys
from PySide2.QtWidgets import QApplication
import copy
import SimplexMethodExtensions

def base_calc(table: SimplexTable):
    html = "Начальное состояние системы:"
    html += table.to_html() + "<br><br>"

    iteration = 1
    while table.one_step():
        html += "Шаг номер №" + str(iteration)
        html += table.to_html() + "<br><br>"
        iteration += 1
    return html


def convert_task(base_system: RestrictionSystem, divider: ObjectiveFunction):
    if len(base_system.__objective_function__.coefficients) != len(divider.coefficients):
        return None

    base_system = base_system.to_canonical_form()
    divider = copy.deepcopy(divider)
    new_variable_count  = len(base_system.__objective_function__.coefficients)
    new_variable_count -= len(divider.coefficients)
    for _ in range(new_variable_count):
        divider.coefficients.append(0)

    system = RestrictionSystem()
    function = system.__objective_function__
    for _ in range(len(divider.coefficients)):
        system.add_variable(Variable())
    function.purpose = Purpose.MAX
    function.coefficients = copy.deepcopy(base_system.__objective_function__.coefficients)
    function.coefficients.append(base_system.__objective_function__.free_member)
    function.free_member = 0

    for base_row in base_system.__rows__:
        row = system.create_restriction()
        row.coefficients = copy.deepcopy(base_row.coefficients)
        row.coefficients.append(-base_row.free_member)
        row.free_member = 0

    row = system.create_restriction()
    row.coefficients = copy.deepcopy(divider.coefficients)
    row.coefficients.append(divider.free_member)
    row.free_member = 1

    return system

if __name__ == "__main__":
    app = QApplication([])
    widget = MainWindow()
    widget.show()

    system = RestrictionSystem()
    function = system.__objective_function__
    for _ in range(4):
        system.add_variable(Variable())


    function.free_member = 0
    function.coefficients = [9, 3, 2, 1]
    function.purpose = Purpose.MAX

    row = system.create_restriction()
    row.coefficients = [4, 1, 1, 3]
    row.sign = Sign.LESS_OR_EQUAL
    row.free_member = 250

    row = system.create_restriction()
    row.coefficients = [1, 0, 3, 1]
    row.sign = Sign.LESS_OR_EQUAL
    row.free_member = 80

    row = system.create_restriction()
    row.coefficients = [1, 3, 1, 0]
    row.sign = Sign.LESS_OR_EQUAL
    row.free_member = 340

    divider = ObjectiveFunction()
    divider.coefficients = [1, 1, 1, 1]
    divider.free_member = 0

    new_system = convert_task(system, divider)

    table = SimplexMethodExtensions.ArtificialBasis(new_system)
    html = base_calc(table)

    variable_count = len(table.__objective_function__.coefficients)
    variable_count -= 1
    divider_value = None
    for i in range(len(table.__basis__)):
        if table.__basis__[i] == variable_count:
            divider_value = table.__rows__[i].free_member
            break
    
    if divider_value is None:
        html += "Решения нет"
    else:
        html += "z<span style=\" vertical-align:sub;\">max</span> = "
        html += "%8.5f" % (table.__objective_function__.free_member)
        html += "<tr />"

        html += "x = { "
        for i in range(variable_count):
            variable_value = 0
            for j in range(len(table.__basis__)):
                if i == table.__basis__[j]:
                    variable_value = table.__rows__[j].free_member
                    break
            variable_value /= divider_value
            html += "%8.5f" % (variable_value) + "; "
        html += "}"

    file = open("result.html", "w")
    file.write(html)
    file.close()
    widget.print_html(html)

    sys.exit(app.exec_())
