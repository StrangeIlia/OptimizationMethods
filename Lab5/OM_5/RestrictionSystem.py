from enum import Enum, unique
from typing import List, Optional

import copy

@unique
class Sign(Enum):
    EQUALS = 0
    MORE_OR_EQUAL = 1
    LESS_OR_EQUAL = -1


    def inverse(self):
        if self == Sign.MORE_OR_EQUAL:
            return Sign.LESS_OR_EQUAL
        elif self == Sign.LESS_OR_EQUAL:
            return Sign.MORE_OR_EQUAL
        else:
            return Sign.EQUALS


@unique
class Purpose(Enum):
    MIN = 0,
    MAX = 1    


class BaseRow:
    def __init__(self):
        self.coefficients: List[float] = []
        self.free_member: float = 0

    def __iadd__(self, second: 'BaseRow'):
        if len(self.coefficients) != len(second.coefficients):
            raise AttributeError()
        for i in range(len(self.coefficients)):
            self.coefficients[i] += second.coefficients[i]
        self.free_member += second.free_member
        return self

    def __isub__(self, second: 'BaseRow'):
        if len(self.coefficients) != len(second.coefficients):
            raise AttributeError()
        for i in range(len(self.coefficients)):
            self.coefficients[i] -= second.coefficients[i]
        self.free_member -= second.free_member
        return self

    def __imul__(self, value: float):
        for i in range(len(self.coefficients)):
            self.coefficients[i] *= value
        self.free_member *= value
        return self

    def __itruediv__(self, value: float):
        for i in range(len(self.coefficients)):
            self.coefficients[i] /= value
        self.free_member /= value
        return self

    def __add__(self, second: 'BaseRow'):
        return copy.deepcopy(self).__iadd__(second)

    def __sub__(self, second: 'BaseRow'):
        return copy.deepcopy(self).__isub__(second)

    def __mul__(self, value: float):
        return copy.deepcopy(self).__imul__(value)

    def __truediv__(self, value: float):
        return copy.deepcopy(self).__itruediv__(value)

    def __neg__(self):
        result = copy.deepcopy(self)  
        for i in range(len(result.coefficients)):
            result.coefficients[i] = -result.coefficients[i]
        result.free_member = -result.free_member
        return result


class ObjectiveFunction(BaseRow):
    def __init__(self):
        super().__init__()
        self.purpose: Purpose = Purpose.MAX


class RestrictionRow(BaseRow):
    def __init__(self):
        super().__init__()
        self.sign: Sign = Sign.EQUALS


class Variable:
    def __init__(self, parent: Optional['Variable'] = None):
        self.lower_bound: Optional[int] = 0
        self.upper_bound: Optional[int] = None
        self.parent: Optional['Variable'] = parent


