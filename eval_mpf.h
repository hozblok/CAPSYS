#ifndef EVAL_MPF_H
#define EVAL_MPF_H
#include "settings.h"
#if USE_MPF_CLASS
#include "functions_for_eval.h"

// Class for evaluating formula specified by the string
class Eval
{
private:
    bool error;                   // error in current node == false by default
    QByteArray errorDescr;        // description of the error that occurred in current node. "" by default
    QByteArray codeStr;           // the function name for current node. //examples in functions_for_eval.h
    REAL value;                   // value for some of code_str, such as "var", "num"
    int indexNameOfVar;           // if code_str=="var", here contains a variable name
    Eval *leftEval, *rightEval;   // child elements in the formula tree

    // evaluation of embedded (included) expressions
    REAL calculateThroghTree(const VEC_R &vecValuesXYZ) const;
    // evaluation of derivative of embedded (included) expressions
    REAL calculateDerivative(int indexVariable, const VEC_R &vecValuesXYZ) const;

    // check braces correctness
    bool check(const QByteArray &str);
    // try to find one of symbols lst in the string str
    int findInText(const QByteArray &str, const QByteArray &pattern);
public:
    // formula parser from QByteArray:
    Eval(QByteArray str);
    ~Eval();
    // Evaluates the formula for 'x','r'=\a x, 'y','n'=\a y, 'z','t'=\a z, 'u'=\a u
    REAL getValue(REAL x = ZERO, REAL y = ZERO, REAL z = ZERO) const;
    // Evaluates the formula for variables var
    REAL getValue(const VEC_R vecValuesXYZ) const;
    // getValueDerivative for 'x', 'y', 'z'
    REAL getValueDerivative(char diff = 'x', REAL x = ZERO, REAL y = ZERO, REAL z = ZERO) const;
    // Evaluates the derivates of the formula for variables var respect to variable diff
    REAL getValueDerivative(char diff, const VEC_R vecValuesXYZ) const;
    QByteArray getCodeStr() const {return codeStr;}
};


#endif // USE_MPF_CLASS
#endif // EVAL_MPF_H
