#include <vector>
#include <QDebug>
#define REAL double

int permutation_swap(std::vector< size_t > &p, const size_t &i, const size_t &j)
{
    const size_t size = p.size();

    if (i >= size)
    {
        qDebug() << "ERROR: permutation_swap: first index is out of range!";
        return -1;
    }
    else if (j >= size)
    {
        qDebug() << "ERROR: permutation_swap: second index is out of range!";
        return -1;
    }
    else if (i != j)
    {
        size_t tmp = p[i];
        p[i] = p[j];
        p[j] = tmp;
    }

    return 0;
}

int matrix_swap_rows(std::vector< std::vector< REAL > > &m, const size_t &i, const size_t &j)
{
    const size_t size1 = m.size();
    const size_t size2 = m[0].size();

    if (i >= size1)
    {
        qDebug() << "ERROR: matrix_swap_rows: first row index is out of range";
        return -1;
    }
    else if (j >= size1)
    {
        qDebug() << "ERROR: matrix_swap_rows: second row index is out of range";
        return -1;
    }
    else if (i != j)
    {
        for (size_t k = 0; k < size2; k++)
        {
            REAL tmp = m[i][k] ;
            m[i][k] = m[j][k] ;
            m[j][k] = tmp ;
        }
    }

    return 0;
}

void permutation_init(std::vector< size_t > & p)
{
    const size_t n = p.size();

    /* initialize permutation to identity */
    for (size_t i = 0; i < n; ++i)
    {
        p[i] = i;
    }
}

int LU_decomposition(std::vector< std::vector< REAL > > &A, std::vector< size_t > &p, int *signum)
{
    if (A.size() != A[0].size())
    {
        qDebug() << "ERROR: LU_decomposition: LU decomposition requires square matrix";
        return -1;
    }
    else if (p.size() != A.size())
    {
        qDebug() << "ERROR: LU_decomposition: permutation length must match matrix size";
        return -1;
    }
    else
    {
        const size_t N = A.size();
        *signum = 1;
        permutation_init (p);

        for (size_t j = 0; j < N - 1; j++)
        {
            /* Find maximum in the j-th column */
            REAL ajj, max = fabs(A[j][j]);
            size_t i_pivot = j;

            for (size_t i = j + 1; i < N; i++)
            {
                REAL aij = fabs(A[i][j]);

                if (aij > max)
                {
                    max = aij;
                    i_pivot = i;
                }
            }

            if (i_pivot != j)
            {
                int flag_test = matrix_swap_rows(A, j, i_pivot);
                flag_test += permutation_swap(p, j, i_pivot);
                if (flag_test != 0)
                {
                    return -1;
                }
                *signum = -(*signum);
            }

            ajj = A[j][j];

            if (ajj != 0.0)
            {
                for (size_t i = j + 1; i < N; i++)
                {
                    REAL aij = A[i][j] / ajj;
                    A[i][j] = aij;

                    for (size_t k = j + 1; k < N; k++)
                    {
                        REAL aik = A[i][k];
                        REAL ajk = A[j][k];
                        A[i][k] = aik - aij * ajk;
                    }
                }
            }
        }

        return 0;
    }
}
