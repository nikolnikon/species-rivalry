#include <QtGui>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include "fmModelImpl.h"

double left_bound = 0;
double right_bound = 5;
double plot_step = 1.;

fmModelImpl::fmModelImpl(QWidget *parent) : QMainWindow(parent), populPlot(0),rkPopulPlot(0), analPopulPlot(0), phasePlot(0), rkPhasePlot(0), preyCurve(0), predCurve(0), rkPreyCurve(0), rkPredCurve(0), analPreyCurve(0), analPredCurve(0), preyBal(0), predBal(0), phaseCurve(0),rkPhaseCurve(0), populPicker(0), rkPopulPicker(0), phasePicker(0), rkPhasePicker(0), eeul(0), rk(0), stopped(false), initConds(2), rkInitConds(2)
{
  using namespace MathStuff;
	
	setupUi(this);
	
	populPlot = new QwtPlot(this);
	rkPopulPlot = new QwtPlot(this);
	analPopulPlot = new QwtPlot(this);
	phasePlot = new QwtPlot(this);
	rkPhasePlot = new QwtPlot(this);
	
	populPlot->setTitle("Явный метод Эйлера");
	populPlot->setAxisTitle(QwtPlot::xBottom, "t");
	populPlot->setAxisTitle(QwtPlot::yLeft, "H(t), F(t)");
	populPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	populPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	populPlot->setMargin(5);
	

	rkPopulPlot->setTitle("Метод Рунге-Кутты 4-го порядка");
	rkPopulPlot->setAxisTitle(QwtPlot::xBottom, "t");
	rkPopulPlot->setAxisTitle(QwtPlot::yLeft, "H(t), F(t)");
	rkPopulPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	rkPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	rkPopulPlot->setMargin(5);

	analPopulPlot->setTitle("Аналитическое решение");
	analPopulPlot->setAxisTitle(QwtPlot::xBottom, "t");
	analPopulPlot->setAxisTitle(QwtPlot::yLeft, "H(t), F(t)");
	analPopulPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	analPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	analPopulPlot->setMargin(5);
	analPopulPlot->setVisible(false);

	phasePlot->setTitle("Явный метод Эйлера");
	phasePlot->setAxisTitle(QwtPlot::xBottom, "H");
	phasePlot->setAxisTitle(QwtPlot::yLeft, "F(H)");
	phasePlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	phasePlot->setMargin(5);

	rkPhasePlot->setTitle("Метод Рунге-Кутты 4-го порядка");
	rkPhasePlot->setAxisTitle(QwtPlot::xBottom, "H");
	rkPhasePlot->setAxisTitle(QwtPlot::yLeft, "F(H)");
	rkPhasePlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	rkPhasePlot->setMargin(5);
	
  QHBoxLayout *numericLayout_1 = new QHBoxLayout;
	numericLayout_1->addWidget(populPlot);
  numericLayout_1->addWidget(phasePlot);

	QHBoxLayout *numericLayout_2 = new QHBoxLayout;
	numericLayout_2->addWidget(rkPopulPlot);
  numericLayout_2->addWidget(rkPhasePlot);
	
	//QHBoxLayout *analytLayout = new QHBoxLayout;
	//analytLayout->addWidget(analPopulPlot);
	
	QVBoxLayout *plotsLayout = new QVBoxLayout;
	//plotsLayout->addLayout(analytLayout);
	plotsLayout->addLayout(numericLayout_1);
	plotsLayout->addLayout(numericLayout_2);
	plotsLayout->setSizeConstraint(QLayout::SetMaximumSize);
	
	QVBoxLayout *buttonsLayout = new QVBoxLayout;
	buttonsLayout->addWidget(qpbStart);
	buttonsLayout->addWidget(qpbStop);
	buttonsLayout->addWidget(qpbRestart);
	
	QVBoxLayout *paramsLayout = new QVBoxLayout;
	paramsLayout->setSizeConstraint(QLayout::SetFixedSize);
	paramsLayout->setGeometry(QRect(paramsLayout->geometry().x(), paramsLayout->geometry().y(), 129, 500/*paramsLayout->geometry().height()*/));
	paramsLayout->addWidget(qgbModParams);
	paramsLayout->addWidget(qgbInitConds);
	paramsLayout->addWidget(qgbIntParams);
	paramsLayout->addLayout(buttonsLayout);
	//paramsLayout->addWidget(qglButtons);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(paramsLayout);
	mainLayout->addLayout(plotsLayout);
	this->centralWidget()->setLayout(mainLayout);
	showMaximized();

	// Кривые
	preyCurve = new QwtPlotCurve("H(t)");
	predCurve = new QwtPlotCurve("F(t)");
	rkPreyCurve = new QwtPlotCurve("H(t)");
	rkPredCurve = new QwtPlotCurve("F(t)");
	analPreyCurve = new QwtPlotCurve("H(t)");
	analPredCurve = new QwtPlotCurve("F(t)");

	preyBal = new QwtPlotCurve();
	predBal = new QwtPlotCurve();
	phaseCurve = new QwtPlotCurve("F(H)");
	rkPhaseCurve = new QwtPlotCurve("F(H)");

	QPen preyPen(Qt::green, 2, Qt::SolidLine);
	QPen predPen(Qt::red, 2, Qt::SolidLine);
	preyCurve->setPen(preyPen);
	predCurve->setPen(predPen);
	rkPreyCurve->setPen(preyPen);
	rkPredCurve->setPen(predPen);
	analPreyCurve->setPen(preyPen);
	analPredCurve->setPen(predPen);
	preyPen.setStyle(Qt::DashLine);
	predPen.setStyle(Qt::DashLine);
	preyPen.setWidth(1);
	predPen.setWidth(1);
	preyBal->setPen(preyPen);
	predBal->setPen(predPen);
	QPen phasePen(Qt::blue, 2, Qt::SolidLine);
	phaseCurve->setPen(phasePen);
	rkPhaseCurve->setPen(phasePen);

	preyCurve->attach(populPlot);
	predCurve->attach(populPlot);
	rkPreyCurve->attach(rkPopulPlot);
	rkPredCurve->attach(rkPopulPlot);
	analPreyCurve->attach(analPopulPlot);
	analPredCurve->attach(analPopulPlot);
	preyBal->attach(populPlot);
	predBal->attach(populPlot);
	preyBal->attach(rkPopulPlot);
	predBal->attach(rkPopulPlot);
	phaseCurve->attach(phasePlot);
	rkPhaseCurve->attach(rkPhasePlot);

	populPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, populPlot->canvas());
  populPicker->setRubberBandPen(QColor(Qt::blue));
  //populPicker->setRubberBand(QwtPicker::CrossRubberBand);
  populPicker->setTrackerPen(QColor(Qt::black));
	
	rkPopulPicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, rkPopulPlot->canvas());
  rkPopulPicker->setRubberBandPen(QColor(Qt::blue));
  //rkPopulPicker->setRubberBand(QwtPicker::CrossRubberBand);
  rkPopulPicker->setTrackerPen(QColor(Qt::black));

	phasePicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, phasePlot->canvas());
  phasePicker->setRubberBandPen(QColor(Qt::red));
  //phasePicker->setRubberBand(QwtPicker::CrossRubberBand);
  phasePicker->setTrackerPen(QColor(Qt::black));

	rkPhasePicker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, rkPhasePlot->canvas());
  rkPhasePicker->setRubberBandPen(QColor(Qt::red));
  //rkPhasePicker->setRubberBand(QwtPicker::CrossRubberBand);
  rkPhasePicker->setTrackerPen(QColor(Qt::black));

	std::vector<Euler::RightFunc> rightFuncs;
	rightFuncs.push_back(f1);
	rightFuncs.push_back(f2);
	
	eeul = new ExplicitEuler(2, rightFuncs);
	rk = new RungeKutta(2, rightFuncs);

	connect(qpbStart, SIGNAL(clicked()), this, SLOT(startModel()));
	connect(qpbStop, SIGNAL(clicked()), this, SLOT(pauseModel()));
	connect(qpbRestart, SIGNAL(clicked()), this, SLOT(restartModel()));
	connect(qleStep, SIGNAL(textChanged(const QString &)), this, SLOT(stepChanged(const QString &)));
	connect(qleTime, SIGNAL(textChanged(const QString &)), this, SLOT(timeChanged(const QString &)));
	connect(qleKH, SIGNAL(textChanged(const QString &)), this, SLOT(KHChanged(const QString &)));
	connect(qleKh, SIGNAL(textChanged(const QString &)), this, SLOT(KhChanged(const QString &)));
	connect(qleKf, SIGNAL(textChanged(const QString &)), this, SLOT(KfChanged(const QString &)));
	connect(qleKF, SIGNAL(textChanged(const QString &)), this, SLOT(KFChanged(const QString &)));
	connect(qlePrey, SIGNAL(textChanged(const QString &)), this, SLOT(preyChanged(const QString &)));
	connect(qlePred, SIGNAL(textChanged(const QString &)), this, SLOT(predChanged(const QString &)));

	KHChanged(qleKH->text());
	KhChanged(qleKh->text());
	KfChanged(qleKf->text());
	KFChanged(qleKF->text());
	preyChanged(qlePrey->text());
	predChanged(qlePred->text());
	stepChanged(qleStep->text());
	timeChanged(qleTime->text());
}

