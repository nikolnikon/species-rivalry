#include <QtGui>
#include <fstream> 
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "fmModelImpl.h"
#include "Maths.h" 

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs; 

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent), populPlot(0)/*predPlot(0), preyPlot(0)*/, phasePlot(0), preyCurve(0), predCurve(0), preyBal(0), predBal(0), phaseCurve(0), eeul(0)
{
  using namespace MathStuff;
	
	setupUi(this);
	
	/*predPlot = new QwtPlot(this);
  preyPlot = new QwtPlot(this);
	predPlot->setTitle("Predator");
	preyPlot->setTitle("Prey");
	predPlot->setGeometry(10, 10, 500, 500);
	preyPlot->setGeometry(520, 10, 500, 500);*/
	populPlot = new QwtPlot(this);
	phasePlot = new QwtPlot(this);
	populPlot->setGeometry(10, 10, 500, 500);
	phasePlot->setGeometry(520, 10, 500, 500);
	

	preyCurve = new QwtPlotCurve();
	predCurve = new QwtPlotCurve();
	preyBal = new QwtPlotCurve();
	predBal = new QwtPlotCurve();
	phaseCurve = new QwtPlotCurve();

	QPen preyPen(Qt::green, 1, Qt::SolidLine);
	QPen predPen(Qt::red, 1, Qt::SolidLine);
	preyCurve->setPen(preyPen);
	predCurve->setPen(predPen);
	preyPen.setStyle(Qt::DashLine);
	predPen.setStyle(Qt::DashLine);
	preyBal->setPen(preyPen);
	predBal->setPen(predPen);

	preyCurve->attach(populPlot);
	predCurve->attach(populPlot);
	preyBal->attach(populPlot);
	predBal->attach(populPlot);
	phaseCurve->attach(phasePlot);


	std::vector<double> initConds;
	initConds.push_back(2);
	initConds.push_back(3);
	
	std::vector<Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(f1);
	rightFuncs.push_back(f2);
	
	eeul = new ExplicitEuler(2, 0.001, 0.1, 2, initConds, rightFuncs);

	connect(qpbStart, SIGNAL(clicked()), this, SLOT(startModel()));
	connect(qpbTest, SIGNAL(clicked()), this, SLOT(test()));

	/*QHBoxLayout *plotsLayout = new QHBoxLayout;
  plotsLayout->setSpacing(10);
  plotsLayout->addWidget(preyPlot);
  plotsLayout->addWidget(predPlot);

	QHBoxLayout *buttonsLayout = new QHBoxLayout ;
  buttonsLayout->addWidget(qpbStart);

  QVBoxLayout *widgetLayout = new QVBoxLayout;
  widgetLayout->addLayout(plotsLayout);
  widgetLayout->addLayout(buttonsLayout);

  setLayout(widgetLayout);*/
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
		plgn_phase << QPointF(crSol[i][0], crSol[i][1]);
	}
	preyCurve->setData(plgn_prey);
	predCurve->setData(plgn_pred);
	phaseCurve->setData(plgn_phase);

	plgn_prey.clear();
	plgn_pred.clear();
	for (int i = 0; i < crTimeNodes.size(); ++i) {
		plgn_prey << QPointF(crTimeNodes[i], predCoeffs.k1()/predCoeffs.k2());
		plgn_pred << QPointF(crTimeNodes[i], preyCoeffs.k1()/preyCoeffs.k2());
	}
	preyBal->setData(plgn_prey);
	predBal->setData(plgn_pred);

	populPlot->replot();
	phasePlot->replot();

	/*std::vector<double> initConds;
	initConds.push_back(1.);
	initConds.push_back(4.);
	QwtPlotCurve phaseCurves[3];
	phaseCurves[0].attach(phasePlot);
	phaseCurves[1].attach(phasePlot);
	phaseCurves[2].attach(phasePlot);
	for (int i = 0; i < 3; ++i) {
		eeul->setInitConds(initConds);
		eeul->solve();
		const Matrix crSol = eeul->getSolution();
		const std::vector<double>& crTimeNodes = eeul->getTimeNodes();
		plgn_phase.clear();
		for (int j = 0; j < crTimeNodes.size(); ++j) 
			plgn_phase << QPointF(crSol[j][0], crSol[j][1]);
		phaseCurves[i].setData(plgn_phase);
		phasePlot->replot();
		initConds.clear();
		double td1 = i + 1. * 2. / 1.5;
		double td2 = i + 3. * 2. / 1.5;
		initConds.push_back(i + 1. * 2. / 1.5);
		initConds.push_back(i + 3. * 2. / 1.5);
	}*/
	std::ofstream out;
	out.open("all.txt");
	for (int i = 0; i < crSol.rowCount(); ++i) {
		for (int j = 0; j < crSol.columnCount(); ++j) {
			double td = crSol[i][j];
			out << crSol[i][j] << "  ";
		}
		out << "\n";
	}
	out << "\n" << crSol.rowCount();
}

fmModelImpl::~fmModelImpl()
{
	delete eeul;
	delete preyCurve;
	delete predCurve;
	delete phaseCurve;
	delete populPlot;
	delete phasePlot;
}

void fmModelImpl::test()
{
	std::ofstream out;
	out.open("step.txt");
	while (const std::vector<double> *pPoint = eeul->getNextPoint()) {
		for(int i = 0; i < pPoint->size(); ++i)
			out << pPoint->at(i) << "  ";
		out << "\n";
	}
	const MathStuff::Matrix &m = eeul->getSolution();
	out << "\n" << m.rowCount();
}