class RestrictionSystem:
    def __init__(self):
        super().__init__()
        self.__rows__: List[RestrictionRow] = []
        self.__variables__: List[Variable] = []
        self.__objective_function__: ObjectiveFunction = ObjectiveFunction()

    def __copy_variable__(self, index: int):
        """ 
        Разрезает промежуток переменной (None, None) 
        """
        variable = self.__variables__[index]
        column = self.add_variable(Variable(variable.parent))
        for row in self.__rows__:
            row.coefficients[column] = -row.coefficients[index]


    def __invert_variable_coeff__(self, index: int):
        """ 
        Инвертирует значение переменной (None, Value) 
        """
        for row in self.__rows__:
            row.coefficients[index] = -row.coefficients[index]
        self.__objective_function__.coefficients[index] = - \
            self.__objective_function__.coefficients[index]
        variable = self.__variables__[index]
        variable.lower_bound, variable.upper_bound = variable.upper_bound, variable.lower_bound


    def __shift_variable_value__(self, index: int):
        """ 
        Смещает значение переменной по нижнему пределу 
        """
        variable = self.__variables__[index]
        value = variable.lower_bound
        if value == None:
            return
        variable.lower_bound = 0
        if variable.upper_bound != None:
            variable.upper_bound -= value
        for row in self.__rows__:
            row.free_member -= value
        self.__objective_function__.free_member -= value


    def __normalize_variables__(self):
        """ 
        Возвращает новый экземляр с нормализованными переменным 
        """
        result = RestrictionSystem()
        result.__rows__ = copy.deepcopy(self.__rows__)
        result.__objective_function__ = copy.deepcopy(
            self.__objective_function__)
        for variable in self.__variables__:
            result.__variables__.append(Variable(variable))
        result.remove_unused_variable()
        sourse_len = len(result.__variables__)
        for i in range(sourse_len):
            variable=result.__variables__[i]
            if variable.lower_bound == None and variable.upper_bound == None:
                result.__copy_variable__(i)
            elif variable.lower_bound != None and variable.upper_bound != None:
                result.__shift_variable_value__(i)
                restriction = result.create_restriction()
                restriction.sign = Sign.LESS_OR_EQUAL
                restriction.free_member = variable.upper_bound
                restriction.coefficients[i] = 1
                variable.upper_bound = None
        for i in range(len(result.__variables__)):
            variable=result.__variables__[i]
            if variable.lower_bound == None and variable.upper_bound != None:
                result.__invert_variable_coeff__(i)
            result.__shift_variable_value__(i)     
        return result


    def __to_canonical_form__(self):
        """
        Приводит систему к каноничной форме
        """
        for i in range(len(self.__rows__)):
            row = self.__rows__[i]
            if row.sign == Sign.MORE_OR_EQUAL:
                for i in range(len(row.coefficients)):
                    row.coefficients[i]=-row.coefficients[i]
                row.free_member = -row.free_member
                row.sign = Sign.LESS_OR_EQUAL
            if row.sign == Sign.LESS_OR_EQUAL:
                index = self.add_variable(Variable(self.__variables__[i]))
                row.coefficients[index] = 1
                row.sign = Sign.EQUALS
        if self.__objective_function__.purpose == Purpose.MIN:
            for i in range(len(self.__objective_function__.coefficients)):
                self.__objective_function__.coefficients[i] = \
                    -self.__objective_function__.coefficients[i]
            self.__objective_function__.purpose = Purpose.MAX


    def to_canonical_form(self):
        """
        Приводит систему к каноничной форме, 
        включает в себя обработку переменных
        """
        normalized = self.__normalize_variables__()
        normalized.remove_empty_restrictions()
        normalized.__to_canonical_form__()
        return normalized


    def add_variable(self, variable: Variable):
        """ 
        Добавляет переменную 
        """
        for row in self.__rows__:
            row.coefficients.append(0)
        self.__variables__.append(variable)
        self.__objective_function__.coefficients.append(0)
        return len(self.__variables__) - 1


    def remove_variable(self, index: int):
        """ 
        Удаляет переменную 
        """
        for row in self.__rows__:
            row.coefficients.pop(index)
        self.__variables__.pop(index)
        self.__objective_function__.coefficients.pop(index)


    def create_restriction(self):
        """ 
        Создает новое ограничение  
        """
        new_restriction = RestrictionRow()
        self.__rows__.append(new_restriction)
        return new_restriction


    def remove_restriction(self, index: int):
        """ 
        Удаляет строку ограничения 
        """
        self.__rows__.pop(index)


    def remove_empty_restrictions(self):
        """ 
        Удаляет не используемые строки 
        """
        def is_empty_row(row: RestrictionRow):
            for v in row.coefficients:
                if v != 0:
                    return False
            return True        
        self.__rows__ = [row for row in self.__rows__ if not is_empty_row(row)]

         
    def remove_unused_variable(self):
        """ 
        Удаляет не используемые переменные 
        """
        def is_empty_column(system: RestrictionSystem, index: int):
            for row in system.__rows__:
                if row.coefficients[index] != 0:
                    return False
            return True
        i = 0    
        while i != len(self.__variables__):
            if is_empty_column(self, i):
                self.remove_variable(i)
            else:
                i += 1
