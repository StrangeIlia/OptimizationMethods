import copy
from sys import float_info
from typing import List, Optional
from RestrictionSystem import BaseRow, Purpose, RestrictionSystem

class SimplexTable:
    __rows__: List[BaseRow] = []
    __objective_function__: BaseRow = BaseRow()
    __basis__: List[Optional[int]] = []


    def __init__(self, system: Optional[RestrictionSystem] = None):
        """
        Инициализация системой
        """
        if system != None:
            self.fill_table(system)

    def fill_table(self, system: RestrictionSystem):
        self.__basis__.clear()
        self.__rows__.clear()
        system = system.to_canonical_form()
        for row in system.__rows__:
            self.__rows__.append(copy.deepcopy(row))
            self.__basis__.append(None)
        if system.__objective_function__.purpose == Purpose.MAX:
            self.__objective_function__ = -copy.deepcopy(
                system.__objective_function__)
        else:
            self.__objective_function__ = copy.deepcopy(
                system.__objective_function__)


    def __create_basis__(self, row_index: int, variable_index: Optional[int]):
        """
        Создает базис для указанной переменной 
        в указанной столбце
        """
        selected_row = self.__rows__[row_index]
        variable_index = variable_index if variable_index != None else 0
        if abs(selected_row.coefficients[variable_index]) < 100 * float_info.epsilon:
            return False
        selected_row /= selected_row.coefficients[variable_index]
        for row in self.__rows__:
            if row != selected_row:
                row -= selected_row * row.coefficients[variable_index]
        row = self.__objective_function__
        row -= selected_row * row.coefficients[variable_index]
        self.__basis__[row_index] = variable_index
        return True


    def __valid_simplex_table__(self):
        for row in self.__rows__:
            if row.free_member < 0:
                return False
        return True

    def __smart_create_basis__(self, row_index: int, last_index: int):
        """
        Создает базис для указанной переменной
        """
        if(row_index >= len(self.__rows__)):
            return self.__valid_simplex_table__()
        if(row_index < 0):
            return False
        self.__basis__[row_index] = None
        variable_count = len(self.__objective_function__.coefficients)
        max_column = variable_count + row_index - len(self.__rows__) + 1
        for j in range(last_index + 1, max_column):
            if self.__create_basis__(row_index, j):
                if self.__smart_create_basis__(row_index + 1, j):
                    return True
        return False

    def __find_all_basis__(self):
        """
        Ищет базисы, которые были до этого
        Возвращает истина, если нашел базисы для всех строк
        """
        if self.full_basis():
            return True
        count_variable = len(self.__objective_function__.coefficients)
        for j in range(count_variable):
            first_row = None
            second_row = None
            for i in range(0, len(self.__rows__)):
                row = self.__rows__[i]
                if abs(row.coefficients[j]) > float_info.epsilon:
                    if row.free_member / row.coefficients[j] < 0:
                        second_row = i
                        break
                    if first_row != None:
                        second_row = i
                        break
                    else:
                        first_row = i
            if second_row == None and first_row != None:
                self.__create_basis__(first_row, j)
        return self.full_basis()

    def remove_basis(self, row_index: int):
        """
        Удаляет базис
        """
        self.__basis__[row_index] = None


    def init(self):
        return self.__smart_create_basis__(0, -1)
        
    def __one_step__(self):
        columns = []
        for j in range(len(self.__objective_function__.coefficients)):
            if self.__objective_function__.coefficients[j] < -float_info.epsilon:
                columns.append(j)
        columns = sorted(
            columns, key=lambda v: self.__objective_function__.coefficients[v])
        for column_number in columns:
            best_row = None
            bestValue = float_info.max
            for i in range(len(self.__rows__)):
                row = self.__rows__[i]
                if row.coefficients[column_number] > 1e-7 and row.free_member > -1e-7:
                    value = row.free_member / row.coefficients[column_number]
                    if value < bestValue:
                        bestValue = value
                        best_row = i
            if best_row != None:
                self.__create_basis__(best_row, column_number)
                return True

        return False

    def one_step(self):
        if not self.__find_all_basis__():
            self.init()
            return True
        return self.__one_step__()


    def full_basis(self):
        """
        Возвращает истина, если базисов столько же
        сколько и строк
        """
        for v in self.__basis__:
            if v == None:
                return False
        return True

    def __variable_name__(self, index: Optional[int]):
        if index == None:
            return "Invalid"
        return "x<span style=\" vertical-align:sub;\">" + str(index + 1) + "</span>"

    def to_html(self):
        """
        Записывает симлекс-таблицу в html таблицу
        """
        variable_count = len(self.__objective_function__.coefficients)
        html = "<table border=\"1\" cellpadding=\"4\" cellspacing=\"0\">"
        html += "<tr><th>Базисные переменные</th><th>Свободные члены</th>"
        for i in range(variable_count):
            html += "<th>" + self.__variable_name__(i) + "</th>"
        html += "</tr>"
        for i in range(len(self.__rows__)):
            html += "<tr><td aling=\"center\">" + \
                self.__variable_name__(self.__basis__[i]) + "</td>"
            row = self.__rows__[i]
            html += "<td aling=\"center\">" + \
                "%8.5f" % (row.free_member) + "</td>"
            for v in row.coefficients:
                html += "<td aling=\"center\">" + "%8.5f" % (v) + "</td>"
            html += "</tr>"  
        html += "<tr><td aling=\"center\">z</td>"
        row = self.__objective_function__
        html += "<td aling=\"center\">" + \
            "%8.5f" % (row.free_member) + "</td>"
        for v in row.coefficients:
            html += "<td aling=\"center\">" + "%8.5f" % (v) + "</td>"
        html += "</tr></table>"
        return html
