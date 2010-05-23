#include <QtGui>
 
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "fmModelImpl.h"

double left_bound = 0;
double right_bound = 5;
double plot_step = 1.;
//int iter = -1;

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent), populPlot(0), analPopulPlot(0), phasePlot(0), preyCurve(0), predCurve(0), analPreyCurve(0), analPredCurve(0), preyBal(0), predBal(0), phaseCurve(0), eeul(0), stopped(false), initConds(2), rkInitConds(2)
{
  using namespace MathStuff;
	
	setupUi(this);
	
	populPlot = new QwtPlot(this);
	rkPopulPlot = new QwtPlot(this);
	analPopulPlot = new QwtPlot(this);
	phasePlot = new QwtPlot(this);
	rkPhasePlot = new QwtPlot(this);
	populPlot->setMargin(5);
	rkPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	rkPopulPlot->setMargin(5);
	populPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	analPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	analPopulPlot->setMargin(5);
	phasePlot->setMargin(5);
	rkPhasePlot->setMargin(5);
	
  QHBoxLayout *numericLayout_1 = new QHBoxLayout;
	numericLayout_1->addWidget(populPlot);
  numericLayout_1->addWidget(phasePlot);

	QHBoxLayout *numericLayout_2 = new QHBoxLayout;
	numericLayout_2->addWidget(rkPopulPlot);
  numericLayout_2->addWidget(rkPhasePlot);
	
	QHBoxLayout *analytLayout = new QHBoxLayout;
	analytLayout->addWidget(analPopulPlot);
	
	QVBoxLayout *plotsLayout = new QVBoxLayout;
	plotsLayout->addLayout(analytLayout);
	plotsLayout->addLayout(numericLayout_1);
	plotsLayout->addLayout(numericLayout_2);
	plotsLayout->setSizeConstraint(QLayout::SetMaximumSize);
	
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(qpbStart);
	buttonsLayout->addWidget(qpbStop);
	buttonsLayout->addWidget(qpbTest);
	
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
	rkPreyCurve = new QwtPlotCurve();
	rkPredCurve = new QwtPlotCurve();
	analPreyCurve = new QwtPlotCurve();
	analPredCurve = new QwtPlotCurve();
	preyBal = new QwtPlotCurve();
	predBal = new QwtPlotCurve();
	phaseCurve = new QwtPlotCurve();
	rkPhaseCurve = new QwtPlotCurve();

	QPen preyPen(Qt::green, 1, Qt::SolidLine);
	QPen predPen(Qt::red, 1, Qt::SolidLine);
	preyCurve->setPen(preyPen);
	predCurve->setPen(predPen);
	rkPreyCurve->setPen(preyPen);
	rkPredCurve->setPen(predPen);
	analPreyCurve->setPen(preyPen);
	analPredCurve->setPen(predPen);
	preyPen.setStyle(Qt::DashLine);
	predPen.setStyle(Qt::DashLine);
	preyBal->setPen(preyPen);
	predBal->setPen(predPen);

	preyCurve->attach(populPlot);
	predCurve->attach(populPlot);
	rkPreyCurve->attach(rkPopulPlot);
	rkPredCurve->attach(rkPopulPlot);
	analPreyCurve->attach(analPopulPlot);
	analPredCurve->attach(analPopulPlot);
	preyBal->attach(populPlot);
	predBal->attach(populPlot);
	phaseCurve->attach(phasePlot);
	rkPhaseCurve->attach(rkPhasePlot);


	std::vector<Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(f1);
	rightFuncs.push_back(f2);
	
	eeul = new ExplicitEuler(2, rightFuncs);
	rk = new RungeKutta(2, rightFuncs);

	connect(qpbStart, SIGNAL(clicked()), this, SLOT(startModel()));
	connect(qpbStop, SIGNAL(clicked()), this, SLOT(pauseModel()));
	connect(qpbTest, SIGNAL(clicked()), this, SLOT(test()));
	connect(qleStep, SIGNAL(textChanged(const QString &)), this, SLOT(stepChanged(const QString &)));
	connect(qleTime, SIGNAL(textChanged(const QString &)), this, SLOT(timeChanged(const QString &)));
	connect(qleKH, SIGNAL(textChanged(const QString &)), this, SLOT(KHChanged(const QString &)));
	connect(qleKh, SIGNAL(textChanged(const QString &)), this, SLOT(KhChanged(const QString &)));
	connect(qleKf, SIGNAL(textChanged(const QString &)), this, SLOT(KfChanged(const QString &)));
	connect(qleKF, SIGNAL(textChanged(const QString &)), this, SLOT(KFChanged(const QString &)));
	connect(qlePrey, SIGNAL(textChanged(const QString &)), this, SLOT(preyChanged(const QString &)));
	connect(qlePred, SIGNAL(textChanged(const QString &)), this, SLOT(predChanged(const QString &)));
	/*connect(qle, SIGNAL(textChanged(const QString &)), this, SLOT(Changed(const QString &)));*/

	out.open("out.txt");
}

