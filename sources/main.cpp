#include <QApplication>
#include "fmModelImpl.h"
#include "Maths.h"

MathStuff::Coeffs preyCoeffs;
MathStuff::Coeffs predCoeffs;
MathStuff::Coeffs analCoeffs;

int main(int argc, char **argv)
{
	/*preyCoeffs.setK1(7);
	preyCoeffs.setK2(2);
	predCoeffs.setK1(2);
	predCoeffs.setK2(9);*/

	QApplication app(argc, argv);

	fmModelImpl formModel;
	formModel.show();

	return app.exec();
}
