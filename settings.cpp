#include "settings.h"

void print(const char * message, bool information)
{
    qDebug() << message;
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

REAL Utils::getRandom(const REAL * const lower, const REAL * const upper)
{
    return (*lower + (*upper - *lower) * random.get_f(NUMBITS));
}


REAL Utils::getRandom()
{
    return (random.get_f(NUMBITS));
}

int Utils::getSimpleRandomInt(int max)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    return qrand() % (max + 1);
}

// convert REAL to string, for example "0.000000000314" or "3.14e-10" then flagE = true
std::string getString(const REAL value, bool flagE)
{
    std::string result = "";
    mp_exp_t exp;
    std::string str = value.get_str(exp, 10, 0);
    if (flagE)
    {
        result.append(str);
        if (str[0] == '-' || str[0] == '+')
        {
            result.insert(1, std::string("0."));
        }
        else
        {
            result.insert(0, std::string("0."));
        }
        result.append("e");
        result.append(IntToStr(exp));
    }
    else
    {
        bool flagMinus = false;
        if (str[0] == '-')
        {
            flagMinus = true;
            str.erase(0, 1);
        }
        int lenStr = str.size();
        if (exp > 0)
        {
            if (exp >= lenStr)
            {
                for(int i = 0; i < exp - lenStr + 1; ++i)
                {
                    str.append("0");
                }
            }
            str.insert(exp, ".");
            result = str;
        }
        else
        {
            for (int i = 0; i < abs(exp) + 1; ++i)
            {
                result.append("0");
            }
            result.append(str);
            result.insert(1, ".");
        }
        if (flagMinus)
        {
            result.insert(0,"-");
        }
    }

    return result;
}
#endif //USE_MPF_CLASS
