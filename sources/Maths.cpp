#include "Maths.h"

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs;

double MathStuff::f1(const std::vector<double>& Y, double t)
{
	return (preyCoeffs.k1() - preyCoeffs.k2() * Y[1]) * Y[0]; 
}

double MathStuff::f2(const std::vector<double>& Y, double t)
{
	return (- predCoeffs.k1() + predCoeffs.k2() * Y[0]) * Y[1]; 
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

void MathStuff::Matrix::clear()
{
	for (int i = 0; i < m.size(); ++i)
		m[i].clear();
	m.clear();
}

MathStuff::Euler::Euler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<MathStuff::Euler::RightFunc> &rightFuncs) : iDimension(dimension), dStep(initStep), dAccuracy(accuracy), dEndTime(endTime), vecRightFuncs(rightFuncs), vecTimeNodes(1, 0.0), mSol(initConds)
{
}

void MathStuff::Euler::setInitConds(const std::vector<double> &crInitConds)
{
	mSol.clear();
	mSol.addRow(crInitConds);
	vecTimeNodes.clear();
	vecTimeNodes.push_back(0.);
}

MathStuff::ExplicitEuler::ExplicitEuler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<RightFunc> &rightFuncs) : Euler(dimension, initStep, accuracy, endTime, initConds, rightFuncs)
{
}

void MathStuff::ExplicitEuler::solve()
{
	int iter = 1;
	double curTime = dStep;
	while (curTime <= dEndTime) { // вместо 2 должно стоять конечное время
		vecTimeNodes.push_back(curTime);
		mSol.addRow();
		for (int i = 0; i < mSol.columnCount(); ++i) 
			mSol[iter][i] = mSol[iter - 1][i] + dStep * vecRightFuncs[i](mSol[iter - 1], vecTimeNodes[iter - 1]);
		curTime += dStep;
		iter += 1;
	}
}

const std::vector<double>* MathStuff::ExplicitEuler::getNextPoint()
{
	static int iter = 0;
	//++iter;
	if (iter == 0)
		return &(mSol[iter++]);
	double curTime = 0; 
	curTime += iter * dStep;
	if (curTime > dEndTime)
		return 0;
	vecTimeNodes.push_back(curTime);
	mSol.addRow();
	for (int i = 0; i < mSol.columnCount(); ++i)
		mSol[iter][i] = mSol[iter - 1][i] + dStep * vecRightFuncs[i](mSol[iter - 1], vecTimeNodes[iter - 1]);
	return &(mSol[iter++]);
}
