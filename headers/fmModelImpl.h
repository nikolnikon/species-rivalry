#ifndef FMMODELIMPL_H
#define FMMODELIMPL_H

#include "ui_fmModel.h"
#include <qwt_plot.h>

 class fmModelImpl : public QMainWindow, private Ui::fmModel
{
public:
  fmModelImpl(QWidget *parent = 0);
  //~fmModelImpl();

private:
  QwtPlot *predPlot;
  QwtPlot *preyPlot;
  QwtPlotCurve *predCurve;
  QwtPlotCurve *predCurve;
};

#endif
