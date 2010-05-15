#include <QtGui>
#include <fstream> 
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "fmModelImpl.h"
#include "Maths.h" 

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs;
extern MathStuff::Coeffs analCoeffs;
//extern MathStuff::analyticalPrey;
//extern MathStuff::analyticalPred;

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent), populPlot(0), analPopulPlot(0), phasePlot(0), preyCurve(0), predCurve(0), analPreyCurve(0), analPredCurve(0), preyBal(0), predBal(0), phaseCurve(0), eeul(0)
{
  using namespace MathStuff;
	
	setupUi(this);
	
	populPlot = new QwtPlot(this);
	analPopulPlot = new QwtPlot(this);
	phasePlot = new QwtPlot(this);
	/*populPlot->setGeometry(10, 10, 500, 500);
	phasePlot->setGeometry(520, 10, 500, 500);*/
	populPlot->setMargin(5);
	analPopulPlot->setMargin(5);
	phasePlot->setMargin(5);
	
  QHBoxLayout *numericLayout = new QHBoxLayout;
	numericLayout->addWidget(populPlot);
  numericLayout->addWidget(phasePlot);
	
	QHBoxLayout *analytLayout = new QHBoxLayout;
	analytLayout->addWidget(analPopulPlot);
	
	QVBoxLayout *plotsLayout = new QVBoxLayout;
	plotsLayout->addLayout(analytLayout);
	plotsLayout->addLayout(numericLayout);
	plotsLayout->setSizeConstraint(QLayout::SetMaximumSize);
	
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(qpbStart);
	buttonsLayout->addWidget(qpbStop);
	buttonsLayout->addWidget(qpbTest);
	//buttonsLayout->addStretch(4);
	
	QVBoxLayout *paramsLayout = new QVBoxLayout;
	paramsLayout->setSizeConstraint(QLayout::SetFixedSize);
	paramsLayout->setGeometry(QRect(paramsLayout->geometry().x(), paramsLayout->geometry().y(), 159, paramsLayout->geometry().height()));
	paramsLayout->addWidget(qgbModParams);
	paramsLayout->addWidget(qgbInitConds);
	paramsLayout->addWidget(qgbIntParams);
	paramsLayout->addLayout(buttonsLayout);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(paramsLayout);
	mainLayout->addLayout(plotsLayout);
	this->centralWidget()->setLayout(mainLayout);
	this->adjustSize();

	preyCurve = new QwtPlotCurve();
	predCurve = new QwtPlotCurve();
	analPreyCurve = new QwtPlotCurve();
	analPredCurve = new QwtPlotCurve();
	preyBal = new QwtPlotCurve();
	predBal = new QwtPlotCurve();
	phaseCurve = new QwtPlotCurve();

	QPen preyPen(Qt::green, 1, Qt::SolidLine);
	QPen predPen(Qt::red, 1, Qt::SolidLine);
	preyCurve->setPen(preyPen);
	predCurve->setPen(predPen);
	analPreyCurve->setPen(preyPen);
	analPredCurve->setPen(predPen);
	preyPen.setStyle(Qt::DashLine);
	predPen.setStyle(Qt::DashLine);
	preyBal->setPen(preyPen);
	predBal->setPen(predPen);

	preyCurve->attach(populPlot);
	predCurve->attach(populPlot);
	analPreyCurve->attach(analPopulPlot);
	analPredCurve->attach(analPopulPlot);
	preyBal->attach(populPlot);
	predBal->attach(populPlot);
	phaseCurve->attach(phasePlot);


	/*std::vector<double> initConds;
	initConds.push_back(2);
	initConds.push_back(3);
	
	std::vector<Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(f1);
	rightFuncs.push_back(f2);
	
	eeul = new ExplicitEuler(2, 0.01, 0.1, 10, initConds, rightFuncs);*/

	connect(qpbStart, SIGNAL(clicked()), this, SLOT(startModel()));
	connect(qpbTest, SIGNAL(clicked()), this, SLOT(test()));
}

void fmModelImpl::setInitConds()
{
	using namespace MathStuff;
	
	// коэффициенты правых частей
	preyCoeffs.setK1(qleKH->text().toDouble());
	preyCoeffs.setK2(qleKh->text().toDouble());
	predCoeffs.setK1(qleKf->text().toDouble());
	predCoeffs.setK1(qleKF->text().toDouble());
	
	// начальные значения жерт, хищников и их производных
	double initPrey = qlePrey->text().toDouble();
	double initPred = qlePred->text().toDouble();
	double initDerivPrey = qleDerivPrey->text().toDouble();
	double initDerivPred = qleDerivPred->text().toDouble();
	
	std::vector<double> initConds;
	initConds.push_back(initPrey);
	initConds.push_back(initPred);

	double step = qleStep->text().toDouble();
	double time = qleTime->text().toDouble();

	analCoeffs.setK1(initPrey); // fi = 0
	analCoeffs.setK2(initDerivPred / (preyCoeffs.k1() * predCoeffs.k1())); // fi = pi/2
	
	if (! eeul) {
		std::vector<Euler::RightFunc> rightFuncs;
		rightFuncs.push_back(f1);
		rightFuncs.push_back(f2);
		eeul = new ExplicitEuler(2, step, 0.0001, time, initConds, rightFuncs);
	}
	else {
		eeul->reset();
		eeul->setStep(step);
		eeul->setEndTime(time);
		eeul->setInitConds(initConds);

	}
}

