import sys

import sympy
from typing import List
import html_converter
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
    # --------------------------------------------

    new_variables: List[sympy.Symbol] = []
    replacement = create_phi_0(a, b, c1, c2)
    for i in range(n):
        C_i = sympy.Symbol("C" + str(i + 1))
        # replacement += C_i * create_phi_k_sin(i + 1, a, b)
        replacement += C_i * create_phi_k(i + 1, a, b)
        new_variables.append(C_i)

    new_function = function
    new_function = new_function.subs(u, replacement)
    new_function = new_function.subs(u_der, replacement.diff(x))

    print("Измененный интеграл: " + str(new_function))

    integral = sympy.integrate(new_function, (x, a, b))

    rows = []
    for symbol in integral.free_symbols:
        der = integral.diff(symbol)
        rows.append(sympy.Eq(der, 0))
    solution_slay = sympy.solve(rows)

    result = replacement
    for (key, value) in solution_slay.items():
        result = result.subs(key, value)

    print(result)

    result = sympy.expand(result)
    print(result)

    # html = html_converter.replace_symbols(result)
    # html = "<html><head>" \
    #        "<script>MathJax = {  loader: {load: ['input/asciimath', 'output/chtml']}}</script>" \
    #        "<script src=\"https://polyfill.io/v3/polyfill.min.js?features=es6\"></script>" \
    #        "<script type=\"text/javascript\" id=\"MathJax-script\" async" \
    #        " src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/startup.js\">" \
    #        "</script></head><body>" + \
    #        html + \
    #        "</body></html>"

    html = str(result)
    html = html.replace("**", "^")
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
                    <p>
                        <mathjax>
                            $$
                            """ + html + """
                            $$
                        </mathjax>
                    </p>
                </body>
            </html>
            """
    file = open("result.html", "w", encoding="utf-8")

    file.write(html)
    file.close()
    windows.print_html(html)

    sys.exit(app.exec_())
