#pragma once
#include "vector_utils.h"
typedef  std::vector<vec_n> mat_mn;

static mat_mn zeros(const int rows, const int cols)
{
	mat_mn res;
	res.resize(rows);
	for (auto& row : res)
	{
		row.resize(cols);
		for (int i = 0; i < cols; i++)
		{
			row[i] = 0;
		}
	}
	return res;
}

static mat_mn zeros(const int size)
{
	return zeros(size, size);
}

static mat_mn identity(const int rows, const int cols)
{
	mat_mn res;
	res.resize(rows);
	int row_n = 0;
	for (auto& row : res)
	{
		row.resize(cols);
		for (int i = 0; i < cols; i++)
		{
			row[i] = row_n == i ? 1.0f : 0.0f;
		}
		row_n++;
	}
	return res;
}

static mat_mn identity(const int size)
{
	return identity(size, size);
}

static std::vector<size_t> size(const mat_mn& mat)
{
	if (mat.size() == 0)
	{
		return { mat.size(), (size_t)(-1) };
	}
	return { mat.size(), mat[0].size() };
}

static std::ostream& operator<<(std::ostream& steram, const mat_mn& v)
{
	steram << "{ \n";
	for (int i = 0; i < v.size() - 1; i++)
	{
		steram << v[i] << ",\n";
	}
	steram << v[v.size() - 1];

	steram << "\n }";

	return steram;
}

static mat_mn hessian(func_n func, vec_n& x, const double eps = 1e-6f)
{
	mat_mn res = zeros(x.size(), x.size());
	int row, col;
	for (row = 0; row < x.size(); row++)
	{
		for (col = 0; col <= row; col++)
		{
			res[row][col] = partial2(func, x, row, col, eps);
			res[col][row] = res[row][col];
		}
	}
	return res;
}

static vec_n  operator*(const mat_mn& mat, const vec_n& vec)
{
	auto _size = size(mat);
	if (_size[1] != vec.size())
	{
		throw std::runtime_error("error :: matrix by vector multipliction");
	}
	vec_n result;
	result.resize(_size[0]);
	int cntr = 0;
	for (auto const& row : mat)
	{
		result[cntr++] = dot(row, vec);
	}
	return result;
}

static vec_n  operator*(const vec_n& vec, const mat_mn& mat)
{
	auto _size = size(mat);
	if (_size[0] != vec.size())
	{
		throw std::runtime_error("error :: vector by matrix multipliction");
	}
	vec_n result;
	result.resize(_size[1]);
	int cntr = 0;

	for (int col = 0; col < _size[1]; col++)
	{
		result[0] = 0;
		for (int row = 0; row < _size[1]; row++)
		{
			result[col] += mat[col][row] * vec[row];
		}
	}
	return result;
}

static mat_mn operator*(const mat_mn& a, const mat_mn& b)
{
	auto a_size = size(a);
	auto b_size = size(b);

	if (a_size[1] != b_size[0])
	{
		throw std::runtime_error("error :: matrix by matrix multipliction");
	}
	mat_mn result;
	result.resize(a_size[0]);//, b_size[0]);

	for (int row = 0; row < a_size[0]; row++)
	{
		result[row].resize(b_size[1]);

		for (int col = 0; col < b_size[1]; col++)
		{
			result[row][col] = 0;

			for (int k = 0; k < b_size[0]; k++)
			{
				result[row][col] += a[row][k] * b[k][col];
			}
		}
	}
	return result;
}

static mat_mn operator*(const mat_mn& mat, const double& a)
{
	mat_mn res;
	res.resize(mat.size());
	for (int i = 0; i < res.size(); i++)
	{
		res[i] = mat[i] * a;
	}
	return res;
}

static mat_mn operator+(const mat_mn& a, const mat_mn& b)
{
	auto _size = size(a);
	if (_size != size(b))
	{
		throw std::runtime_error("error :: matrix + matrix");
	}
	mat_mn result;
	result.resize(_size[0]);
	for (int i = 0; i < result.size(); i++)
	{
		result[i] = a[i] + b[i];
	}
	return result;
}

static mat_mn operator-(const mat_mn& a, const mat_mn& b)
{
	auto _size = size(a);
	if (_size != size(b))
	{
		throw std::runtime_error("error :: matrix - matrix");
	}
	mat_mn result;
	result.resize(_size[0]);
	for (int i = 0; i < result.size(); i++)
	{
		result[i] = a[i] - b[i];
	}
	return result;
}