void fmModelImpl::setInitConds()
{
	using namespace MathStuff;
}
void fmModelImpl::preyChanged(const QString &val)
{
	initConds[0] = val.toDouble();
	rkInitConds[0] = val.toDouble();
	analCoeffs.setK1(val.toDouble());
	bPreyChanged = true;
}

void fmModelImpl::predChanged(const QString &val) 
{
	initConds[1] = val.toDouble();
	rkInitConds[1] = val.toDouble();
	analCoeffs.setK2(val.toDouble());
	bPredChanged = true;
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

	//setInitConds();

	// коэффициенты правых частей
	stopped = false;
	bool calc = true;
	
	// начальные значения жертв, хищников
	/*double initPrey = qlePrey->text().toDouble();
	double initPred = qlePred->text().toDouble();
	
	analCoeffs.setK1(initConds[0]);
	analCoeffs.setK2(initConds[1]);*/
	
	/*if (preyChanged && ! predChanged) {
		std::vector<double> initConds(2);
		initConds.push_back(initPrey);
		initConds.push_back(eeul->getPoint(eeul->getCurIter())[1]);
		eeul->initPoint(eeul->getCurIter() + 1, initConds);
		preyChanged = false;
	}
	if (! preyChanged && predChanged) {
		std::vector<double> initConds(2);
		initConds.push_back(eeul->getPoint(eeul->getCurIter())[0]);
		initConds.push_back(initPred);
		eeul->initPoint(eeul->getCurIter() + 1, initConds);
		predChanged = false;
	}
	if (preyChanged && predChanged) {
		std::vector<double> initConds(2);
		initConds.push_back(initPrey);
		initConds.push_back(initPred);
		eeul->initPoint(eeul->getCurIter() + 1, initConds);
		preyChanged = false;
		predChanged = false;
	}*/
	if (bPreyChanged || bPredChanged) {
		eeul->initPoint(eeul->getCurIter() + 1, initConds);
		rk->initPoint(rk->getCurIter() + 1, rkInitConds);
		bPreyChanged = false;
		bPredChanged = false;
		calc = false;
	}
	
	double curTime = 0;
	int ti = 0;
	const std::vector<double> *pPoint, *pRkPoint;
	while ((pPoint = eeul->getNextPoint(calc)) && (pRkPoint = rk->getNextPoint(calc))) {
		curTime = eeul->getCurIter() * eeul->getStep();
		plgn_prey << QPointF(curTime, pPoint->at(0));
		plgn_pred << QPointF(curTime, pPoint->at(1));
		plgn_rk_prey << QPointF(curTime, pRkPoint->at(0));
		plgn_rk_pred << QPointF(curTime, pRkPoint->at(1));
		plgn_anal_prey << QPointF(curTime, analyticalPrey(curTime));
		plgn_anal_pred << QPointF(curTime, analyticalPred(curTime));
		plgn_phase << QPointF(pPoint->at(0), pPoint->at(1));
		plgn_rk_phase << QPointF(pRkPoint->at(0), pRkPoint->at(1));
		preyCurve->setData(plgn_prey);
		predCurve->setData(plgn_pred);
		rkPredCurve->setData(plgn_rk_pred);
		rkPreyCurve->setData(plgn_rk_prey);
		analPreyCurve->setData(plgn_anal_prey);
		analPredCurve->setData(plgn_anal_pred);
		phaseCurve->setData(plgn_phase);
		rkPhaseCurve->setData(plgn_rk_phase);

		if (curTime > right_bound) {
			left_bound += 1;
			right_bound += 1;
      populPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
			rkPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
			analPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
		}

		populPlot->replot();
		rkPopulPlot->replot();
		analPopulPlot->replot();
		phasePlot->replot();
		rkPhasePlot->replot();
		//iter += 1;
		calc = true;
		qApp->processEvents();
		ti+=1;
	}
	out << ti <<"\n";
}

void fmModelImpl::pauseModel()
{
	/*initConds[0] = eeul->getPoint(eeul->getCurIter())[0];
	initConds[0] = eeul->getPoint(eeul->getCurIter())[0];*/
	initConds = eeul->getPoint(eeul->getCurIter());
	rkInitConds = rk->getPoint(rk->getCurIter());
	stopped = true;
	while (stopped)
		qApp->processEvents();
}

fmModelImpl::~fmModelImpl()
{
	const MathStuff::Matrix &m = eeul->getSolution();
	for (int i = 0; i < m.rowCount(); ++i) {
		out << i << ":\t";
		for (int j = 0; j < m.columnCount(); ++j)
			out << m[i][j] << "\t";
		out << "\n";
	}
	
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
