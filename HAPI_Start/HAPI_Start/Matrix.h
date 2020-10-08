#pragma once
#include <vector>
class Matrix {
public:
	explicit Matrix(const int _rows, const int _cols, const std::vector<std::vector<float>>& _values = std::vector<std::vector<float>>());

	void SetValue(float _val, int i, int j)
	{
		m_values[i][j] = _val;
	}

	float GetValue(int i, int j) const
	{
		return m_values[i][j];
	}

	static Matrix MatMul(const Matrix& a, const Matrix& b);


	int m_rows;
	int m_columns;
private:
	std::vector<std::vector<float>> m_values;
};

