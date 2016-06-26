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
        //qDebug() << codeStr << "current" << string;
        return;
    }
    string = string.toLower();
    string.replace(QByteArray(" "), QByteArray(""));

    // remove braces
    if (string.startsWith("(") && string.endsWith(")") && check(string))
    {
        string.remove(0,1);
        string.chop(1);
    }

    // lowest priority -- logical
    int n;
    n = findInText(string, QByteArray("&|"));
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
        //qDebug() << codeStr << "current" << string;
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
        //qDebug() << codeStr << "current" << string;
        return;
    }
    // normal priority -- additions
    n = findInText(string, QByteArray("+-"));
//    //qDebug() << "@@" << n << string << (n<2 || string.mid(n-1,1)!=QByteArray("e") || (string.mid(n-2,1)!=QByteArray(".") && !isdigit((int)string.mid(n-2,1).constData())));
    if(n >= 0)
    {
        if (string[n] == '+')
        {
            codeStr = "add";
        }
        else
        {
            codeStr = "sub";
        }
        //qDebug() << codeStr << "current" << string;
        //qDebug() << codeStr << "leftEval" << string.left(n);
        //qDebug() << codeStr << "rightEval" << string.mid(n + 1);
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
        //qDebug() << codeStr << "current" << string;
        //qDebug() << "mul leftEval" << string.left(n);
        //qDebug() << "mul rightEval" << string.mid(n + 1);
        return;
    }
    // highest priority -- power
    n = findInText(string, QByteArray("^"));
    if (n >= 0)
    {
        codeStr = "ipow";
        leftEval = new Eval(string.left(n));
        rightEval = new Eval(string.mid(n + 1));
        //qDebug() << codeStr << "current" << string;
        return;
    }
    int len = string.size();
    for (n = 0; n < len; ++n)
    {
        if (string[n] == '(')
        {
            break;
        }
    }
    if (n >= len) // "var" or "rnd" or "num" or "pi"
    {
        // available variables
        if (string.size() == 1 && string[0] >= 'a' && string[0] <= 'z')
        {
            codeStr = "var";
            indexNameOfVar = indexesOfVariables.value(string[0], -1);
            if (indexNameOfVar == -1)
            {
                error = true;
                errorDescr = QByteArray("ERROR: unknown variable: ") + string[0];
                //qDebug() << codeStr << "current" << string << errorDescr;
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
                //qDebug() << codeStr << "current" << string << errorDescr;
                return;
            }
        }
    }
    else if (n < len) // this is function:
    {
        QByteArray name_fun;
        name_fun.append(string.left(n));
        string.remove(0, n + 1);
        string.chop(1);
        QMap<QByteArray, mathFunction>::const_iterator it;
        bool flag_found = false;
        for (it = map.begin(); it != map.end(); ++it)
        {
            //qDebug() << name_fun << it.key() << codeStr << string.left(n) << "######current" << string << errorDescr;
            if (name_fun.indexOf(it.key()) == 0)
            {
                codeStr.clear();
                codeStr.append(it.key());
                flag_found = true;
            }
            if (flag_found)
            {
                break;
            }

        }
        if (!flag_found)
        {
            error = true;
            errorDescr = QByteArray("unknown function: ") + it.key();
            //qDebug() << codeStr << "current" << string << errorDescr;
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
    //qDebug() << codeStr << "!current" << string << errorDescr;
}

Eval::~Eval()
{
    if (leftEval)
    {
        delete leftEval;
    }
    if (rightEval)
    {
        delete rightEval;
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
    vecValuesXYZ[indexesOfVariables['x']] = x;
    vecValuesXYZ[indexesOfVariables['y']] = y;
    vecValuesXYZ[indexesOfVariables['z']] = z;
    return calculateThroghTree(vecValuesXYZ);
}

REAL Eval::getValue(const VEC_R vecValuesXYZ) const
{
    if (vecValuesXYZ.size() != sizeColumns)
    {
        output("ERROR: Eval::getValue: Invalid vector format!");
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
    vecValuesXYZ[indexesOfVariables['x']] = x;
    vecValuesXYZ[indexesOfVariables['y']] = y;
    vecValuesXYZ[indexesOfVariables['z']] = z;
    return calculateDerivative(indexesOfVariables[diff], vecValuesXYZ);
}


REAL Eval::getValueDerivative(char diff, const VEC_R vecValuesXYZ) const
{
    if (vecValuesXYZ.size() != sizeColumns)
    {
        output("ERROR: Eval::getValueDerivative: Invalid vector format!");
        return ZERO;
    }
    return calculateDerivative(indexesOfVariables[diff], vecValuesXYZ);
}

REAL Eval::calculateThroghTree(const VEC_R &vecValuesXYZ) const
{
    //qDebug() << "codeStr" << codeStr;
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
        //qDebug() << "vecValuesXYZ[indexNameOfVar]" << getQString(vecValuesXYZ[indexNameOfVar]);
        return vecValuesXYZ[indexNameOfVar];
    }

    REAL a("0.0", NUMBITS);
    REAL b("0.0", NUMBITS);
//    //qDebug() << "leftEval->codeStr" <<  leftEval->codeStr << "rightEval->codeStr" << rightEval->codeStr;
    if (leftEval->getCodeStr() != QByteArray("404"))
    {
        a = leftEval->calculateThroghTree(vecValuesXYZ);
    }
    if (rightEval->getCodeStr() != QByteArray("404"))
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

bool Eval::check(QByteArray &str)
{
    int s = 0;
    for (int i = 1; i < str.size() - 1; i++)
    {
        if (str[i] == '(')
        {
            s++;
        }
        if (str[i] == ')')
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

int Eval::findInText(QByteArray str, QByteArray pattern)
{
    int l = 0, r = 0;
    for (int i = str.size() - 1; i >= 0; --i)
    {
        if (str[i]=='(')
        {
            l++;
        }
        if (str[i]==')')
        {
            r++;
        }
        if (l == r && pattern.indexOf(str[i]) >= 0)
        {
            return i;
        }
    }
    return -1;
}

#endif // USE_MPF_CLASS
