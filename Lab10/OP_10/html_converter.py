import sympy
import typing


def to_html(symbol: sympy.Symbol):
    html = str(symbol.name)
    for i in range(len(html)):
        substring = html[i:]
        if substring.isnumeric():
            html = html[:i] + "<span style=\" vertical-align:sub;\">" + \
                   substring + "</span>"
            break
    return html


def symbol_to_html(symbol: sympy.Symbol):
    if symbol.is_Number:
        if symbol.q != 1:
            result = "<mfrac><mrow>" + str(symbol.p)
            result += "</mrow><mrow>" + str(symbol.q)
            result += "</mrow></mfrac>"
            # result = "<span style=\"padding: 0 5px; display: inline-block; text-align: center;\">"
            # result += "<span style=\"padding: 0 5px;\">" + str(symbol.p) + "</span>"
            # result += "<span style=\"padding: 0 5px; border-top: 1px solid #000; display: block;\">"
            # result += str(symbol.q) + "</span></span>"
            return result
        else:
            return str(symbol)
    elif symbol.is_Mul or symbol.is_Add:
        sym = " * " if symbol.is_Mul else " + "
        result = ""
        for arg in symbol.args:
            result += symbol_to_html(arg) + sym
        result = result[:-len(sym)]
        return result
    elif symbol.is_Pow:
        result = symbol_to_html(symbol.args[0])
        result += "<span style=\" vertical-align:super;\">"
        result += symbol_to_html(symbol.args[1])
        result += "</span>"
        return result
    elif symbol.is_Symbol:
        numbers = "0123456789"
        for i in range(len(symbol.name), 0, -1):
            if not symbol.name[i - 1] in numbers:
                result = "<msup><mi>"
                result += symbol.name[i - 1:]
                result += "</mi><mn>"
                result += symbol.name[:i - 1]
                result += "</mn></msup>"
                return result


def replace_symbols(function: sympy.Symbol):
    result_html = str(function)
    for arg in function.args:
        result_html = result_html.replace(str(arg), symbol_to_html(arg))
    return result_html


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
