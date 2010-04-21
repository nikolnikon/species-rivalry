//#include <QtGui>
#include "fmModelImpl.h"

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent)
{
  predPlot = new QwtPlot(QString("Популяция хищников"), this);
  preyPlot = new QwtPlot(QString("Популяция жертв"), this);
}