#include <vector>

namespace MathStuff 
{
	class Matrix
	{
	public:
		Matrix();
		Matrix(int numRows, int numColumns);
		int rowCount();
		int columnCount();
		std::vector<double>& operator[](int ind) {
			return row[ind];
		}
	private:
		std::vector<std::vector<double>> row;
	};
}