static void lu(const mat_mn& mat, mat_mn& low, mat_mn& up)
{
	int rows = mat.size();

	int cols = mat[0].size();

	if (rows != cols)
	{
		throw std::runtime_error("error :: matrix lu decomposition :: non square matrix");
	}

	if (size(mat) != size(low))
	{
		low = zeros(rows, cols);
	}

	if (size(mat) != size(up))
	{
		up = zeros(rows, cols);
	}

	int i = 0, j = 0, k = 0;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < rows; j++)
		{
			if (j >= i)
			{
				low[j][i] = mat[j][i];

				for (k = 0; k < i; k++)
				{
					low[j][i] = low[j][i] - low[j][k] * up[k][i];
				}
			}
		}

		for (j = 0; j < cols; j++)
		{
			if (j < i)
			{
				continue;
			}
			if (j == i)
			{
				up[i][j] = 1;
				continue;
			}
			up[i][j] = mat[i][j] / low[i][i];

			for (k = 0; k < i; k++)
			{
				up[i][j] = up[i][j] - ((low[i][k] * up[k][j]) / low[i][i]);
			}
		}
	}
}

static vec_n linsolve(const mat_mn& low, const mat_mn& up, const vec_n& b)
{
	double det = 1.0f;
	
	vec_n x, z;

	for (int i = 0; i < up.size(); i++)
	{
		det *= (up[i][i] * up[i][i]);
	}

	if (fabs(det) < 1e-12f)
	{
		return x;
	}

	z.resize(low.size());

	double tmp;

	for (int i = 0; i < z.size(); i++)
	{
		tmp = 0.0f;
		for (int j = 0; j < i; j++)
		{
			tmp += z[j] * low[i][j];
		}
		z[i] = (b[i] - tmp) / low[i][i];
	}

	x.resize(low.size());

	for (int i = z.size() - 1; i >= 0; i--)
	{
		tmp = 0.0f;
		for (int j = i + 1; j < z.size(); j++)
		{
			tmp += x[j] * up[i][j];
		}
		x[i] = (z[i] - tmp);
	}

	return x;
}

static vec_n linsolve(const mat_mn& mat, const vec_n& b)
{
	int rows = mat.size();

	int cols = mat[0].size();

	if (rows != cols)
	{
		throw std::runtime_error("error :: matrix inversion :: non square matrix");
	}

	mat_mn low, up;

	lu(mat, low, up);

	return linsolve( low,  up, b);
}

static mat_mn invert(const mat_mn& mat)
{
	int rows = mat.size();

	int cols = mat[0].size();

	if (rows != cols)
	{
		throw std::runtime_error("error :: matrix inversion :: non square matrix");
	}

	mat_mn low, up, inv;

	lu(mat, low, up);

	vec_n b, col;

	b.resize(rows);

	for (int i = 0; i < b.size(); i++)
	{
		b[i] = 0.0f;
	}

	inv = zeros(rows);

	for (int i = 0; i < cols; i++)
	{
		b[i] = 1.0f;
		col = linsolve(low, up, b);
		if (col.size() == 0)
		{
			throw std::runtime_error("error :: unable to find matrix inversion");
		}

		b[i] = 0.0f;
		for (int j = 0; j < rows; j++)
		{
			inv[j][i] = col[j];
		}
	}
	return inv;
}
const double mat_eps = 1E-9;

mat_mn& add_row(mat_mn& mat, const vec_n& row)
{
	if (mat.size() == 0)
	{
		mat.push_back(row);
		return mat;
	}
	if (mat[0].size() != row.size())
	{
		throw std::runtime_error("error :: add_row");
	}
	mat.push_back(row);
	return mat;
}

mat_mn& add_col(mat_mn& mat, const vec_n& col)
{
	if (mat.size() == 0)
	{
		for (int i = 0;i < col.size(); i++)
		{
			mat.push_back(vec_n());
			mat[i].push_back(col[i]);
		}
		return mat;
	}
	if (mat.size() != col.size())
	{
		throw std::runtime_error("error :: add_col");
	}
	for (int i = 0; i < col.size(); i++)
	{
		mat[i].push_back(col[i]);
	}
	return mat;
}

int rank(mat_mn& A)
{
	int n = A.size();

	int m = A[0].size();

	int rank = 0;

	std::vector<bool> row_selected(n, false);

	for (int i = 0; i < m; i++)
	{
		int j;
		for (j = 0; j < n; j++)
		{
			if (!row_selected[j] && abs(A[j][i]) > mat_eps)
			{
				break;
			}
		}

		if (j != n)
		{
			++rank;

			row_selected[j] = true;

			for (int p = i + 1; p < m; p++)
			{
				A[j][p] /= A[j][i];
			}

			for (int k = 0; k < n; k++)
			{
				if (k != j && abs(A[k][i]) > mat_eps)
				{
					for (int p = i + 1; p < m; p++)
					{
						A[k][p] -= A[j][p] * A[k][i];
					}
				}
			}
		}
	}
	return rank;
}

int rank(const mat_mn& a)
{
	mat_mn A = a;
	return rank(A);
}
