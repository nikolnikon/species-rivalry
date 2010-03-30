#include <vector>

namespace MathStuff 
{
	class Matrix
	{
	public:
		typedef std::vector<std::vector<double>> Vecs;
		Matrix();
		Matrix(int numRows, int numColumns);
		int rowCount();
		int columnCount();
		std::vector<double>& operator[](int ind) {
			return m[ind];
		}
	private:
		Vecs m;
	};

	class Euler
	{
	public:
		typedef std::tr1::function<void (double, const std::vector<double>& , std::vector<double>&)
		Euler(double , double, const std::vector<double> &);
		virtual ~Euler() {};
		void setStep(double step) { dStep = step; }
		void setAccuracy(double accur) { dAccuracy = accur; }
		void setInitConds(const std::vector<double> &crInitConds) { vecInitConds = crInitConds; } // не уверен, что копирование вектора - хороший ход
	private:
		double dStep;
		double dAccuracy;
		std::vector<double> vecInitConds;
	};
}