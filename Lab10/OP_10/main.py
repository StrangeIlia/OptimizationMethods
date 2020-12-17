import sys

import sympy
from typing import List
import MainWindow
from PySide2.QtWidgets import QApplication


def create_phi_0(a: float, b: float, c1: float, c2: float):
    x = sympy.Symbol("x")
    return ((b - x) * c1 + (x - a) * c2) / (b - a)


def create_phi_k(k: int, a: float, b: float):
    x = sympy.Symbol("x")
    return x ** (k - 1) * (x - a) * (x - b)


def create_phi_k_sin(k: int, a: float, b: float):
    x = sympy.Symbol("x")
    return sympy.sin(k * sympy.pi * (x - a) / (b - a))


def to_pseudo_html(symbol: sympy.Symbol):
    if symbol.is_Number:
        if symbol.q != 1:
            result = ""
            if symbol.p < 0:
                result += "-\\frac{" + str(-symbol.p) + "}"
            else:
                result += "\\frac{" + str(symbol.p) + "}"
            result += "{" + str(symbol.q) + "}"
            return result
        else:
            return str(symbol)

    elif symbol.is_Mul:
        result = ""
        number = None
        for arg in symbol.args:
            if arg.is_Number:
                result += to_pseudo_html(arg)
                number = arg

        for arg in symbol.args:
            if arg != number:
                if arg.is_Add:
                    result += "(" + to_pseudo_html(arg) + ")"
                else:
                    result += to_pseudo_html(arg)
        return result

    elif symbol.is_Add:
        sym = " + "
        result = ""
        for arg in symbol.args:
            arg_html = to_pseudo_html(arg)
            if arg_html[:1] == "-":
                if result == "":
                    result += arg_html + sym
                else:
                    result = result[:-len(sym)]
                    result += " - "
                    result += arg_html[1:] + sym
            else:
                result += arg_html + sym
        return result[:-len(sym)]

    elif symbol.is_Pow:
        result = to_pseudo_html(symbol.args[0])
        result += "^" + to_pseudo_html(symbol.args[1])
        return result

    elif symbol.is_Symbol:
        return str(symbol)


