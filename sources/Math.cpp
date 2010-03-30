#include "Math.h"

MathStuff::Matrix::Matrix()
{
}

MathStuff::Matrix::Matrix(int numRows, int numColumns) : m(numRows)
{
	for (Vecs::iterator it = m.begin(); it != m.end(); ++it)
		it->resize(numColumns);
}

MathStuff::Euler::Euler(double initStep, double accuracy, const std::vector<double> &initConds) : dStep(initStep), dAccuracy(accuracy), vecInitConds(initConds)
{
}
