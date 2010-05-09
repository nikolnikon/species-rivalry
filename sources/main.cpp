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
}
