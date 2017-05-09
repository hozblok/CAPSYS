#include "eval_mpf.h"
#if USE_MPF_CLASS

Eval::Eval(QByteArray string) :
    error(false),
    errorDescr(""),
    codeStr(""),
    value("0.0", NUMBITS),
    indexNameOfVar(-1),
    leftEval(0),
    rightEval(0)
{
    codeStr = "404";
    if (string.isEmpty() || string.isNull())
    {
        codeStr = "num";
        value = "0.0";
        return;
    }
    string = string.toLower();

    // remove spaces
    string.replace(QByteArray(" "), QByteArray(""));
    // remove braces
    if (string.startsWith("(") && string.endsWith(")") && check(string))
    {
        string.remove(0, 1);
        string.chop(1);
    }

    // lowest priority -- logical
    int n = findInText(string, QByteArray("&|"));
    if (n >= 0)
    {
        if (string[n] == '|')
        {
            codeStr = QByteArray("or");
        }
        else
        {
            codeStr = QByteArray("and");
        }
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        return;
    }
    // low priority -- conditions
    n = findInText(string, QByteArray("<>="));
    if (n >= 0)
    {
        if (string[n] == '<')
        {
            codeStr = "lt";
        }
        else if (string[n] == '>')
        {
            codeStr = "gt";
        }
        else
        {
            codeStr = "eq";
        }
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        return;
    }
    // normal priority -- additions
    n = findInText(string, QByteArray("+-"));
    if (n >= 0)
    {
        if (string[n] == '+')
        {
            codeStr = "add";
        }
        else
        {
            codeStr = "sub";
        }
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        return;
    }
    // high priority -- multiplications
    n = findInText(string, QByteArray("*/"));
    if (n >= 0)
    {
        if (string[n] == '*')
        {
            codeStr = "mul";
        }
        else
        {
            codeStr = "div";
        }
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        return;
    }
    // highest priority -- power
    n = findInText(string, QByteArray("^"));
    if (n >= 0)
    {
        codeStr = "ipow";
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        return;
    }

    int len = string.size();
    n = string.indexOf('(');
    if (n == len - 1) // this is error, delete last '('
    {
        string.chop(1);
        n = -1;
        len--;
    }
    if (n == -1) // there is no '(' => codeStr == "var" or "rnd" or "num" or "pi"
    {
        if (len == 1 && string.at(0) >= 'a' && string.at(0) <= 'z') // x or y or z or other available variables
        {
            codeStr = "var";
            try
            {
                indexNameOfVar = indexesOfVar.at(string[0]);
            }
            catch(...) // std::out_of_range
            {
                indexNameOfVar = -1;
            }
            if (indexNameOfVar == -1)
            {
                error = true;
                errorDescr = QByteArray("ERROR: unknown variable: ") + string[0];
                return;
            }
        }
        else if (string.indexOf("rnd") == 0)
        {
            codeStr = "rnd";
        }
        else if (string.indexOf("pi") == 0)
        {
            codeStr = "num";
            value = M_PI_STR;
        }
        else // this is number
        {
            codeStr = "num";
            try
            {
                value = string.data();
            }
            catch(...)
            {
                value = "0.0";
                error = true;
                errorDescr = QByteArray("ERROR: unknown number: ") + string;
                return;
            }
        }
    }
    else if (n < len) // this is a function:
    {
        QByteArray name_fun = string.left(n);
        string.remove(0, n + 1);
        string.chop(1);
        QMap<QByteArray, mathFunction>::const_iterator it;
        bool flag_found = false;
        for (it = map.begin(); it != map.end(); ++it)
        {
            if (name_fun == it.key())
            {
                codeStr.clear();
                codeStr.append(it.key());
                flag_found = true;
                break;
            }
        }
        if (!flag_found)
        {
            error = true;
            errorDescr = QByteArray("ERROR: unknown function: ") + it.key();
            return;
        }
        n = findInText(string, QByteArray(","));
        if (n >= 0)
        {
            leftEval = new Eval(string.left(n));
            rightEval = new Eval(string.mid(n + 1));
        }
        else
        {
            leftEval = new Eval(string);
        }
    }
}

Eval::~Eval()
{
    if (leftEval != 0)
    {
        delete leftEval;
        leftEval = 0;
    }
    if (rightEval != 0)
    {
        delete rightEval;
        rightEval = 0;
    }
}

REAL Eval::getValue(REAL x, REAL y, REAL z) const
{
    VEC_R vecValuesXYZ(sizeColumns);
    for (int i = 0; i < sizeColumns; ++i)
    {
        vecValuesXYZ[i] = "0.0";
        vecValuesXYZ[i].set_prec(NUMBITS);
    }
    vecValuesXYZ[indexesOfVar.at('x')] = x;
    vecValuesXYZ[indexesOfVar.at('y')] = y;
    vecValuesXYZ[indexesOfVar.at('z')] = z;
    return calculateThroghTree(vecValuesXYZ);
}

REAL Eval::getValue(const VEC_R vecValuesXYZ) const
{
    if (vecValuesXYZ.size() != sizeColumns)
    {
        print("ERROR: Eval::getValue: Invalid vector format!");
        return ZERO;
    }
    return calculateThroghTree(vecValuesXYZ);
}

