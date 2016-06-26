#ifndef LINALG_H
#define LINALG_H

#include <QObject>
#include "settings.h"
#include "eval_mpf.h"

class LinAlg : public QObject
{
    Q_OBJECT
    const bool excessive_checking; // it turn on additional checks

    int permutation_swap(VEC_I &permutation, const int &i, const int &j) const;
    int matrix_swap_rows(VEC_VEC_R &matrix, const int &i, const int &j) const;
    int permutation_init(VEC_I & permutation) const;
    int tests_LU_decomposition(VEC_VEC_R &matrix, VEC_I &permutation, int &size) const;
    int singular(VEC_VEC_R &LU) const;
    int solve_forward_back_substitution(bool upper, bool nonunit, VEC_VEC_R &LU, VEC_R &x);
    int permute_vector(VEC_I &p, VEC_R &x);
    int LU_solve_for_x(VEC_VEC_R &LU_matrix, VEC_I &permutation, VEC_R &x);

public:

    explicit LinAlg(QObject *parent = 0, bool t_excessive_checking = false);
    ~LinAlg();
    int get_size_str() const { return sizeRows; }
    int get_size_col() const { return sizeColumns; }

    int LU_decomposition(VEC_VEC_R &matrix, VEC_I &permutation, int *signum);
    int LU_solve(VEC_VEC_R &LU_matrix, VEC_I &permutation, VEC_R &b, VEC_R &x);

#if USE_MPF_CLASS
    int NewtonMetodForSurfaceAndStraightLine(const QByteArray &initialSurface,  // initial string with equation of surface. ex "x*x + y*y - 1.0"
                    const QByteArray &modifiedSurface, // modified string with equation of surface. ex "(x*x + y*y - 1)/(x - 1.0)"
                    const VEC_R &prevPoint, // any point belonging to a straight line
                    VEC_R &resultPoint,          // here will be result - intersection point
                    VEC_R &directing_vec,
                    REAL &accuracy,
                    int maximum_number_of_attempts = 400);
#endif

signals:

public slots:

};

#endif // LINALG_H
