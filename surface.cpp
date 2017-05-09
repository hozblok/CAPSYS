#include "surface.h"


Surface::Surface(int tId):
    id(tId),
    dimensionM(-6),
    equation(""),
    modifiedEquation(""),
    name(""),
    description("")
{
    print((QString("#creating Surface, id:") + QString::number(id)).toLocal8Bit().constData(), true);

    // get surface
    QSqlQuery query;
    query.prepare("SELECT equation, "
                  "dimension_m, "
                  "name, "
                  "description "
                  "FROM surface WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next())
    {
        dimensionM      = query.value("dimension_m").toInt();
        equation        = query.value("equation").toByteArray();
        modifiedEquation = query.value("equation").toByteArray();
        name            = query.value("name").toByteArray();
        description     = query.value("description").toByteArray();
    }
    else
    {
        //?? вызвать исключение!
    }
}

Surface::~Surface()
{
    print((QString("#deleting Surface, id:") + QString::number(id)).toLocal8Bit().constData(), true);

}

QByteArray Surface::getEquation() const
{
    return equation;
}

const char *Surface::getEquationConstData() const
{
    return equation.constData();
}

QByteArray Surface::getModifiedEquation() const
{
    return modifiedEquation;
}

void Surface::setModifiedEquation(const QByteArray &value)
{
    modifiedEquation = value;
}

int Surface::NewtonMetodForSurfaceAndStraightLine(PointOnRay *prevPoint, // any point belonging to a straight line
                                                  PointOnRay *resultPoint,    // here will be result - intersection point
                                                  const REAL *accuracy,
                                                  int maximumNumberOfAttempts)
{
    const REAL *dirVec[] = {
        prevPoint->getXDirectingVec(),
        prevPoint->getYDirectingVec(),
        prevPoint->getZDirectingVec()
        };

    const REAL *prevP[] = {
        prevPoint->getX(),
        prevPoint->getY(),
        prevPoint->getZ()
    };

    const REAL *resP[] = {
        resultPoint->getX(),
        resultPoint->getY(),
        resultPoint->getZ()
    };

    //+ initialization
    // An mpf_class or expression will give the precision of that value.
    // The precision of a binary expression is the higher of the two operands.
    VEC_VEC_R jacobiMatrix(sizeRows); // matrix a: a*x=b
    for (VEC_VEC_R::iterator itStr = jacobiMatrix.begin(); itStr != jacobiMatrix.end(); ++itStr)
    {
        itStr->resize(sizeColumns);
        for (VEC_R::iterator it = itStr->begin(); it != itStr->end(); ++it)
        {
            it->set_prec(NUMBITS);
        }
    }
    VEC_R bRightPart(sizeRows); // vector b: a*x=b
    VEC_R xSolution(sizeColumns); // vector contains solution
    for (VEC_R::iterator it = bRightPart.begin(); it != bRightPart.end(); ++it)
    {
        it->set_prec(NUMBITS);
    }
    for (VEC_R::iterator it = xSolution.begin(); it != xSolution.end(); ++it)
    {
        it->set_prec(NUMBITS);
    }
    VEC_I vector_permutation(sizeRows);
    //-

    Eval evalModSurf(modifiedEquation.constData());

    bool exitCondition = true;
    for (int i = 0; exitCondition; ++i)
    {
        if (i > maximumNumberOfAttempts)
        {
            print("ERROR: LU_Solve() in NewtonMetod: Limit is exceeded!");
            return -1;
        }

        for (int j = 0; j < sizeColumns; ++j)
        {
            jacobiMatrix[0][j]= evalModSurf.getValueDerivative(namesOfVar[j], *resP[0], *resP[1], *resP[2]);
        }
        jacobiMatrix[1][0]= *dirVec[1];
        jacobiMatrix[1][1]= ZERO - *dirVec[0];
        jacobiMatrix[1][2]= ZERO;
        jacobiMatrix[2][0]= ZERO;
        jacobiMatrix[2][1]= *dirVec[2];
        jacobiMatrix[2][2]= ZERO - *dirVec[1];

        bRightPart[0]= ZERO - evalModSurf.getValue(*resP[0], *resP[1], *resP[2]);
        bRightPart[1]= ZERO - *dirVec[1] * (*resP[0] - *prevP[0]) + *dirVec[0] * (*resP[1] - *prevP[1]);
        bRightPart[2]= ZERO - *dirVec[2] * (*resP[1] - *prevP[1]) + *dirVec[1] * (*resP[2] - *prevP[2]);

        int tsignum = 0;
        if (linAlg.LU_decomposition(jacobiMatrix, vector_permutation, &tsignum) < 0)
        {
            print("ERROR: LU_decomposition() in NewtonMetod: LU_decomposition was broken");
            return -1;
        }
        if (linAlg.LU_solve(jacobiMatrix, vector_permutation, bRightPart, xSolution) < 0)
        {
            print("ERROR: LU_Solve() in NewtonMetod: LU_Solve was broken");
            return -1;
        }

        if (xSolution[0] == xSolution[0] && xSolution[1] == xSolution[1] && xSolution[2] == xSolution[2])
        {
            resultPoint->setX(*resP[0] + xSolution[0]);
            resultPoint->setY(*resP[1] + xSolution[1]);
            resultPoint->setZ(*resP[2] + xSolution[2]);
        }
        else
        {
            print("ERROR: LU_Solve() in NewtonMetod: wrong format of resultPoint");
            return -1;
        }

        exitCondition = (abs(evalModSurf.getValue(*resP[0], *resP[1], *resP[2])) +
                abs(*dirVec[1] * (*resP[0] - *prevP[0]) - *dirVec[0] * (*resP[1] - *prevP[1])) +
                abs(*dirVec[2] * (*resP[1] - *prevP[1]) - *dirVec[1] * (*resP[2] - *prevP[2]))) > *accuracy;
    }

    return 0;
}
