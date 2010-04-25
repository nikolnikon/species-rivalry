#include <QtGui>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "fmModelImpl.h"
#include "Maths.h" 

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs; 

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent), predPlot(0), preyPlot(0), phasePlot(0), predCurve(0), preyCurve(0), phaseCurve(0), eeul(0)
{
  using namespace MathStuff;
	
	setupUi(this);
	
	predPlot = new QwtPlot(/*QString("Популяция хищников""Predators"),*/ this);
  preyPlot = new QwtPlot(/*QString("Популяция жертв""Preys"),*/ this);
	predPlot->setTitle("Predator");
	preyPlot->setTitle("Prey");
	predPlot->setGeometry(10, 10, 500, 500);
	preyPlot->setGeometry(520, 10, 500, 500);
	//predPlot->setVisible(true);
	//preyPlot->setVisible(true);
	
	//phasePlot = new QwtPlot(QString("Фазовый портрет"), this);

	predCurve = new QwtPlotCurve();
	preyCurve = new QwtPlotCurve();
	//phaseCurve = new QwtPlotCurve();

	preyCurve->attach(preyPlot);
	predCurve->attach(preyPlot);
	//predCurve->attach(predPlot);
	//phaseCurve->attach(phasePlot);

	std::vector<double> initConds;
	initConds.push_back(2/*::preyCoeffs.k1() / ::preyCoeffs.k2()*/);
	initConds.push_back(3/*::predCoeffs.k1() / ::predCoeffs.k2()*/);
	
	std::vector<Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(f1);
	rightFuncs.push_back(f2);
	
	eeul = new ExplicitEuler(2, 0.0001, 0.000001, 10, initConds, rightFuncs);

	connect(qpbStart, SIGNAL(clicked()), this, SLOT(startModel()));

	QHBoxLayout *plotsLayout = new QHBoxLayout;
  plotsLayout->setSpacing(10);
  plotsLayout->addWidget(preyPlot);
  plotsLayout->addWidget(predPlot);

	QHBoxLayout *buttonsLayout = new QHBoxLayout ;
  buttonsLayout->addWidget(qpbStart);

  QVBoxLayout *widgetLayout = new QVBoxLayout;
  widgetLayout->addLayout(plotsLayout);
  widgetLayout->addLayout(buttonsLayout);

  setLayout(widgetLayout);
  showMaximized();
}

void fmModelImpl::startModel()
{
	using MathStuff::Matrix;
	
	eeul->solve();
	const Matrix &crSol = eeul->getSolution();
	const std::vector<double>& crTimeNodes = eeul->getTimeNodes();
	
	QPolygonF plgn_prey, plgn_pred, plgn_phase;
	for (int i = 0; i < crTimeNodes.size(); ++i) {
		plgn_prey << QPointF(crTimeNodes[i], crSol[i][0]);
		plgn_pred << QPointF(crTimeNodes[i], crSol[i][1]);
		//plgn_phase << QPointF(crSol[i][0], crSol[i][1]);
	}
	
	preyCurve->setData(plgn_prey);
	predCurve->setData(plgn_pred);
	//phaseCurve->setData(plgn_phase);

	preyPlot->replot();
	//predPlot->replot();
	//phasePlot->replot();
}
