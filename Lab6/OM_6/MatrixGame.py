from typing import List
from RestrictionSystem import Purpose, RestrictionSystem, Sign, Variable
from SimplexMethod import SimplexTable
from DualSimplexMethod import DualSolutonCreator

def create_matrix_game(matrix: List[List[float]]):
    result = RestrictionSystem()

    func = result.__objective_function__
    func.purpose = Purpose.MIN

    min_value = min(matrix[0])
    for row in matrix:
        value = min(row)
        if value < min_value:
            min_value = value
    min_value = min_value if min_value < 0 else 0

    for i in range(len(matrix)):
        result.add_variable(Variable())
        func.coefficients[i] = 1
        
    for j in range(len(matrix[0])):
        row = result.create_restriction()
        row.coefficients = [matrix_row[j] - min_value for matrix_row in matrix]
        row.free_member = 1
        row.sign = Sign.MORE_OR_EQUAL           

    return result, min_value

def matrix_game_solution(table: SimplexTable):
    solution_1 = table.get_solution()
    solution_2 = DualSolutonCreator.dual_task_solution(table)
    solution_1.coefficients = solution_1.coefficients[:len(table.__rows__)]
    solution_1 /= -solution_1.free_member
    solution_2 /= -solution_2.free_member
    return solution_1.coefficients, solution_2.coefficients
