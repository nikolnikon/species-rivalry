#include <QApplication>
#include <QTextCodec>
#include "fmModelImpl.h"
#include "Maths.h"

MathStuff::Coeffs preyCoeffs;
MathStuff::Coeffs predCoeffs;
MathStuff::Coeffs startPopuls;

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	//Установка кодека для перевода однобайтовой кодировки в Unicode в функции QObject::tr()
  //Используется исключительно для преобразования кода проекта в Unicode
  QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
  
  //Установка кодека для перевода const char* и QCString в Unicode и обратно
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

	fmModelImpl formModel;
	formModel.show();

	return app.exec();
}
