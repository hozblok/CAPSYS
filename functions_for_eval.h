#ifndef FUNCTIONS_FOR_EVAL_H
#define FUNCTIONS_FOR_EVAL_H

#include "settings.h"
#include "QtCore"

typedef REAL (*mathFunction)(REAL, REAL, QByteArray &);

REAL  _and(REAL a, REAL b, QByteArray &);
REAL  _or(REAL a, REAL b, QByteArray &);
REAL  _eq(REAL a, REAL b, QByteArray &);
REAL  _lt(REAL a, REAL b, QByteArray &);
REAL  _gt(REAL a, REAL b, QByteArray &);
REAL  _add(REAL a, REAL b, QByteArray &);
REAL  _sub(REAL a, REAL b, QByteArray &);
REAL  _mul(REAL a, REAL b, QByteArray &);
REAL  _div(REAL a, REAL b, QByteArray &error);
REAL  _pow(REAL a, REAL b, QByteArray &);
REAL  _mod(REAL a, REAL b, QByteArray &error);

REAL  _ln(REAL a, REAL, QByteArray &error);
REAL  _sign(REAL a, REAL, QByteArray &);
REAL  _abs(REAL a, REAL, QByteArray &);
REAL  _sqrt(REAL a, REAL, QByteArray &);
REAL  _exp(REAL a, REAL, QByteArray &);
REAL  _sin(REAL a, REAL, QByteArray &error);
REAL  _cos(REAL a, REAL, QByteArray &error);
REAL  _tan(REAL a, REAL, QByteArray &error);
REAL  _asin(REAL a, REAL, QByteArray &error);
REAL  _acos(REAL a, REAL, QByteArray &error);
REAL  _atan(REAL a, REAL, QByteArray &error);

REAL  _zero_fun(REAL, REAL, QByteArray &);
REAL  _one_fun(REAL, REAL, QByteArray &);
REAL  _m_one_fun(REAL a, REAL b, QByteArray &);

REAL  _mul1(REAL, REAL b, QByteArray &);
REAL  _mul2(REAL a, REAL, QByteArray &);
REAL  _div1(REAL, REAL b, QByteArray &error);
REAL  _div2(REAL a, REAL b, QByteArray &error);
REAL  _pow1(REAL a, REAL b, QByteArray &);
REAL  _pow2(REAL a, REAL b, QByteArray &error);
REAL  _asin_d(REAL a, REAL, QByteArray &);
REAL  _acos_d(REAL a, REAL, QByteArray &);
REAL  _atan_d(REAL a, REAL, QByteArray &);
REAL  _cos_d(REAL a, REAL b, QByteArray &);
REAL  _log_d(REAL a, REAL, QByteArray &);
REAL  _sqrt_d(REAL a, REAL b, QByteArray &);
REAL  _tan_d(REAL a, REAL b, QByteArray &);

//dictionary with the necessary functions, and their names.
static QMap<QByteArray, mathFunction> create_map()
{
    QMap<QByteArray, mathFunction> map;
    map["sign"] = _sign;
    map["or"] = _or;
    map["and"] = _and;
    map["lt"] = _lt;
    map["gt"] = _gt;
    map["eq"] = _eq;
    map["mod"] = _mod;
    map["add"] = _add;
    map["sub"] = _sub;
    map["mul"] = _mul;
    map["div"] = _div;
    map["pow"] = _pow;

    map["ln"] = _ln;
    map["asin"] = _asin;
    map["acos"] = _acos;
    map["atan"] = _atan;
    map["abs"] = _abs;
    map["cos"] = _cos;
    map["sin"] = _sin;
    map["exp"] = _exp;
    map["sqrt"] = _sqrt;
    map["tan"] = _tan;
    return map;
}
const QMap<QByteArray, mathFunction> map = create_map();
// This (first) dictionary contains a reference to derivatives of basic functions and their names:
static QMap<QByteArray, mathFunction> create_map_d_1()
{
    QMap<QByteArray, mathFunction> mapd;
    mapd["sign"] =  _zero_fun;
    mapd["or"] =    _zero_fun;
    mapd["and"] =   _zero_fun;
    mapd["lt"] =    _zero_fun;
    mapd["gt"] =    _zero_fun;
    mapd["eq"] =    _zero_fun;
    mapd["mod"] =   _one_fun; //!!
    mapd["add"] =   _one_fun;
    mapd["sub"] =   _one_fun; //!!
    mapd["mul"] =   _mul1;
    mapd["div"] =   _div1;
    mapd["pow"] =   _pow1;

    mapd["ln"] =    _log_d;
    mapd["asin"] =  _asin_d;
    mapd["acos"] =  _acos_d;
    mapd["atan"] =  _atan_d;
    mapd["abs"] =   _sign;
    mapd["sin"] =   _cos;
    mapd["cos"] =   _cos_d;
    mapd["exp"] =   _exp;
    mapd["sqrt"] =  _sqrt_d;
    mapd["tan"] =   _tan_d;

    return mapd;
}
const QMap<QByteArray, mathFunction> map_d_1 = create_map_d_1();
// This (second) dictionary contains a reference to derivatives of basic functions and their names:
static QMap<QByteArray, mathFunction> create_map_d_2()
{
    QMap<QByteArray, mathFunction> map_d_2;
    map_d_2["sign"] =  _zero_fun;
    map_d_2["or"] =    _zero_fun;
    map_d_2["and"] =   _zero_fun;
    map_d_2["lt"] =    _zero_fun;
    map_d_2["gt"] =    _zero_fun;
    map_d_2["eq"] =    _zero_fun;
    map_d_2["mod"] =   _zero_fun;  //!!
    map_d_2["add"] =   _one_fun;
    map_d_2["sub"] =   _m_one_fun; //!!
    map_d_2["mul"] =   _mul2;
    map_d_2["div"] =   _div2;
    map_d_2["pow"] =   _pow2;
    return map_d_2;
}
const QMap<QByteArray, mathFunction> map_d_2 = create_map_d_2();

#endif // FUNCTIONS_FOR_EVAL_H
