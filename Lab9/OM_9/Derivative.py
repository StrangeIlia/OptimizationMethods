import copy
import numpy
from sympy import Symbol
from typing import Dict, List
from sympy.core.expr import Expr
from sympy.utilities.lambdify import lambdify

class PartialDerivative:
    def __init__(self, variables: List[Symbol], funct: Symbol):
        self.__variables__: List[Symbol] = variables
        self.__function__: Expr = funct
        for var in variables:
            self.__function__ = self.__function__.diff(var)

    def diff(self, symbols: List[Symbol]) -> 'PartialDerivative':
        result = copy.deepcopy(self)
        for var in symbols:
            result.__variables__.append(var)
            result.__function__ = self.__function__.diff(var)
        return result

    def __call__(self, dict: Dict[Symbol, float]):
        if(len(dict) != len(self.__variables__)):
            return None

class PartialDerivatives:
    def __init__(self) -> None:
        self.__partial_derivatives__: List[PartialDerivative] = []

    def __init__(self, symbol: Symbol) -> None:
        self.__partial_derivatives__: List[PartialDerivative] = []
        for var in symbol.free_symbols:
            der = PartialDerivative([var], symbol)
            self.__partial_derivatives__.append(der)

    def __call__(self, dict: Dict[Symbol, float]):
        for pd in self.__partial_derivatives__:
            if not dict.__contains__(pd.__variables__[0]):
                return None    

        list_values = []
        list_variables = []
        for var in dict:
            list_variables.append(var)
            list_values.append(dict[var])

        result = []
        for var in dict:
            for pd in self.__partial_derivatives__:
                if pd.__variables__[0] != var: 
                    continue
                funct = lambdify([list_variables], pd.__function__, 'numpy')
                result.append(funct(list_values))
        return numpy.array(result)
            
class HessMatrix:
    def __init__(self) -> None:
        self.__hess_matrix__: List[List[PartialDerivative]] = []

    def __init__(self, symbol: Symbol) -> None:
        self.__hess_matrix__: List[List[PartialDerivative]] = []
        for var in symbol.free_symbols:
            row = []
            base = PartialDerivative([var], symbol)
            for var in symbol.free_symbols:
                row.append(base.diff([var]))
            self.__hess_matrix__.append(row)
            
    # def __init__(self, derivative: PartialDerivatives) -> None:
    #     self.__hess_matrix__: List[List[PartialDerivative]] = []
    #     for pd in derivative.__partial_derivatives__:
    #         result = []
    #         base = copy.deepcopy(pd)
    #         for pd_2 in derivative.__partial_derivatives__:
    #             result.append(base.diff(pd_2.__variables__[0]))
    #         self.__hess_matrix__.append(result)

    def __call__(self, dict: Dict[Symbol, float]):
        for row in self.__hess_matrix__:
            if not dict.__contains__(row[0].__variables__[-1]):
                return None

        list_index = []
        list_values = []
        list_variables = []
        for var in dict:
            list_variables.append(var)
            list_values.append(dict[var])
            for i in range(len(self.__hess_matrix__)):
                if self.__hess_matrix__[i][i].__variables__[-1] == var:
                    list_index.append(i)
                    break

        result = []
        for i in list_index:
            row = []
            for j in list_index:
                pd = self.__hess_matrix__[i][j]
                funct = lambdify([list_variables], pd.__function__, 'numpy')
                row.append(funct(list_values))
            result.append(row)
        return numpy.array(result)
