# This Python file uses the following encoding: utf-8
from SimplexMethod import SimplexTable
import copy
from typing import Optional
from RestrictionSystem import BaseRow, RestrictionSystem
from SimplexMethodExtensions import ArtificialBasis

class DualSolutonCreator:
    @staticmethod
    def dual_task_solution(table: SimplexTable) -> BaseRow:
        row = copy.deepcopy(table.__objective_function__)
        row_len = len(row.coefficients)
        shift = row_len - len(table.__rows__) 
        row.coefficients = [row.coefficients[(i + shift) % row_len]
                            for i in range(len(table.__rows__))]
        return row


class DualSimpleMethod(ArtificialBasis):
    def __init__(self, system: Optional[RestrictionSystem] = None):
        ArtificialBasis.__init__(self, system)

    def __variable_name__(self, index: Optional[int]):
        if index == None:
            return "Invalid"
        else:
            return "x<span style=\" vertical-align:sub;\">" + str(index + 1) + "</span>"
        
