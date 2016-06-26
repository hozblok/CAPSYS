#include "settings.h"

int output(const char * message)
{
    qDebug() << message;
    return 0;
}

#if USE_MPF_CLASS
Utils::Utils() :
    random(gmp_randinit_mt)
{
    random.seed(time(NULL));
}

Utils::~Utils()
{
}

REAL Utils::getRandom(REAL &lower, REAL &upper)
{
    return (lower + (upper - lower) * random.get_f(NUMBITS));
}

REAL Utils::getRandom()
{
    return (random.get_f(NUMBITS));
}

QString getQString(const REAL value, bool flagE)
{
    QString result = "";
    mp_exp_t exp;
    QString str = QString::fromStdString(value.get_str(exp, 10, 0));
    if (flagE)
    {
        result.append(str);
        if (str[0] == '-' || str[0] == '+')
        {
            result.insert(1, QString("0."));
        }
        else
        {
            result.insert(0, QString("0."));
        }
        result.append(QString("e"));
        result.append(QString::number(exp));
    }
    else
    {
        bool flagMinus = false;
        if (str[0] == '-')
        {
            flagMinus = true;
            str.remove(0, 1);
        }
        int lenStr = str.size();
        if (exp > 0)
        {
            if (exp >= lenStr)
            {
                for(int i = 0; i < exp - lenStr + 1; ++i)
                {
                    str.append(QString("0"));
                }
            }
            str.insert(exp, QString("."));
            result = str;
        }
        else
        {
            for (int i = 0; i < abs(exp) + 1; ++i)
            {
                result.append(QString("0"));
            }
            result.append(str);
            result.insert(1, QString("."));
        }
        if (flagMinus)
        {
            result.prepend('-');
        }
    }

    return result;
}
#endif //USE_MPF_CLASS
