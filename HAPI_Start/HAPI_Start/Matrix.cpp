#include "Matrix.h"

Matrix::Matrix(const int _rows, const int _cols, const std::vector<std::vector<float>>& _values) {
	if (!_values.empty()) {
		for (int i = 0; i < m_rows; ++i) {
			std::vector<float> currentRow;
			for (int j = 0; j < m_columns; j++) {
				currentRow.push_back(_values[i][j]);
			}
			m_values.push_back(currentRow);
		}
	}
	else {
		for (int row = 0; row < m_rows; row++)
		{
			std::vector<float> currentRow;
			for (int column = 0; column < m_columns; column++)
			{
				currentRow.push_back(0.f);
			}
			m_values.push_back(currentRow);
		}
	}
}

Matrix Matrix::MatMul(Matrix& a, Matrix& b) {
	const int n = a.m_rows;
	const int m = a.m_columns;
	const int p = b.m_columns;
	Matrix c(n, p);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < p; j++) {
			float sum{};
			for (int k = 0; k < m; k++) {
				sum += a.GetValue(i, k) * b.GetValue(k, j);
			}
			c.SetValue(sum, i, j);
		}
	}
	return c;
}