# Код конечно костыльный, но зато я его быстро написал хд
if __name__ == '__main__':
    # Все для окошка
    app = QApplication([])
    windows = MainWindow.MainWindow()
    windows.show()

    # наши символы
    x = sympy.Symbol("x")
    u = sympy.Symbol("u")
    u_der = sympy.Symbol("u`")

    html = ""
    # --------------------------------------------
    # Сюда вводит входные данные
    # a = 0
    # b = 1
    # n = 2
    # c1 = 0
    # c2 = 0
    # function = u_der ** 2 + u ** 2 + 2 * x * u

    # 3
    # a = -1
    # b = 1
    # n = 2
    # c1 = 0
    # c2 = 2
    # function = x * u_der ** 2 + x ** 5 * u ** 2 + (1 + 2 * x ** 2) * u
    # function = x * u_der ** 2 + x ** 5 * u ** 2 + (1 + 2 * x ** 2) * u

    # 2
    a = -1
    b = 1
    n = 2
    c1 = 0
    c2 = 2
    function = u_der ** 2 + x ** 2 * u ** 2 + (2 + 2 * x) * u
    # function = x * u_der ** 2 + x ** 5 * u ** 2 + (1 + 2 * x ** 2) * u
    # --------------------------------------------
    html += "<p>Начальное условие: "
    html += "<mathjax>$$"
    symbol_u = to_pseudo_html(u)
    html += "I\{" + symbol_u + "\} = "
    html += "\\int\\limits_{" + str(a) + "}^{" + str(b) + "}"
    html += to_pseudo_html(function)
    html += "\\, dx, \\quad " + symbol_u + "(" + str(a) + ") = "
    html += str(c1) + ", \\quad " + symbol_u + "(" + str(b)
    html += ") = " + str(c2) + ", \\quad n = " + str(n)
    html += "$$</mathjax></p>"

    new_variables: List[sympy.Symbol] = []
    replacement = create_phi_0(a, b, c1, c2)
    html += "<p>Найдем значения <mathjax>\(\\varphi_k:\)</mathjax></p>"
    html += "<p><mathjax>$$\\varphi_0 = "
    html += to_pseudo_html(replacement)
    html += "$$</mathjax></p>"
    for i in range(n):
        C_i = sympy.Symbol("C_" + str(i + 1))
        phi_i = C_i * create_phi_k(i + 1, a, b)
        replacement += phi_i
        new_variables.append(C_i)
        html += "<p><mathjax>$$\\varphi_" + str(i + 1) + " = "
        html += to_pseudo_html(phi_i)
        html += "$$</mathjax></p>"

    html += "<p>Разложим интеграл на составляющие: "
    html += "<mathjax>$$"
    symbol_u = to_pseudo_html(u)
    html += "I\{" + symbol_u + "\} = "

    function = function.expand()
    elements = ""
    for s in function.args:
        elements += "\\int\\limits_{" + str(a) + "}^{" + str(b) + "}"
        elements += to_pseudo_html(s)
        elements += "\\, dx + "
    elements = elements[:-2]
    html += elements
    html += "$$</mathjax>"
    html += "</p>"

    html += "<p>Возьмем интегралы по составляющим: </p>"

    integral = None
    for s in function.args:
        html += "<p><mathjax>$$"
        html += "\\int\\limits_{" + str(a) + "}^{" + str(b) + "}"
        html += to_pseudo_html(s.expand())
        html += "\\, dx = "
        s = s.subs(u, replacement)
        s = s.subs(u_der, replacement.diff(x))
        html += "\\int\\limits_{" + str(a) + "}^{" + str(b) + "}"
        html += to_pseudo_html(s.expand())
        html += "\\, dx = "
        s = sympy.integrate(s, (x, a, b))
        html += to_pseudo_html(s.expand())
        if integral is None:
            integral = s
        else:
            integral += s
        html += "$$</mathjax></p>"

    integral = integral.expand()

    function_f = "Ф("
    for c in new_variables:
        function_f += to_pseudo_html(c) + ", "
    function_f = function_f[:-2] + ")"

    html += "<p>Найдем определенный интеграл данного выражения: </p>"
    html += "<p><mathjax>$$" + function_f
    html += " = I\{" + symbol_u + "\} = "
    html += to_pseudo_html(integral)
    html += "$$</mathjax></p>"

    html += "<p>Для выполнения условия экстремума необходимо решить следующию систему: </p>"
    html += "<p><mathjax>$$\\begin{cases} "

    cases = ""
    rows = []
    for symbol in new_variables:
        der = integral.diff(symbol)
        cases += " \\displaystyle \\frac{\\partial Ф}{\\partial "
        cases += to_pseudo_html(symbol) + "} = "
        cases += to_pseudo_html(der) + " = 0 \\\\"
        rows.append(sympy.Eq(der, 0))
    cases = cases[:-2]
    html += cases
    html += " \\end{cases}$$</mathjax></p>"

    solution_slay = sympy.solve(rows)
    html += "<p>Решенив данную систему мы получаем: </p>"
    html += "<p><mathjax>$$"

    variables_values = ""
    result = replacement
    for var in new_variables:
        variables_values += to_pseudo_html(var)
        variables_values += " = " + to_pseudo_html(solution_slay[var])
        variables_values += ", "
        result = result.subs(var, solution_slay[var])
    variables_values = variables_values[:-2]
    html += variables_values
    html += "$$</mathjax></p>"

    result = sympy.expand(result)

    html += "<p>В общем виде выражение для экстремали примет вид: </p>"
    html += "<p><mathjax>$$" + function_f
    html += " = I\{" + symbol_u + "\} = "
    html += to_pseudo_html(result)
    html += "$$</mathjax></p>"

    html = """
            <!DOCTYPE html>
            <html>
                <head>
                    <title>MathJax TeX Test Page</title>
                    <script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>
                    <script type="text/javascript" id="MathJax-script" async
                        src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js">
                    </script>
                </head>
                <body>
                    """ + html + """
                </body>
            </html>
            """

    # Интеграл
    # \int\limits_{-N}^{N} e^x\, dx

    # Границы для определенного интеграла
    # \Bigl. F(x) \Bigr|_A^B

    # Скобки
    # "\\begin{cases} \\displaystyle n/2 \\\\ \\displaystyle 3n+1 \\end{cases}"

    file = open("result.html", "w", encoding="utf-8")

    file.write(html)
    file.close()
    windows.print_html(html)

    sys.exit(app.exec_())
