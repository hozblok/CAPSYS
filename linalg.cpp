#include "linalg.h"

LinAlg::LinAlg(bool t_excessive_checking) :
    excessive_checking(t_excessive_checking)
{
}

LinAlg::~LinAlg()
{
}

int LinAlg::permutation_swap(VEC_I &permutation, const int &i, const int &j) const
{
    if (excessive_checking)
    {
        const int size = permutation.size();
        if (size != sizeRows)
        {
            print("ERROR: permutation_swap: size of permutation is wrong!");
            return -1;
        }
    }

    if (i >= sizeRows)
    {
        print("ERROR: permutation_swap: first index is out of range!");
        return -1;
    }
    else if (j >= sizeRows)
    {
        print("ERROR: permutation_swap: second index is out of range!");
        return -1;
    }
    else if (i != j)
    {
        std::swap(permutation[i], permutation[j]);
    }

    return 0;
}

int LinAlg::matrix_swap_rows(VEC_VEC_R &matrix, const int &i, const int &j) const
{
    if (excessive_checking)
    {
        const int size1 = matrix.size();
        const int size2 = matrix[0].size();
        if (size1 != sizeRows || size2 != sizeColumns)
        {
            print("ERROR: matrix_swap_rows: size of martix is wrong!");
            return -1;
        }
    }

    if (i >= sizeRows)
    {
        print("ERROR: matrix_swap_rows: first row index is out of range");
        return -1;
    }
    else if (j >= sizeRows)
    {
        print("ERROR: matrix_swap_rows: second row index is out of range");
        return -1;
    }
    else if (i != j)
    {
        for (int k = 0; k < sizeColumns; k++)
        {
#if USE_MPF_CLASS
            swap(matrix[i][k], matrix[j][k]);
#else
            std::swap(m[i][k], m[j][k]);
#endif
        }
    }
    return 0;
}

int LinAlg::permutation_init(VEC_I & permutation) const
{
    if (excessive_checking)
    {
        const int size = permutation.size();
        if (size != sizeRows)
        {
            print("ERROR: permutation_init: size of p is wrong!");
            return -1;
        }
    }

    /* initialize permutation to identity */
    for (int i = 0; i < sizeRows; ++i)
    {
        permutation[i] = i;
    }

    return 0;
}

int LinAlg::tests_LU_decomposition(VEC_VEC_R &matrix, VEC_I &permutation, size_t &size) const
{
    if (size != sizeRows)
    {
        print("ERROR: LU_decomposition: LU decomposition requires three-dimensional space");
        return -1;
    }
    if (size != matrix[0].size())
    {
        print("ERROR: LU_decomposition: LU decomposition requires square matrix");
        return -1;
    }
    else if (size != permutation.size())
    {
        print("ERROR: LU_decomposition: permutation length must match matrix size");
        return -1;
    }
    return 0;
}

int LinAlg::LU_decomposition(VEC_VEC_R &matrix, VEC_I &permutation, int *signum)
{
    size_t size = matrix.size();
    if (tests_LU_decomposition(matrix, permutation, size) != 0)
    {
        return -1;
    }
    *signum = 1;
    if (permutation_init(permutation) != 0)
    {
        return -1;
    }

    for (size_t j = 0; j + 1 < size; j++)
    {
        /* Find maximum in the j-th column */

#if USE_MPF_CLASS
        REAL tmp_max("0.0", NUMBITS);
        tmp_max = abs(matrix[j][j]);
#else
        REAL max = fabs(A[j][j]);
#endif
        size_t i_pivot = j;

        for (size_t i = j + 1; i < size; i++)
        {
#if USE_MPF_CLASS
            REAL tmp_aij("0.0", NUMBITS);
            tmp_aij = abs(matrix[i][j]);
#else
            REAL aij = fabs(A[i][j]);
#endif
            if (tmp_aij > tmp_max)
            {
                tmp_max = tmp_aij;
                i_pivot = i;
            }
        }

        if (i_pivot != j)
        {
            int flag_test = matrix_swap_rows(matrix, j, i_pivot);
            flag_test += permutation_swap(permutation, j, i_pivot);
            if (flag_test != 0)
            {
                return -1;
            }
            *signum = -(*signum);
        }
#if USE_MPF_CLASS
        REAL tmp_ajj("0.0", NUMBITS);
        tmp_ajj = matrix[j][j];
#else
        REAL ajj = A[j][j];
#endif
        if (tmp_ajj != 0.0)
        {
            for (size_t i = j + 1; i < size; i++)
            {
                matrix[i][j] = matrix[i][j] / tmp_ajj;
                for (size_t k = j + 1; k < size; k++)
                {
                    matrix[i][k] = matrix[i][k] - matrix[i][j] * matrix[j][k];
                }
            }
        }
    }

    return 0;
}

