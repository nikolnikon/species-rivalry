//#include <QtGui>
#include "fmModelImpl.h"

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent)
{
  predPlot = new QwtPlot(QString("��������� ��������"), this);
  preyPlot = new QwtPlot(QString("��������� �����"), this);
}