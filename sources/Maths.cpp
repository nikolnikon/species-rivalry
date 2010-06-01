#include "Maths.h"
#include <math.h>
#include <QtGui>

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs;
extern MathStuff::Coeffs startPopuls;

double MathStuff::f1(const std::vector<double>& Y, double t)
{
	return (preyCoeffs.k1() - preyCoeffs.k2() * Y[1]) * Y[0]; 
}

double MathStuff::f2(const std::vector<double>& Y, double t)
{
	return (-predCoeffs.k1() + predCoeffs.k2() * Y[0]) * Y[1]; 
}

double MathStuff::analyticalPrey(double t)
{
	return ((startPopuls.k1() - predCoeffs.k1() / predCoeffs.k2()) * ::cos(preyCoeffs.k1() * predCoeffs.k1() * t - 0.) + predCoeffs.k1() / predCoeffs.k2());
}

double MathStuff::analyticalPred(double t)
{
	return ((startPopuls.k2() - preyCoeffs.k1() / preyCoeffs.k2()) * ::cos(preyCoeffs.k1() * predCoeffs.k1() * t - 3.14 * 0.5) + preyCoeffs.k1() / preyCoeffs.k2());
}

MathStuff::Matrix::Matrix(int numColumns) : m(0, std::vector<double>(numColumns, 0.0)), iNumColumns(numColumns)
{
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

MathStuff::Euler::Euler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<MathStuff::Euler::RightFunc> &rightFuncs) : iDimension(dimension), dStep(initStep), dAccuracy(accuracy), dEndTime(endTime), vecRightFuncs(rightFuncs), vecTimeNodes(1, 0.0), mSol(initConds), iIter(-1)
{
}

void MathStuff::Euler::setInitConds(const std::vector<double> &crInitConds)
{
	mSol.clear();
	mSol.addRow(crInitConds);
	vecTimeNodes.clear();
	vecTimeNodes.push_back(0.);
}

void MathStuff::Euler::initPoint(int iter, const std::vector<double> &crPoint)
{
	mSol.addRow(crPoint);
	vecTimeNodes.push_back(iter * dStep);
}

void MathStuff::Euler::reset()
{
	mSol.clear();
	vecTimeNodes.clear();
	iIter = -1;
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

const std::vector<double>* MathStuff::ExplicitEuler::getNextPoint(bool calc)
{
	iIter += 1;
	if (! calc)
		return &(mSol[iIter]);
	double curTime = iIter * dStep;
	if (curTime > dEndTime) {
		//iIter = 0;
		return 0;
	}
	vecTimeNodes.push_back(curTime);
	mSol.addRow();
	for (int i = 0; i < mSol.columnCount(); ++i)
		mSol[iIter][i] = mSol[iIter - 1][i] + dStep * vecRightFuncs[i](mSol[iIter - 1], vecTimeNodes[iIter - 1]);
	return &(mSol[iIter]);
}

MathStuff::RungeKutta::RungeKutta(int dimension, const std::vector<RightFunc> &rightFuncs) : Euler(dimension, rightFuncs), vecK1(2, 0.0), vecK2(2, 0.0), vecK3(2, 0.0), vecK4(2, 0.0)
{
}

MathStuff::RungeKutta::RungeKutta(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<RightFunc> &rightFuncs) : Euler(dimension, initStep, accuracy, endTime, initConds, rightFuncs), vecK1(2, 0.0), vecK2(2, 0.0), vecK3(2, 0.0), vecK4(2, 0.0)
{
}

void MathStuff::RungeKutta::calcK1()
{
	for (int i = 0; i < iDimension; ++i)
		vecK1[i] = dStep * vecRightFuncs[i](mSol[iIter - 1], vecTimeNodes[iIter - 1]);
}

void MathStuff::RungeKutta::calcK2()
{
	std::vector<double> tmp(iDimension, 0.0);
	for (int i = 0; i < iDimension; ++i)
		tmp[i] = mSol[iIter - 1][i] + vecK1[i] / 2.;
	for (int i = 0; i < iDimension; ++i)
		vecK2[i] = dStep * vecRightFuncs[i](tmp, vecTimeNodes[iIter - 1] + dStep / 2.);
}

void MathStuff::RungeKutta::calcK3()
{
	std::vector<double> tmp(iDimension, 0.0);
	for (int i = 0; i < iDimension; ++i)
		tmp[i] = mSol[iIter - 1][i] + vecK2[i] / 2.;
	for (int i = 0; i < iDimension; ++i)
		vecK3[i] = dStep * vecRightFuncs[i](tmp, vecTimeNodes[iIter - 1] + dStep / 2.);
}

void MathStuff::RungeKutta::calcK4()
{
	std::vector<double> tmp(iDimension, 0.0);
	for (int i = 0; i < iDimension; ++i)
		tmp[i] = mSol[iIter - 1][i] + vecK3[i];
	for (int i = 0; i < iDimension; ++i)
		vecK4[i] = dStep * vecRightFuncs[i](tmp, vecTimeNodes[iIter - 1] + dStep);
}

const std::vector<double>* MathStuff::RungeKutta::getNextPoint(bool calc)
{
	iIter += 1;
	if (! calc)
		return &(mSol[iIter]);
	double curTime = iIter * dStep;
	if (curTime > dEndTime)
		return 0;
	vecTimeNodes.push_back(curTime);
	mSol.addRow();
	calcK1();
	calcK2();
	calcK3();
	calcK4();
	for (int i = 0; i < mSol.columnCount(); ++i)
		mSol[iIter][i] = mSol[iIter - 1][i] + (1. / 6.) * (vecK1[i] + 2. * vecK2[i]+ 2. * vecK3[i] + vecK4[i]);
	return &(mSol[iIter]);
}