void fmModelImpl::startModel()
{
	using namespace MathStuff;
	
	//eeul->solve();
	//const Matrix &crSol = eeul->getSolution();
	//const std::vector<double>& crTimeNodes = eeul->getTimeNodes();
	//
	//QPolygonF plgn_prey, plgn_pred, plgn_phase;
	//for (int i = 0; i < crTimeNodes.size(); ++i) {
	//	plgn_prey << QPointF(crTimeNodes[i], crSol[i][0]);
	//	plgn_pred << QPointF(crTimeNodes[i], crSol[i][1]);
	//	plgn_phase << QPointF(crSol[i][0], crSol[i][1]);
	//}
	//preyCurve->setData(plgn_prey);
	//predCurve->setData(plgn_pred);
	//phaseCurve->setData(plgn_phase);

	//plgn_prey.clear();
	//plgn_pred.clear();
	//for (int i = 0; i < crTimeNodes.size(); ++i) {
	//	plgn_prey << QPointF(crTimeNodes[i], predCoeffs.k1()/predCoeffs.k2());
	//	plgn_pred << QPointF(crTimeNodes[i], preyCoeffs.k1()/preyCoeffs.k2());
	//}
	//preyBal->setData(plgn_prey);
	//predBal->setData(plgn_pred);

	//populPlot->replot();
	//phasePlot->replot();

	///*std::vector<double> initConds;
	//initConds.push_back(1.);
	//initConds.push_back(4.);
	//QwtPlotCurve phaseCurves[3];
	//phaseCurves[0].attach(phasePlot);
	//phaseCurves[1].attach(phasePlot);
	//phaseCurves[2].attach(phasePlot);
	//for (int i = 0; i < 3; ++i) {
	//	eeul->setInitConds(initConds);
	//	eeul->solve();
	//	const Matrix crSol = eeul->getSolution();
	//	const std::vector<double>& crTimeNodes = eeul->getTimeNodes();
	//	plgn_phase.clear();
	//	for (int j = 0; j < crTimeNodes.size(); ++j) 
	//		plgn_phase << QPointF(crSol[j][0], crSol[j][1]);
	//	phaseCurves[i].setData(plgn_phase);
	//	phasePlot->replot();
	//	initConds.clear();
	//	double td1 = i + 1. * 2. / 1.5;
	//	double td2 = i + 3. * 2. / 1.5;
	//	initConds.push_back(i + 1. * 2. / 1.5);
	//	initConds.push_back(i + 3. * 2. / 1.5);
	//}*/
	//std::ofstream out;
	//out.open("all.txt");
	//for (int i = 0; i < crSol.rowCount(); ++i) {
	//	for (int j = 0; j < crSol.columnCount(); ++j) {
	//		double td = crSol[i][j];
	//		out << crSol[i][j] << "  ";
	//	}
	//	out << "\n";
	//}
	//out << "\n" << crSol.rowCount();

	setInitConds();
	
	QPolygonF plgn_prey, plgn_pred, plgn_anal_prey, plgn_anal_pred, plgn_phase;
	int iter = 0;
	double curTime = 0;
	while (const std::vector<double> *pPoint = eeul->getNextPoint()) {
		curTime = iter * eeul->getStep();
		plgn_prey << QPointF(curTime, pPoint->at(0));
		plgn_pred << QPointF(curTime, pPoint->at(1));
		plgn_anal_prey << QPointF(curTime, analyticalPrey(curTime));
		plgn_anal_pred << QPointF(curTime, analyticalPred(curTime));
		preyCurve->setData(plgn_prey);
		predCurve->setData(plgn_pred);
		analPreyCurve->setData(plgn_anal_prey);
		analPredCurve->setData(plgn_anal_pred);
		populPlot->replot();
		analPopulPlot->replot();
		iter += 1;
		qApp->processEvents();
	}
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
	qDebug("STOP");
	/*std::ofstream out;
	out.open("step.txt");
	while (const std::vector<double> *pPoint = eeul->getNextPoint()) {
		for(int i = 0; i < pPoint->size(); ++i)
			out << pPoint->at(i) << "  ";
		out << "\n";
	}
	const MathStuff::Matrix &m = eeul->getSolution();
	out << "\n" << m.rowCount();*/
}
