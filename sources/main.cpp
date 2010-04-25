#include <QApplication>
#include "fmModelImpl.h"
#include "Maths.h"

MathStuff::Coeffs preyCoeffs;
MathStuff::Coeffs predCoeffs;

int main(int argc, char **argv)
{
	preyCoeffs.setK1(3);
	preyCoeffs.setK2(2);
	predCoeffs.setK1(2);
	predCoeffs.setK2(4);

	QApplication app(argc, argv);

	fmModelImpl formModel;
	formModel.show();

	return app.exec();

	/*std::vector<double> initConds;
	initConds.push_back(predCoeffs.k1() / predCoeffs.k2());
	initConds.push_back(preyCoeffs.k1() / preyCoeffs.k2());
	
	std::vector<MathStuff::Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(MathStuff::f1);
	rightFuncs.push_back(MathStuff::f2);
	
	MathStuff::Euler *pe = new MathStuff::ExplicitEuler(2, 0.1, 0.000001, 2, initConds, rightFuncs);
	pe->solve();
	MathStuff::Matrix m(pe->getSolution());*/
}
