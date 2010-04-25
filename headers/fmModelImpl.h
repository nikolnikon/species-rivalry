#ifndef FMMODELIMPL_H
#define FMMODELIMPL_H

#include "ui_fmModel.h"

class QwtPlot;
class QwtPlotCurve;
namespace MathStuff {
	class Euler;
}
class fmModelImpl : public QMainWindow, private Ui::fmModel
{
	Q_OBJECT
public:
  fmModelImpl(QWidget *parent = 0);
  //~fmModelImpl();

private slots:
	void startModel();

private:
  QwtPlot *predPlot;
  QwtPlot *preyPlot;
	QwtPlot *phasePlot;
  QwtPlotCurve *predCurve;
  QwtPlotCurve *preyCurve;
	QwtPlotCurve *phaseCurve;

	MathStuff::Euler *eeul;
};
#endif