REAL Eval::getValueDerivative(char diff, REAL x, REAL y, REAL z) const
{
    VEC_R vecValuesXYZ(sizeColumns);
    for (int i = 0; i < sizeColumns; ++i)
    {
        vecValuesXYZ[i] = "0.0";
        vecValuesXYZ[i].set_prec(NUMBITS);
    }
    vecValuesXYZ[indexesOfVar.at('x')] = x;
    vecValuesXYZ[indexesOfVar.at('y')] = y;
    vecValuesXYZ[indexesOfVar.at('z')] = z;
    return calculateDerivative(indexesOfVar.at(diff), vecValuesXYZ);
}


REAL Eval::getValueDerivative(char diff, const VEC_R vecValuesXYZ) const
{
    if (vecValuesXYZ.size() != sizeColumns)
    {
        print("ERROR: Eval::getValueDerivative: Invalid vector format!");
        return ZERO;
    }
    return calculateDerivative(indexesOfVar.at(diff), vecValuesXYZ);
}

REAL Eval::calculateThroghTree(const VEC_R &vecValuesXYZ) const
{
//    qDebug() << " dbg @calculateThroghTree: codeStr" << codeStr;
    if (codeStr == "rnd")
    {
        return utils.getRandom();
    }
    else if (codeStr == "num")
    {
        //qDebug() << "value" << getQString(value);
        return value;
    }
    else if (codeStr == "var")
    {
//        qDebug() << "vecValuesXYZ[indexNameOfVar]" << QString::fromStdString(getString(vecValuesXYZ[indexNameOfVar]));
        return vecValuesXYZ[indexNameOfVar];
    }

    REAL a("0.0", NUMBITS), b("0.0", NUMBITS);
    if (leftEval != 0 && leftEval->getCodeStr() != QByteArray("404"))
    {
        a = leftEval->calculateThroghTree(vecValuesXYZ);
    }
    if (rightEval != 0 && rightEval->getCodeStr() != QByteArray("404"))
    {
        b = rightEval->calculateThroghTree(vecValuesXYZ);
    }

    mathFunction function = map.value(codeStr, _zero_fun);
    QByteArray error_message("");
    //qDebug() << "!codeStr" << codeStr << getQString(a) << getQString(b) << getQString(function(a, b, error_message)) << error_message;
    return function(a, b, error_message);
}

REAL Eval::calculateDerivative(int indexVariable, const VEC_R &vecValuesXYZ) const
{
    if (codeStr == "rnd")
    {
        return ZERO;
    }
    else if(codeStr == "num")
    {
        return ZERO;
    }
    else if(codeStr == "var")
    {
        if (indexVariable == indexNameOfVar)
        {
            REAL ONE("1.0", NUMBITS);
            return ONE;
        }
        else
        {
            return ZERO;
        }
    }

    REAL a("0.0",NUMBITS), d("0.0",NUMBITS);
    a = leftEval->calculateThroghTree(vecValuesXYZ);
    d = leftEval->calculateDerivative(indexVariable,vecValuesXYZ);

    //?? тут явно нужна база данных настроек - ещё один файлик sqlite
    if (codeStr == "lt"
            || codeStr == "gt"
            || codeStr == "eq"
            || codeStr == "or"
            || codeStr == "and"
            || codeStr == "add"
            || codeStr == "sub"
            || codeStr == "mul"
            || codeStr == "div"
            || codeStr == "pow"
            || codeStr == "mod") // functions of two variables
    {
        REAL b("0.0", NUMBITS);
        REAL c("0.0", NUMBITS);
        b = rightEval->calculateThroghTree(vecValuesXYZ);
        c = rightEval->calculateDerivative(indexVariable,vecValuesXYZ);
        mathFunction function_1 = map_d_1.value(codeStr, _zero_fun);
        mathFunction function_2 = map_d_2.value(codeStr, _zero_fun);
        QByteArray error_message("");
        b = (function_1(a, b, error_message) * d) + (function_2(a, b, error_message) * c);
        return b;
    }
    else // functions of one variable
    {
        mathFunction function_1 = map_d_1.value(codeStr, _zero_fun);
        QByteArray error_message("");
        a = function_1(a, ZERO, error_message) * d;
        return a;
    }
    return ZERO;
}

bool Eval::check(const QByteArray &str)
{
    int s = 0;
    //?? тут явно надо представить более красивое решение
    for (int i = 1; i < str.size() - 1; i++)
    {
        if (str[i] == '(')
        {
            s++;
        }
        else if (str[i] == ')')
        {
            s--;
        }
        if (s < 0)
        {
            return false;
        }
    }
    return (s == 0) ? true : false;
}

int Eval::findInText(const QByteArray &str, const QByteArray &pattern)
{
    int countLBraces = 0, countRBraces = 0;
    //for (int i = str.size() - 1; i >= 0; --i)
    for (QByteArray::const_iterator it = str.cend(); it != str.cbegin(); )
    {
        --it;
        //if (str.at(i) == '(')
        if (*it == '(')
        {
            countLBraces++;
        }
        //else if (str.at(i) == ')')
        else if (*it == ')')
        {
            countRBraces++;
        }
        //if (countLBraces == countRBraces && pattern.indexOf(str.at(i)) >= 0)
        if (countLBraces == countRBraces && pattern.indexOf(*it) >= 0)
        {
            //return i;
            return (it - str.cbegin());
        }
    }
    return -1;
}

#endif // USE_MPF_CLASS
