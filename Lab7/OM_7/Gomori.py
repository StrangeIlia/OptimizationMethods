from SimplexMethodExtensions import ArtificialBasis
import copy
from os import system
from typing import List
from RestrictionSystem import Purpose, RestrictionSystem, Sign, Variable
import math
from SimplexMethod import SimplexTable


def integer_check(table: SimplexTable):
    for row in table.__rows__:
        fraction = round(row.free_member, 7)
        fraction = fraction - math.floor(fraction)
        if fraction > 1e-7:
            return False
    return True

def create_restriction_system(table: SimplexTable):
    system = RestrictionSystem()
    for _ in range(len(table.__objective_function__.coefficients)):
        system.add_variable(Variable())

    for row in table.__rows__:
        system_row = system.create_restriction()
        system_row.free_member = row.free_member
        system_row.coefficients = copy.deepcopy(row.coefficients)
        system_row.sign = Sign.EQUALS
    function = system.__objective_function__
    function.coefficients = [
        -v for v in table.__objective_function__.coefficients]
    function.free_member = -table.__objective_function__.free_member
    function.purpose = Purpose.MAX

    return system

def __fraction_part__(value: float):
    return value - math.floor(value)


def __find_best_fraction__(table: SimplexTable):
    best_row = None
    best_fraction = -1
    for row in table.__rows__:
        fraction = round(row.free_member, 7)
        fraction = fraction - math.floor(fraction)
        if fraction > 1e-7:
            if best_fraction < fraction:
                best_fraction = fraction
                best_row = row
    return best_row

def create_slice(table: SimplexTable):
    system = create_restriction_system(table)
    row = __find_best_fraction__(table)
    if row is None:
        return system
    
    new_row = system.create_restriction()
    for i in range(len(row.coefficients)):
        new_row.coefficients.append(
            __fraction_part__(row.coefficients[i]))
    new_row.free_member = __fraction_part__(row.free_member)
    new_row.sign = Sign.MORE_OR_EQUAL

    return system

class TreeElem:
    def __init__(self, system: RestrictionSystem):
        self.table: SimplexTable = ArtificialBasis(system)
        while self.table.one_step():
            pass

        self.variables: List[Variable] = []
        for _ in range(len(self.table.__objective_function__.coefficients)):
            self.variables.append(Variable())
    
    def __valid_solution__(self):
        for i in range(len(self.table.__rows__)):
            row = self.table.__rows__[i]
            basis = self.table.__basis__[i]
            if basis is None:
                return False

            lower_bound = self.variables[basis].lower_bound
            if lower_bound is not None:
                if lower_bound > row.free_member:
                    return False
            
            upper_bound = self.variables[basis].upper_bound
            if upper_bound is not None:
                if upper_bound < row.free_member:
                    return False

        return True
        

def create_branches(elem: TreeElem, req_variables: List[int]):
    if integer_check(elem.table):
        return []
    if not elem.__valid_solution__():
        return []

    result = []
    base_system = create_restriction_system(elem.table)

    for i in range(len(base_system.__rows__)):
        row = base_system.__rows__[i]
        basis = elem.table.__basis__[i]
        value = round(row.free_member, 7)
        if basis is None:
            return []
        if basis not in req_variables:
            continue

        lower_bound = elem.variables[basis].lower_bound
        if lower_bound is not None:
            if lower_bound > value:
                continue
        
        upper_bound = elem.variables[basis].upper_bound
        if upper_bound is not None:
            if upper_bound < value:
                continue
        
        fraction = value - math.floor(value)
        if fraction < 1e-7 or value < 0:
            continue

        system = copy.deepcopy(base_system)
        new_row = system.create_restriction()
        new_row.free_member = math.floor(value)
        new_row.sign = Sign.LESS_OR_EQUAL
        new_row.coefficients = [
            1 if basis == j else 0 for j in range(len(row.coefficients))
        ]

        new_elem = TreeElem(system)
        for i in range(len(elem.variables)):
            new_elem.variables[i] = copy.deepcopy(elem.variables[i])
        new_elem.variables[basis].upper_bound = new_row.free_member
        if new_elem.__valid_solution__():
            result.append(new_elem)

        new_row.free_member += 1
        new_row.sign = Sign.MORE_OR_EQUAL
        new_elem = TreeElem(system)
        for i in range(len(elem.variables)):
            new_elem.variables[i] = copy.deepcopy(elem.variables[i])
        new_elem.variables[basis].lower_bound = new_row.free_member
        if new_elem.__valid_solution__():
            result.append(new_elem)

    return result

        


def __create_branches(table: SimplexTable):
    if integer_check(table):
        return []
    result = []
    base_system = create_restriction_system(table)


    for i in range(len(base_system.__rows__)):
        row = base_system.__rows__[i]
        basis = table.__basis__[i]
        value = round(row.free_member, 7)
        fraction = value - math.floor(value)
        if fraction < 1e-7 or value < 0:
            continue
    
        system = copy.deepcopy(base_system)
        new_row = system.create_restriction()
        new_row.free_member = math.floor(value)
        new_row.sign = Sign.LESS_OR_EQUAL
        new_row.coefficients = [
            1 if basis == j else 0 for j in range(len(row.coefficients))
        ]

        result.append((copy.deepcopy(system), basis))
        new_row.free_member += 1
        new_row.sign = Sign.MORE_OR_EQUAL
        result.append((system, basis))

    return result