int LinAlg::singular(VEC_VEC_R &LU) const
{
//    int n = LU.size();
    for (int i = 0; i < sizeColumns; ++i)
    {
        if (LU[i][i] == ZERO)
        {
            return 1;
        }
    }

    return 0;
}

int LinAlg::solve_forward_back_substitution(bool upper, bool nonunit, VEC_VEC_R &LU, VEC_R &x)
{
//+    excessive checking:
//    int sizeRows = LU.size();
//    int size_col = LU[0].size();
//    if (sizeRows != size_col)
//    {
//        output("ERROR: solve_forward_back_substitution: matrix must be square");
//        return -1;
//    }
//    else if (size_col != x.size())
//    {
//        output("ERROR: solve_forward_back_substitution: invalid length");
//        return -1;
//    }
//-

    int i_x = 0, j_x = 0;

    if (sizeColumns == 0)
    {
        return 0;
    }

    /* form  x := inv( A )*x */
    if (upper)
    {
        /* backsubstitution */
        i_x = (sizeColumns - 1);
        if (nonunit)
        {
            x[i_x] = x[i_x] / LU[sizeColumns - 1][sizeColumns - 1];
        }
        i_x--;
        for (int i = sizeColumns - 2; i >= 0; --i)
        {
#if USE_MPF_CLASS
            REAL tmp("0.0", NUMBITS);
            tmp = x[i_x];
#else
            REAL tmp = X[ix];
#endif
            j_x = i_x + 1;
            for (int j = i + 1; j < sizeColumns; ++j)
            {
                tmp -= LU[i][j] * x[j_x];
                j_x++;
            }
            if (nonunit)
            {
                x[i_x] = tmp / LU[i][i];
            }
            else
            {
                x[i_x] = tmp;
            }
            i_x--;
        }
    }
    else if (upper == false) //lower
    {
        /* forward substitution */
        i_x = 0;
        if (nonunit)
        {
            x[i_x] = x[i_x] / LU[0][0];
        }
        i_x++;
        for (int i = 1; i < sizeColumns; i++)
        {
#if USE_MPF_CLASS
            REAL tmp("0.0", NUMBITS);
            tmp = x[i_x];
#else
            REAL tmp = X[ix];
#endif
            j_x = 0;
            for (int j = 0; j < i; j++)
            {
                tmp -= LU[i][j] * x[j_x];
                j_x++;
            }
            if (nonunit)
            {
                x[i_x] = tmp / LU[i][i];
            } else
            {
                x[i_x] = tmp;
            }
            i_x++;
        }
    }
    else
    {
        print("ERROR: solve_forward_back_substitution: unrecognized operation!");
        return -1;
    }

    return 0;
}

int LinAlg::permute_vector(VEC_I &p, VEC_R &x)
{
    for (size_t i = 0; i < x.size(); ++i)
    {
        size_t k = p[i];
        while (k > i)
        {
            k = p[k];
        }
        if (k < i)
        {
            continue;
        }

        /* Now have k == i, i.e the least in its cycle */
        size_t pk = p[k];
        if (pk == i)
        {
            continue;
        }

        /* shuffle the elements of the cycle */
        {
#if USE_MPF_CLASS
            REAL tmp("0.0", NUMBITS);
            tmp = x[i];
#else
            REAL tmp = x[i];
#endif
            while (pk != i)
            {
                x[k] = x[pk];
                k = pk;
                pk = p[k];
            }
            x[k] = tmp;
        }
    }

    return 0;
}

int LinAlg::LU_solve_for_x(VEC_VEC_R &LU_matrix, VEC_I &permutation, VEC_R &x)
{
    if (LU_matrix.size() != LU_matrix[0].size())
    {
        print("ERROR: LU matrix must be square");
        return -1;
    }
    else if (LU_matrix.size() != permutation.size())
    {
        print("ERROR: permutation length must match matrix size");
        return -1;
    }
    else if (LU_matrix.size() != x.size())
    {
        print("ERROR: matrix size must match solution/rhs size");
        return -1;
    }
    else if (x.size() != permutation.size())
    {
        print("ERROR: vector and permutation must be the same length");
        return -1;
    }
    else if (singular(LU_matrix))
    {
        print("ERROR: matrix is singular");
        return -1;
    }
    else
    {
        /* Apply permutation to RHS */
        permute_vector(permutation, x);

        /* Solve for c using forward-substitution, L c = P b */
        bool upper = false;
        bool nonunit = false;
        solve_forward_back_substitution(upper, nonunit, LU_matrix, x);

        /* Perform back-substitution, U x = c */
        upper = true;
        nonunit = true;
        solve_forward_back_substitution(upper, nonunit, LU_matrix, x);

        return 0;
    }
}