void fmModelImpl::preyChanged(const QString &val)
{
	initConds[0] = val.toDouble();
	rkInitConds[0] = val.toDouble();
	startPopuls.setK1(val.toDouble());
	bPreyChanged = true;
}

void fmModelImpl::predChanged(const QString &val) 
{
	initConds[1] = val.toDouble();
	rkInitConds[1] = val.toDouble();
	startPopuls.setK2(val.toDouble());
	bPredChanged = true;
}

void fmModelImpl::startModel()
{
	using namespace MathStuff;
	
	// коэффициенты правых частей
	stopped = false;
	bool calc = true;
	
	if (bPreyChanged || bPredChanged) {
		eeul->initPoint(eeul->getCurIter() + 1, initConds);
		rk->initPoint(rk->getCurIter() + 1, rkInitConds);
		bPreyChanged = false;
		bPredChanged = false;
		calc = false;
	}
	
	double curTime = 0;
	//int ti = 0;
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
		plgn_prey_bal << QPointF(curTime, predCoeffs.k1() / predCoeffs.k2());
		plgn_pred_bal << QPointF(curTime, preyCoeffs.k1() / preyCoeffs.k2());
		
		preyCurve->setData(plgn_prey);
		predCurve->setData(plgn_pred);
		rkPredCurve->setData(plgn_rk_pred);
		rkPreyCurve->setData(plgn_rk_prey);
		analPreyCurve->setData(plgn_anal_prey);
		analPredCurve->setData(plgn_anal_pred);
		phaseCurve->setData(plgn_phase);
		rkPhaseCurve->setData(plgn_rk_phase);
		preyBal->setData(plgn_prey_bal);
		predBal->setData(plgn_pred_bal);

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
		calc = true;
		qApp->processEvents();
		//ti+=1;
	}
	//out << ti <<"\n";
}

