#include "Math.h"

extern MathStuff::Coeffs coeff_1;
extern MathStuff::Coeffs coeff_2;

double MathStuff::f1(const std::vector<double>& Y, double t)
{
	return (coeff_1.k1() - coeff_1.k2() * Y[1]) * Y[0]; 
}

double MathStuff::f2(const std::vector<double>& Y, double t)
{
	return (- coeff_2.k1() + coeff_2.k2() * Y[0]) * Y[1]; 
}

double MathStuff::defaultFunc(const std::vector<double>& Y, double t)
{
	return 0;
}

double MathStuff::test(const std::vector<double>& Y, double t)
{
	return (- 10 * (t - 1) * Y[0]);
}

MathStuff::Matrix::Matrix(int numRows, int numColumns) : m(numRows, std::vector<double>(numColumns, 0.0)), iNumColumns(numColumns)
{
	//for (Vecs::iterator it = m.begin(); it != m.end(); ++it)
	//	it->resize(numColumns);
}

MathStuff::Matrix::Matrix(const std::vector<double> &crFirstRow) : m(1, crFirstRow), iNumColumns(crFirstRow.size())
{
}

void MathStuff::Matrix::addRow()
{
	m.push_back(std::vector<double>(iNumColumns, 0.0));
}

void MathStuff::Matrix::addRow(const std::vector<double> &crRow)
{
	m.push_back(crRow);
}

MathStuff::Euler::Euler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<MathStuff::Euler::RightFunc> &rightFuncs) : iDimension(dimension), dStep(initStep), dAccuracy(accuracy), dEndTime(endTime), vecRightFuncs(rightFuncs), vecTimeNodes(1, 0.0), mSol(initConds)
{
}

MathStuff::ExplicitEuler::ExplicitEuler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<RightFunc> &rightFuncs) : Euler(dimension, initStep, accuracy, endTime, initConds, rightFuncs)
{
}

void MathStuff::ExplicitEuler::solve()
{
	double curTime = vecTimeNodes[0];
	int iter = 0;
	while (curTime <= dEndTime) { // вместо 2 должно стоять конечное время
		iter += 1;
		curTime += dStep;
		vecTimeNodes.push_back(curTime);
		mSol.addRow();
		for (int i = 0; i < mSol.columnCount(); ++i) 
			mSol[iter][i] = mSol[iter - 1][i] + dStep * vecRightFuncs[i](mSol[iter - 1], vecTimeNodes[iter - 1]);
		
	}
}
