#include <QApplication>
#include "Math.h"

int main(int argc, char **argv)
{
	MathStuff::Matrix m(2, 2);

	m[1][1] = 3;
	qDebug("%f", m[1][1]);
}
