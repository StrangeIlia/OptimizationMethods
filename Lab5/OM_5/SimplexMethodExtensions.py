from typing import Optional
from RestrictionSystem import BaseRow, RestrictionSystem
from SimplexMethod import SimplexTable

class AbstractArtificialBasis(SimplexTable):
    def __init__(self, system: Optional[RestrictionSystem] = None):
        super().__init__(system)
        self.__count_addition_vatiable__: int = 0

    def __change_objective_function__(self):
        """
        Изменяет изначальную функцию
        """
        for i in range(self.__count_addition_vatiable__):
            self.__objective_function__.coefficients.append(0)

    def __count_basis_rows__(self):
        result = 0
        for v in self.__basis__:
            result += 1 if v != None else 0
        return result

    def __add_variables__(self):
        """
        Добавляет искусственные переменные
        и делает их базисными
        """
        if self.full_basis():
            return False
        if self.__count_addition_vatiable__ != 0:
            return False
        self.__count_addition_vatiable__ = len(
            self.__rows__) - self.__count_basis_rows__()
        if self.__count_addition_vatiable__ == 0:
            return True
        self.__change_objective_function__()
        variable_number = 0
        for i in range(len(self.__rows__)):
            row = self.__rows__[i]
            for j in range(self.__count_addition_vatiable__):
                if self.__basis__[i] == None and variable_number == j:
                    if row.free_member > 0:
                        row.coefficients.append(1.0)
                    else:
                        row.coefficients.append(-1.0)
                else:
                    row.coefficients.append(0.0)
            if self.__basis__[i] == None:
                variable_number += 1
        variable_number = 0
        for i in range(len(self.__rows__)):
            if self.__basis__[i] != None:
                continue
            row = self.__rows__[i]
            self.__create_basis__(
                i, len(row.coefficients) - self.__count_addition_vatiable__ + variable_number)
            variable_number += 1
        return True


    def __return_objective_function__(self):
        """
        Возвращает изначальную функцию
        """
        self.__objective_function__.coefficients = self.__objective_function__.coefficients[
            : -self.__count_addition_vatiable__]


    def __remove_variables__(self):
        """
        Удаляет искусственные переменные
        """
        if self.__count_addition_vatiable__ == 0:
            return False
        self.__return_objective_function__()
        for i in range(len(self.__rows__)):
            self.__rows__[i].coefficients = self.__rows__[
                i].coefficients[:-self.__count_addition_vatiable__]
        self.__count_addition_vatiable__ = 0
        for i in range(len(self.__basis__)):
            j = self.__basis__[i]
            if j != None:
                self.__create_basis__(i, j)
        return True


    def init(self):
        """
        Инициализирует начало обработки
        """
        if self.__find_all_basis__():
            return True
        return self.__add_variables__()

    def __can_remove_variables__(self):
        if self.__count_addition_vatiable__ == 0:
            return False
        count_old_variable = len(
            self.__objective_function__.coefficients) - self.__count_addition_vatiable__
        for v in self.__basis__:
            if v == None or v >= count_old_variable:
                return False
        return True

    def one_step(self):
        if self.__can_remove_variables__():
            return self.__remove_variables__()
        
        return super().one_step()

    def __variable_name__(self, index: Optional[int]):
        if index == None:
            return "Invalid"
        count_old_variable = len(
            self.__objective_function__.coefficients) - self.__count_addition_vatiable__
        if index < count_old_variable:
            return "x<span style=\" vertical-align:sub;\">" + str(index + 1) + "</span>"
        else:
            return "y<span style=\" vertical-align:sub;\">" + str(index + 1 - count_old_variable) + "</span>"


class ArtificialBasis(AbstractArtificialBasis):
    def __init__(self, system: Optional[RestrictionSystem] = None):
        super().__init__(system)
        self.__old_objective_function__: Optional[BaseRow] = None

    def __change_objective_function__(self):
        function = BaseRow()
        count_old_variables = len(self.__objective_function__.coefficients)
        self.__old_objective_function__ = self.__objective_function__
        self.__objective_function__ = function
        function.coefficients.clear()
        for i in range(count_old_variables):
            function.coefficients.append(0)
        for i in range(self.__count_addition_vatiable__):
            function.coefficients.append(1)


    def __return_objective_function__(self):
        if self.__old_objective_function__ != None:
            self.__objective_function__ = self.__old_objective_function__
            self.__old_objective_function__ = None


class LargePenaltyMethod(AbstractArtificialBasis):
    def __find_max_value__(self):
        max_value = 0
        for row in self.__rows__:
            for v in row.coefficients:
                if max_value < abs(v):
                    max_value = abs(v)
        return max_value

    def __change_objective_function__(self):
        max_value = 10 * self.__find_max_value__()
        function = self.__objective_function__
        for i in range(self.__count_addition_vatiable__):
            function.coefficients.append(max_value)
