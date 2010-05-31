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

	//��������� ������ ��� �������� ������������ ��������� � Unicode � ������� QObject::tr()
  //������������ ������������� ��� �������������� ���� ������� � Unicode
  QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
  
  //��������� ������ ��� �������� const char* � QCString � Unicode � �������
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

	fmModelImpl formModel;
	formModel.show();

	return app.exec();
}
