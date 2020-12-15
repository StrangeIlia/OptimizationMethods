import sympy
from typing import List


def create_phi_0(a: float, b: float, c1: float, c2: float):
    x = sympy.Symbol("x")
    return ((b - x) * c1 + (x - a) * c2) / (b - a)


def create_phi_k(k: int, a: float, b: float):
    x = sympy.Symbol("x")
    return x ** (k - 1) * (x - a) * (x - b)


# Код конечно костыльный, но зато я его быстро написал хд
if __name__ == '__main__':
    x = sympy.Symbol("x")
    u = sympy.Symbol("u")
    u_der = sympy.Symbol("u`")
    # --------------------------------------------
    # Сюда вводит входные данные
    a = 0
    b = 1
    n = 2
    c1 = 0
    c2 = 0
    function = u_der ** 2 + u ** 2 + 2 * x * u
    #--------------------------------------------

    new_variables: List[sympy.Symbol] = []
    replacement = create_phi_0(a, b, c1, c2)
    for i in range(n):
        C_i = sympy.Symbol("C" + str(i + 1))
        replacement += C_i * create_phi_k(i + 1, a, b)
        new_variables.append(C_i)

    new_function = function
    new_function = new_function.subs(u, replacement)
    new_function = new_function.subs(u_der, replacement.diff(x))

    integral = sympy.integrate(new_function, (x, a, b))

    rows = []
    for symbol in integral.free_symbols:
        der = integral.diff(symbol)
        rows.append(sympy.Eq(der, 0))
    solution_slay = sympy.solve(rows)

    result = replacement
    for (x, y) in solution_slay.items():
        result = result.subs(x, y)

    print(result)


