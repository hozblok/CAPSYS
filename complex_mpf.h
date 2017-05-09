#ifndef COMPLEX_MPF_H
#define COMPLEX_MPF_H

#include "functions_for_eval.h"

class complex_mpf
{
public:
    REAL real;
    REAL imag;

    complex_mpf():
        real("0.0", NUMBITS), imag("0.0", NUMBITS)
    {
    }

    complex_mpf(REAL a, REAL b):
        real("0.0", NUMBITS), imag("0.0", NUMBITS)
    {
        real = a;
        imag = b;
    }

    complex_mpf(const complex_mpf &Orig):
        real("0.0", NUMBITS), imag("0.0", NUMBITS)
    {
        real = Orig.real;
        imag = Orig.imag;
    }

    complex_mpf operator +(const complex_mpf &op2)
    {
        complex_mpf temp;
        temp.real = real + op2.real;
        temp.imag = imag + op2.imag;
        return temp;
    }

    complex_mpf operator -(const complex_mpf &op2)
    {
        complex_mpf temp;
        temp.real = real - op2.real;
        temp.imag = imag - op2.imag;
        return temp;
    }

    complex_mpf operator *(const complex_mpf &op2)
    {
        complex_mpf temp;
        temp.real = real * op2.real - imag * op2.imag;
        temp.imag = real * op2.imag + imag * op2.real;
        return temp;
    }

    complex_mpf operator /(const complex_mpf &op2)
    {
        complex_mpf temp;
        REAL t("0.0", NUMBITS);
        t = op2.real * op2.real + op2.imag * op2.imag;
        temp.real = (real * op2.real + imag * op2.imag) / t;
        temp.imag = (imag * op2.real - real * op2.imag) / t;
        return temp;
    }

    complex_mpf operator /(REAL op2)
    {
        complex_mpf temp;
        temp.real = real / op2;
        temp.imag = imag / op2;
        return temp;
    }

    complex_mpf operator =(const complex_mpf &op2)
    {
        real = op2.real;
        imag = op2.imag;
        return *this;
    }

    bool operator ==(const complex_mpf &op2)
    {
        if ((real == op2.real) && (imag == op2.imag)) return true;
        else return false;
    }

    REAL mod()
    {
        QByteArray error;
        return _sqrt(real * real + imag * imag, ZERO, error);
    }

    REAL arg()
    {
        REAL tmp("0.0", NUMBITS);
        QByteArray error;
        tmp = _atan(imag / real, ZERO, error);
        if (real < ZERO)
        {
            if(imag > ZERO)
            {
                tmp = MPF_PI + tmp;
            }
            else
            {
                tmp = tmp - MPF_PI;
            }
        }
        else if(real > ZERO)
        {
            tmp = tmp;
        }
        return tmp;
    }

    complex_mpf pow2()
    {
        return *this * *this;
    }

    complex_mpf sqrt_compl()
    {
        QByteArray error;
        REAL m("0.0", NUMBITS), a("0.0", NUMBITS);
        m = this->mod();
        a = this->arg()/(ONE + ONE);
        complex_mpf result(_sqrt(m, ZERO, error) * _cos(a, ZERO, error), _sqrt(m, ZERO, error) * _sin(a, ZERO, error));
        return result;
    }

    void show()
    {
        qDebug() << QString::fromStdString(getString(real)) << " + i " << QString::fromStdString(getString(imag));
    }
};

#endif // COMPLEX_MPF_H