void fmModelImpl::restartModel()
{
	eeul->reset();
	rk->reset();

	plgn_prey.clear();
	plgn_pred.clear();
	plgn_rk_prey.clear();
	plgn_rk_pred.clear();
	plgn_anal_prey.clear();
	plgn_anal_pred.clear();
	plgn_phase.clear();
	plgn_rk_phase.clear();
	plgn_prey_bal.clear();
	plgn_pred_bal.clear();
	
	KHChanged(qleKH->text());
	KhChanged(qleKh->text());
	KfChanged(qleKf->text());
	KFChanged(qleKF->text());
	preyChanged(qlePrey->text());
	predChanged(qlePred->text());
	stepChanged(qleStep->text());
	timeChanged(qleTime->text());

	left_bound = 0;
  right_bound = 5;
	populPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);
	rkPopulPlot->setAxisScale(QwtPlot::xBottom, left_bound, right_bound, plot_step);

	startModel();
}

void fmModelImpl::pauseModel()
{
	initConds = eeul->getPoint(eeul->getCurIter());
	rkInitConds = rk->getPoint(rk->getCurIter());
	stopped = true;
	while (stopped)
		qApp->processEvents();
}

fmModelImpl::~fmModelImpl()
{
	/*const MathStuff::Matrix &m = eeul->getSolution();
	for (int i = 0; i < m.rowCount(); ++i) {
		out << i << ":\t";
		for (int j = 0; j < m.columnCount(); ++j)
			out << m[i][j] << "\t";
		out << "\n";
	}*/
	
	delete eeul;
	delete rk;
	delete preyCurve;
	delete predCurve;
	delete rkPreyCurve;
	delete rkPredCurve;
	delete analPreyCurve;
	delete analPredCurve;
	delete phaseCurve;
	delete rkPhaseCurve;
	delete populPlot;
	delete rkPopulPlot;
	delete analPopulPlot;
	delete phasePlot;
	delete rkPhasePlot;
	delete preyBal;
	delete predBal;
	delete populPicker;
	delete rkPopulPicker;
	delete phasePicker;
	delete rkPhasePicker;

	stopped = false;
	qApp->processEvents();
}