int LinAlg::LU_solve(VEC_VEC_R &LU_matrix, VEC_I &permutation, VEC_R &b, VEC_R &x)
{
    if (LU_matrix.size() != LU_matrix[0].size())
    {
        print("ERROR: LU_solve: LU matrix must be square");
        return -1;
    }
    else if (LU_matrix.size() != permutation.size())
    {
        print("ERROR: LU_solve: permutation length must match matrix size");
        return -1;
    }
    else if (LU_matrix.size() != b.size())
    {
        print("ERROR: LU_solve: matrix size must match b size");
        return -1;
    }
    else if (LU_matrix[0].size() != x.size())
    {
        print("ERROR: LU_solve: matrix size must match solution size");
        return -1;
    }
    else if (singular(LU_matrix))
    {
        print("ERROR: LU_solve: matrix is singular");
        return -1;
    }
    else
    {
        /* Copy x <- b */
        for (size_t i = 0; i < b.size(); ++i) {
            x[i] = b[i];
        }
        //        gsl_vector_memcpy (x, b);
        /* Solve for x */
        int status = LU_solve_for_x(LU_matrix, permutation, x);

        return status;
    }
}

#if USE_MPF_CLASS
int LinAlg::NewtonMetodForSurfaceAndStraightLine(const QByteArray &initial_surface,  // initial string with equation of surface. ex "x*x + y*y - 1.0"
                        const QByteArray &modified_surface, // modified string with equation of surface. ex "(x*x + y*y - 1)/(x - 1.0)"
                        const VEC_R &prev_point, // any point belonging to a straight line
                        VEC_R &result_point,          // here will be result - intersection point
                        VEC_R &directing_vec, // directing vector
                        REAL &accuracy,
                        int maximum_number_of_attempts)
{
    //+ normalize the direction vector
    REAL tmp("0.0", NUMBITS);
    for (int i = 0; i < sizeColumns; ++i)
    {
        tmp += directing_vec[i] * directing_vec[i];
    }
    tmp = sqrt(tmp);
    for (int i = 0; i < sizeColumns; ++i)
    {
        directing_vec[i] /= tmp;
    }
    //-

    //+ initialization
    // An mpf_class or expression will give the precision of that value. The precision of a binary expression is the higher of the two operands.
    VEC_VEC_R jacobi_matrix(sizeRows); // matrix a: a*x=b
    for (VEC_VEC_R::iterator it_str = jacobi_matrix.begin(); it_str != jacobi_matrix.end(); ++it_str)
    {
        it_str->resize(sizeColumns);
        for (VEC_R::iterator it = it_str->begin(); it != it_str->end(); ++it)
        {
            it->set_prec(NUMBITS);
        }
    }
    VEC_R b_vector_right_part(sizeRows); // vector b: a*x=b
    VEC_R x_vector_solution(sizeColumns); // vector contains solution
    for (VEC_R::iterator it = b_vector_right_part.begin(); it != b_vector_right_part.end(); ++it)
    {
        it->set_prec(NUMBITS);
    }
    for (VEC_R::iterator it = x_vector_solution.begin(); it != x_vector_solution.end(); ++it)
    {
        it->set_prec(NUMBITS);
    }
    VEC_I vector_permutation(sizeRows);
    //-

    Eval eval_modified_surface(modified_surface.data());
    Eval eval_initial_surface(initial_surface.data());

    bool exitCondition = true;
    for (int i = 0; exitCondition; ++i)
    {
        if (i > maximum_number_of_attempts)
        {
            print("ERROR: LU_Solve() in NewtonMetod: Limit is exceeded!");
            return -1;
        }

        for (int j = 0; j < sizeColumns; ++j)
        {
            jacobi_matrix[0][j]= eval_modified_surface.getValueDerivative(namesOfVar[j], result_point[0], result_point[1], result_point[2]);
        }
        jacobi_matrix[1][0]= directing_vec[1];
        jacobi_matrix[1][1]= ZERO - directing_vec[0];
        jacobi_matrix[1][2]= ZERO;
        jacobi_matrix[2][0]= ZERO;
        jacobi_matrix[2][1]= directing_vec[2];
        jacobi_matrix[2][2]= ZERO - directing_vec[1];

        b_vector_right_part[0]= ZERO - eval_modified_surface.getValue(result_point[0], result_point[1], result_point[2]);
        b_vector_right_part[1]= ZERO - directing_vec[1] * (result_point[0] - prev_point[0]) + directing_vec[0] * (result_point[1] - prev_point[1]);
        b_vector_right_part[2]= ZERO - directing_vec[2] * (result_point[1] - prev_point[1]) + directing_vec[1] * (result_point[2] - prev_point[2]);
//        bVector[1]= 0 - mglf1.Calc(rVector[0],rVector[1],rVector[2]); //another variant
//        bVector[2]= 0 - mglf2.Calc(rVector[0],rVector[1],rVector[2]); //another variant

//        qDebug() << "result_point:";
//        qDebug() << result_point[0].get_d() << result_point[1].get_d() << result_point[2].get_d() << eval_modified_surface.CalcD(diffCharXi[0], result_point[0], result_point[1], result_point[2]).get_d() << diffCharXi[0];
//        qDebug() << result_point[0].get_d() << result_point[1].get_d() << result_point[2].get_d() << eval_modified_surface.CalcD(diffCharXi[1], result_point[0], result_point[1], result_point[2]).get_d() << diffCharXi[1];
//        qDebug() << result_point[0].get_d() << result_point[1].get_d() << result_point[2].get_d() << eval_modified_surface.CalcD(diffCharXi[2], result_point[0], result_point[1], result_point[2]).get_d() << diffCharXi[2];
//        qDebug() << "- result_point";
//        qDebug() << "prev_point:";
//        qDebug() << prev_point[0].get_d() << prev_point[1].get_d() << prev_point[2].get_d();
//        qDebug() << "- prev_point";
//        qDebug() << "Matrica:";
//        qDebug() << jacobi_matrix[0][0].get_d() << jacobi_matrix[0][1].get_d() << jacobi_matrix[0][2].get_d();
//        qDebug() << jacobi_matrix[1][0].get_d() << jacobi_matrix[1][1].get_d() << jacobi_matrix[1][2].get_d();
//        qDebug() << jacobi_matrix[2][0].get_d() << jacobi_matrix[2][1].get_d() << jacobi_matrix[2][2].get_d();
//        qDebug() << "- Matrica:";
//        qDebug() << "b:";
//        qDebug() << b_vector_right_part[0].get_d() << b_vector_right_part[1].get_d() << b_vector_right_part[2].get_d();
//        qDebug() << "- b";

        int tsignum = 0;
        if (LU_decomposition(jacobi_matrix, vector_permutation, &tsignum) < 0)
        {
            print("ERROR: LU_decomposition() in NewtonMetod: LU_decomposition was broken");
            return -1;
        }
        if (LU_solve(jacobi_matrix, vector_permutation, b_vector_right_part, x_vector_solution) < 0)
        {
            print("ERROR: LU_Solve() in NewtonMetod: LU_Solve was broken");
            return -1;
        }

        if (x_vector_solution[0] == x_vector_solution[0] &&  x_vector_solution[1] == x_vector_solution[1] &&  x_vector_solution[2] == x_vector_solution[2])
        {
            for (int j = 0; j < sizeColumns; ++j)
            {
                result_point[j] += x_vector_solution[j];
            }
        }
        else
        {
            print("ERROR: LU_Solve() in NewtonMetod: wrong format of resultPoint");
            return -1;
        }

        exitCondition = (abs(eval_modified_surface.getValue(result_point[0],result_point[1],result_point[2])) +
                abs(directing_vec[1] * (result_point[0] - prev_point[0]) - directing_vec[0] * (result_point[1] - prev_point[1])) +
                abs(directing_vec[2] * (result_point[1] - prev_point[1]) - directing_vec[1] * (result_point[2] - prev_point[2]))) > accuracy;

//        qDebug() << "xSolution:";
//        qDebug() << x_vector_solution[0].get_d() << x_vector_solution[1].get_d() << x_vector_solution[2].get_d();
//        qDebug() << "- xSolution";
//        tmp = abs(eval_modified_surface.Calc(result_point[0],result_point[1],result_point[2]));
//        mp_exp_t exp;
//        qDebug() << tmp.get_str(exp).data() << exp << accuracy.get_d() << (tmp > accuracy);
//        qDebug() << "RESULT:" << i << (eval_modified_surface.Calc(result_point[0],result_point[1],result_point[2])).get_d() << result_point[0].get_d() << result_point[1].get_d() << result_point[2].get_d();
    }

    return 0;
}
#endif
