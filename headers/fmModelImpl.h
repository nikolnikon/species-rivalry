#ifndef FMMODELIMPL_H
#define FMMODELIMPL_H

#include "ui_fmModel.h"

class QwtPlot;
class QwtPlotCurve;
class QHBoxLayout;
namespace MathStuff {
	class Euler;
}
class fmModelImpl : public QMainWindow, private Ui::fmModel
{
	Q_OBJECT
public:
  fmModelImpl(QWidget *parent = 0);
  ~fmModelImpl();

private slots:
	void startModel();
	void test();

private:
	QwtPlot *populPlot;
	QwtPlot *phasePlot;
	QwtPlotCurve *preyCurve;
  QwtPlotCurve *predCurve;
	QwtPlotCurve *preyBal;
	QwtPlotCurve *predBal;
	QwtPlotCurve *phaseCurve;
	MathStuff::Euler *eeul;
};
#endif
