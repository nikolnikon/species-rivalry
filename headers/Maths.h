#ifndef MATHS_H
#define MATHS_H

#include <vector>
#include <functional>

namespace MathStuff 
{
	/*!Функции вычисления правых частей ОДУ*/
	double f1(const std::vector<double>& crY, double t = 0);
	double f2(const std::vector<double>& crY, double t = 0);
	double defaultFunc(const std::vector<double>& Y, double t = 0);
	double test(const std::vector<double>& Y, double t);
	
	class Matrix
	{
	public:
		typedef std::vector<std::vector<double>> Vecs;

		Matrix() {}
		Matrix(int numRows, int numColumns);
		Matrix(const std::vector<double> &crFirstRow);
		
		int rowCount() const { return m.size(); }
		int columnCount() const { return iNumColumns; }
		std::vector<double>& operator[](int ind) { return m[ind]; }
		void addRow();
		void addRow(const std::vector<double> &crRow);
	private:
		Vecs m;
		int iNumColumns;
	};

	class Euler
	{
	public:
		typedef std::tr1::function<double (const std::vector<double>& , double )> RightFunc;

		Euler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<RightFunc> &rightFuncs);
		virtual ~Euler() {};
		
		void setStep(double step) { dStep = step; }
		void setAccuracy(double accur) { dAccuracy = accur; }
		// не уверен, что копирование вектора - хороший ход
		void setInitConds(const std::vector<double> &crInitConds) { /*vecInitConds = crInitConds;*/ } 
		void setRightFuncs(const std::vector<RightFunc> &crRightFuncs) { vecRightFuncs = crRightFuncs; }
		virtual void solve() = 0;
		const Matrix& getSolution() const { return mSol; }
	protected:
		/*!Количество уравнений в системе*/
		int iDimension;
		double dStep;
		double dAccuracy;
		std::vector<RightFunc> vecRightFuncs;
		std::vector<double> vecTimeNodes;
		double dEndTime;
		Matrix mSol;
	};

	class ExplicitEuler : public Euler
	{
	public:
		ExplicitEuler(int dimension, double initStep, double accuracy, double endTime, const std::vector<double> &initConds, const std::vector<RightFunc> &rightFuncs);
		void solve();
	};

	/*!Класс, реализующий функциональный объект, который представляет функцию правой части*/
	/*class RightFunc
	{
	public:
		RightFunc(double k1, double k2) : dK1(k1), dK2(k2) {}
		double operator()(const std::vector<double> &crY, double t = 0) const;
	private:
		double dK1;
		double dK2;
	};*/

	struct Coeffs
	{
	public:
		//Coeffs(double k1, double k2) : dK1(k1), dK2(k2) 
		void setK1(double val) { dK1 = val; } 
		void setK2(double val) { dK2 = val; } 
		double k1() const { return dK1; }
		double k2() const { return dK2; }
	private:
		double dK1;
		double dK2;
	};
}
#endif
