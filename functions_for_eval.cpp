#include "functions_for_eval.h"

REAL _and(REAL a, REAL b, QByteArray &)
{
    a = a != ZERO && b != ZERO ? ONE : ZERO;
    return a;
}

REAL _or(REAL a, REAL b, QByteArray &)
{
    a = a != ZERO || b != ZERO ? ONE : ZERO;
    return a;
}

REAL _eq(REAL a, REAL b, QByteArray &)
{
    a = a == b ? ONE : ZERO;
    return a;
}

REAL _lt(REAL a, REAL b, QByteArray &)
{
    a = a < b ? ONE : ZERO;
    return a;
}

REAL _gt(REAL a, REAL b, QByteArray &)
{
    a = a > b ? ONE : ZERO;
    return a;
}

REAL _add(REAL a, REAL b, QByteArray &)
{
    a = a + b;
    return a;
}

REAL _sub(REAL a, REAL b, QByteArray &)
{
    a = a - b;
    return a;
}

REAL _mul(REAL a, REAL b, QByteArray &)
{
    a = a * b;
    return a;
}

REAL _div(REAL a, REAL b, QByteArray &error)
{
    if (b == ZERO)
    {
        error = QByteArray("ERROR: division by zero error!");
        output(error.constData());
        return ZERO;
    }
    a = a / b;
    return a;
}

REAL _pow(REAL a, REAL b, QByteArray &)
{
    mpfr_t base;
    mpfr_init2(base, NUMBITS);
    mpfr_t degr;
    mpfr_init2(degr, NUMBITS);
    mpfr_t result;
    mpfr_init2(result, NUMBITS);
//    mpfr_init_set_f(base, a.get_mpf_t(), MPFR_RNDN);
//    mpfr_init_set_f(degr, b.get_mpf_t(), MPFR_RNDN);
    mpfr_set_str(base, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
    mpfr_set_str(degr, getQString(b).toLocal8Bit().constData(), 10, MPFR_RNDN);
    mpfr_pow(result, base, degr, MPFR_RNDN);
    mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    mpfr_clear(base);
    mpfr_clear(degr);
    mpfr_clear(result);
    return a;
}

REAL _mod(REAL a, REAL b, QByteArray &error)
{
    if (b == ZERO)
    {
        error = QByteArray("ERROR: division by zero error!");
        output(error.constData());
        return ZERO;
    }
    mpfr_t base;
    mpfr_init2(base, NUMBITS);
    mpfr_t degr;
    mpfr_init2(degr, NUMBITS);
    mpfr_t result;
    mpfr_init2(result, NUMBITS);
//    mpfr_init_set_f(base, a.get_mpf_t(), MPFR_RNDN);
//    mpfr_init_set_f(degr, b.get_mpf_t(), MPFR_RNDN);
    mpfr_set_str(base, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
    mpfr_set_str(degr, getQString(b).toLocal8Bit().constData(), 10, MPFR_RNDN);
    mpfr_fmod(result, degr, base, MPFR_RNDN);
    mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    mpfr_clear(base);
    mpfr_clear(degr);
    mpfr_clear(result);
    return a;
}

REAL _ln(REAL a, REAL, QByteArray &error)
{
    if (a <= ZERO)
    {
        error = QByteArray("ERROR: ln(a <= 0!!!)!");
        output(error.constData());
        return ZERO;
    }
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
//    mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
    mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
    mpfr_log(result, tmp1, MPFR_RNDN);
    mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _sign(REAL a, REAL, QByteArray &)
{
    a = a < ZERO ? -ONE :(a > ZERO ? ONE : ZERO);
    return a;
}

REAL _abs(REAL a, REAL, QByteArray &)
{
    a = abs(a);
    return a;
}

REAL _sqrt(REAL a, REAL, QByteArray &)
{
    a = sqrt(a);
    return a;
}

REAL _asin(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_asin(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _acos(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_acos(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _atan(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_atan(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _exp(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_exp(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _sin(REAL a, REAL, QByteArray &)
{
//    qDebug() << "aaaa:" << getQString(a);
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);

        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
//        char *s;
//        mp_exp_t e;
//        s = mpfr_get_str(NULL, &e, 10, 510, tmp1, MPFR_RNDN);
//        qDebug() << "aaaa:" << getQString(a) << "    " << s;
//        a = "0.0";
//        mpfr_get_f(a.get_mpf_t(), tmp1, MPFR_RNDN);
//        qDebug() << "aaaa:" << getQString(a);

        mpfr_sin(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _cos(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_cos(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}

REAL _tan(REAL a, REAL, QByteArray &)
{
    mpfr_t tmp1; mpfr_init2(tmp1, NUMBITS);
    mpfr_t result; mpfr_init2(result, NUMBITS);
    try
    {
//        mpfr_init_set_f(tmp1, a.get_mpf_t(), MPFR_RNDN);
        mpfr_set_str(tmp1, getQString(a).toLocal8Bit().constData(), 10, MPFR_RNDN);
        mpfr_tan(result, tmp1, MPFR_RNDN);
        mpfr_get_f(a.get_mpf_t(), result, MPFR_RNDN);
    }
    catch(...)
    {
        mpfr_clear(tmp1);
        mpfr_clear(result);
        return ZERO;
    }
    mpfr_clear(tmp1);
    mpfr_clear(result);
    return a;
}
//----------------------------------------
REAL _zero_fun(REAL, REAL, QByteArray &)
{
    return ZERO;
}

REAL _one_fun(REAL, REAL, QByteArray &)
{
    return ONE;
}

REAL _m_one_fun(REAL a, REAL, QByteArray &)
{
    a = "-1.0";
    return a;
}
//----------------------------------------
REAL _mul1(REAL, REAL b, QByteArray &)
{
    return b;
}

REAL _mul2(REAL a, REAL, QByteArray & )
{
    return a;
}

REAL _div1(REAL, REAL b, QByteArray &error)
{
    if (b == ZERO)
    {
        error = QByteArray("ERROR: division by zero error!");
        output(error.constData());
        return ZERO;
    }
    return 1 / b;
}

REAL _div2(REAL a, REAL b, QByteArray &error)
{
    if (b == ZERO)
    {
        error = QByteArray("ERROR: division by zero error!");
        output(error.constData());
        return ZERO;
    }
    return ZERO - a / (b * b);
}

REAL _pow1(REAL a, REAL b, QByteArray &error)
{
    return (b * _pow(a, b - ONE, error));
}

REAL _pow2(REAL a, REAL b, QByteArray &error)
{
    return _ln(a, b, error) * _pow(a, b, error);
}

REAL _cos_d(REAL a,REAL b, QByteArray &error)
{
    return ZERO - _sin(a, b, error);
}

REAL _tan_d(REAL a, REAL b, QByteArray &error)
{
    return ONE / (_cos(a, b, error)*_cos(a, b, error));
}

REAL _asin_d(REAL a, REAL, QByteArray &)
{
    return ONE / sqrt(ONE - a * a);
}

REAL _acos_d(REAL a, REAL, QByteArray &)
{
    return - ONE / sqrt(ONE - a * a);
}

REAL _atan_d(REAL a, REAL, QByteArray &)
{
    return ONE / (ONE + a * a);
}

REAL _sqrt_d(REAL a, REAL b, QByteArray &)
{
    b = "0.5";
    return b / sqrt(a);
}

REAL _log_d(REAL a, REAL, QByteArray &)
{
    return ONE / a;
}
