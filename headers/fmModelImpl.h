#ifndef FMMODELIMPL_H
#define FMMODELIMPL_H

#include "ui_fmModel.h"
#include "Maths.h"
#include <fstream>

extern MathStuff::Coeffs preyCoeffs;
extern MathStuff::Coeffs predCoeffs;
extern MathStuff::Coeffs startPopuls;

class QwtPlot;
class QwtPlotCurve;
class QwtPlotPicker;
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
	void pauseModel();
	void stepChanged(const QString &val) {eeul->setStep(val.toDouble()); rk->setStep(val.toDouble());}
	void timeChanged(const QString &val) {eeul->setEndTime(val.toDouble()); rk->setEndTime(val.toDouble());}
	void preyChanged(const QString &val);
	void predChanged(const QString &val);
	void KHChanged(const QString &val) {preyCoeffs.setK1(val.toDouble());}
	void KhChanged(const QString &val) {preyCoeffs.setK2(val.toDouble());}
	void KfChanged(const QString &val) {predCoeffs.setK1(val.toDouble());}
	void KFChanged(const QString &val) {predCoeffs.setK2(val.toDouble());}
	void restartModel();

private:
	QwtPlot *populPlot;
	QwtPlot *rkPopulPlot;
	QwtPlot *analPopulPlot;
	QwtPlot *phasePlot;
	QwtPlot *rkPhasePlot;

	QwtPlotCurve *preyCurve;
  QwtPlotCurve *predCurve;

	QwtPlotCurve *rkPreyCurve;
  QwtPlotCurve *rkPredCurve;

	QwtPlotCurve *analPreyCurve;
  QwtPlotCurve *analPredCurve;

	QwtPlotCurve *preyBal;
	QwtPlotCurve *predBal;

	QwtPlotCurve *phaseCurve;
	QwtPlotCurve *rkPhaseCurve;

	QwtPlotPicker *populPicker;
	QwtPlotPicker *rkPopulPicker;
	QwtPlotPicker *phasePicker;
	QwtPlotPicker *rkPhasePicker;

	QPolygonF plgn_prey, plgn_pred, plgn_rk_prey, plgn_rk_pred, plgn_anal_prey, plgn_anal_pred, plgn_phase, plgn_rk_phase, plgn_prey_bal, plgn_pred_bal;
	MathStuff::Euler *eeul;
	MathStuff::RungeKutta *rk;
	bool stopped;
	std::vector<double> initConds;
	std::vector<double> rkInitConds;
	bool bPreyChanged;
	bool bPredChanged;
	//std::ofstream out;
};
#endif
