from typing import Optional
from RestrictionSystem import RestrictionSystem
from sys import float_info
from SimplexMethod import SimplexTable

class ConsistentRefinementOfEstimates(SimplexTable):
    def __init__(self, system: Optional[RestrictionSystem] = None):
        SimplexTable.__init__(self, system)

    def __check_objective_function__(self):
        for v in self.__objective_function__.coefficients:
            if v < 0:
                return False
        return True

    def __valid_simplex_table__(self):
        valid = super().__valid_simplex_table__()
        if valid:
            return True
        return self.__check_objective_function__()
    
    def __one_step__(self):
        rows = []
        for i in range(len(self.__rows__)):
            if self.__rows__[i].free_member < 0:
                rows.append(i)
        rows = sorted(rows, key=lambda v: self.__rows__[v].free_member)
        for row_number in rows: 
            best_column = None
            bestValue = float_info.max
            row = self.__rows__[row_number]
            for j in range(len(row.coefficients)):
                if row.coefficients[j] < 0: #and abs(self.__objective_function__.coefficients[j]) > 100 * float_info.epsilon:
                    value = -self.__objective_function__.coefficients[j] / \
                        row.coefficients[j]
                    if value <= bestValue:
                        bestValue = value
                        best_column = j
            if best_column != None:
                self.__create_basis__(row_number, best_column)
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
                if abs(row.coefficients[j]) > 100 * float_info.epsilon:
                    if first_row != None:
                        second_row = i
                        break
                    else:
                        first_row = i
            if second_row == None and first_row != None:
                self.__create_basis__(first_row, j)
        return self.full_basis()

    def one_step(self):
        if not self.__find_all_basis__():
            return self.init()
        if self.__check_objective_function__():
            return self.__one_step__()
        elif super().__valid_simplex_table__():
            return super().__one_step__()
        else: 
            return self.init()
        
