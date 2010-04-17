#include <QApplication>
#include "Math.h"

MathStuff::Coeffs coeff_1;
MathStuff::Coeffs coeff_2;

int main(int argc, char **argv)
{
	coeff_1.setK1(3);
	coeff_1.setK2(2);
	coeff_2.setK1(2);
	coeff_2.setK2(4);

	std::vector<double> initConds;
	initConds.push_back(coeff_2.k1() / coeff_2.k2());
	initConds.push_back(coeff_1.k1() / coeff_1.k2());
	
	std::vector<MathStuff::Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(MathStuff::f1);
	rightFuncs.push_back(MathStuff::f2);
	
	MathStuff::Euler *pe = new MathStuff::ExplicitEuler(2, 0.1, 0.000001, 2, initConds, rightFuncs);
	pe->solve();
	MathStuff::Matrix m(pe->getSolution());
	
	/*MathStuff::Euler *pe = new MathStuff::ExplicitEuler(1, 0.1, 0.000001, std::vector<double>(1, 0.006737), std::vector<MathStuff::Euler::RightFunc>(1, MathStuff::test));
	pe->solve();
	MathStuff::Matrix m(pe->getSolution());*/

	/*MathStuff::Matrix m(2, 2);

	m[1][1] = 3;
	qDebug("%f", m[1][1]);*/
}
