#include "hw1.h"
#include <assert.h>

static inline void check_empty(std::initializer_list<Matrix> ilm) {
    for (auto it = ilm.begin(); it != ilm.end(); it++)
        if (it->empty())
            throw std::logic_error("matrix must be non-empty");
}

static inline void check_square(const Matrix &matrix) {
    if (matrix.size() != matrix[0].size())
        throw std::logic_error("matrix must be square");
}

static inline void check_size(size_t n, size_t m) {
    if (n == 0 || m == 0)
        throw std::logic_error("matrix size must be non-zero");
}

static inline bool is_empty(const Matrix &matrix) { return matrix.size() == 0; }

// ? IMPLEMENT CODE (NOT CHECKED)
namespace algebra {

Matrix zeros(size_t n, size_t m) {
    check_size(n, m);
    return Matrix(n, std::vector<double>(m, 0));
}

Matrix ones(size_t n, size_t m) {
    check_size(n, m);
    return Matrix(n, std::vector<double>(m, 1));
}

Matrix random(size_t n, size_t m, double min, double max) {
    check_size(n, m);
    if (min >= max) {
        throw std::logic_error(
            "interval left must be no more than interval right");
    }
    Matrix matrix = zeros(n, m);
    // ! ref:
    // https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    std::default_random_engine engine;
    std::uniform_real_distribution<double> u(min, max);
    for (auto &v : matrix) {
        for (auto &e : v)
            e = u(engine);
    }
    return matrix;
}

void show(const Matrix &matrix) {
    check_empty({matrix});
    std::cout << std::showpoint << std::setprecision(3);
    for (auto &v : matrix) {
        for (auto &e : v)
            std::cout << e << "\t";
        std::cout << std::endl;
    }
    std::cout << std::noshowpoint << std::setprecision(3);
}

Matrix multiply(const Matrix &matrix, double c) {
    check_empty({matrix});
    Matrix res(matrix);
    for (auto &v : res) {
        for (auto &e : v)
            e *= c;
    }
    return res;
}

Matrix multiply(const Matrix &matrix1, const Matrix &matrix2) {
    if (is_empty(matrix1))
        return matrix2;
    if (is_empty(matrix2))
        return matrix1;
    size_t n1 = matrix1.size(), m1 = matrix1[0].size();
    size_t n2 = matrix2.size(), m2 = matrix2[0].size();
    if (m1 != n2)
        throw std::logic_error("dimensions don't match");
    Matrix res = zeros(n1, m2);
    for (auto i = 0; i < n1; i++) {
        for (auto j = 0; j < m2; j++) {
            for (auto k = 0; k < m1; k++)
                res[i][j] += matrix1[i][k] * matrix2[k][j];
        }
    }
    return res;
}

Matrix sum(const Matrix &matrix, double c) {
    if (is_empty(matrix))
        return matrix;
    Matrix res(matrix);
    for (auto &v : res) {
        for (auto &e : v)
            e += c;
    }
    return res;
}

Matrix sum(const Matrix &matrix1, const Matrix &matrix2) {
    if (is_empty(matrix1) && is_empty(matrix2))
        return matrix1;
    size_t n1 = matrix1.size(), m1 = matrix1[0].size();
    size_t n2 = matrix2.size(), m2 = matrix2[0].size();
    if ((n1 != n2) || (m1 != m2))
        throw std::logic_error("dimensions don't match");
    Matrix res(matrix1);
    for (auto i = 0; i < n1; i++) {
        for (auto j = 0; j < m1; j++)
            res[i][j] += matrix2[i][j];
    }
    return res;
}

Matrix transpose(const Matrix &matrix) {
    if (is_empty(matrix))
        return matrix;
    size_t n = matrix.size(), m = matrix[0].size();
    Matrix res = zeros(m, n);
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < m; j++)
            res[j][i] = matrix[i][j];
    }
    return res;
}

Matrix minor(const Matrix &matrix, size_t n, size_t m) {
    if (is_empty(matrix))
        return matrix;
    check_square(matrix);
    size_t n0 = matrix.size(), m0 = matrix[0].size();
    if (n >= n0 || m >= m0)
        throw std::logic_error("index out of bound");
    Matrix res = zeros(n0 - 1, m0 - 1);
    for (auto i = 0; i < n0 - 1; i++) {
        for (auto j = 0; j < m0 - 1; j++) {
            auto ii = (i < n) ? i : i + 1;
            auto jj = (j < m) ? j : j + 1;
            res[i][j] = matrix[ii][jj];
        }
    }
    return res;
}

static inline double det2(const Matrix &matrix) {
    assert(matrix.size() == 2 && matrix[0].size() == 2);
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

static inline double det3(const Matrix &matrix) {
    assert(matrix.size() == 3 && matrix[0].size() == 3);
    return matrix[0][0] * matrix[1][1] * matrix[2][2] +
           matrix[0][1] * matrix[1][2] * matrix[2][0] +
           matrix[0][2] * matrix[1][0] * matrix[2][1] -
           matrix[0][2] * matrix[1][1] * matrix[2][0] -
           matrix[0][0] * matrix[1][2] * matrix[2][1] -
           matrix[0][1] * matrix[1][0] * matrix[2][2];
}

double determinant(const Matrix &matrix) {
    if (is_empty(matrix))
        return 1.0;
    check_square(matrix);
    size_t n = matrix.size();
    if (n == 1)
        return matrix[0][0];
    if (n == 2)
        return det2(matrix);
    if (n == 3)
        return det3(matrix);
    double res = 0;
    for (auto i = 0; i < n; i++) {
        Matrix sub_matrix = minor(matrix, 0, i);
        int sign = (i & 1) ? -1 : 1;
        res += sign * matrix[0][i] * determinant(sub_matrix);
    }
    return res;
}

static Matrix adjoint(const Matrix &matrix) {
    size_t n = matrix.size();
    Matrix res = zeros(n, n);
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            int sign = (i + j) & 1 ? -1 : 1;
            Matrix sub_matrix = minor(matrix, i, j);
            res[i][j] = sign * determinant(sub_matrix);
        }
    }
    return transpose(res);
}

Matrix inverse(const Matrix &matrix) {
    if (is_empty(matrix))
        return matrix;
    check_square(matrix);
    double det = determinant(matrix);
    if (det == 0)
        throw std::logic_error("singular matrix has no inverse");
    Matrix res = adjoint(matrix);
    return multiply(res, 1.0 / det);
}

Matrix concatenate(const Matrix &matrix1, const Matrix &matrix2, int axis) {
    if (is_empty(matrix1))
        return matrix2;
    if (is_empty(matrix2))
        return matrix1;
    size_t n1 = matrix1.size(), m1 = matrix1[0].size();
    size_t n2 = matrix2.size(), m2 = matrix2[0].size();
    Matrix res;
    if (axis == 0) { // axis equals to 0
        if (m1 != m2)
            throw std::logic_error("columns don't match while axis=0");
        res = zeros(n1 + n2, m1);
        for (auto i = 0; i < n1; i++) {
            for (auto j = 0; j < m1; j++)
                res[i][j] = matrix1[i][j];
        }
        for (auto i = 0; i < n2; i++) {
            for (auto j = 0; j < m2; j++)
                res[i + n1][j] = matrix2[i][j];
        }
    } else if (axis == 1) { // axis equals to 1
        if (n1 != n2)
            throw std::logic_error("rows don't match while axis=1");
        res = zeros(n1, m1 + m2);
        for (auto i = 0; i < n1; i++) {
            for (auto j = 0; j < m1; j++)
                res[i][j] = matrix1[i][j];
        }
        for (auto i = 0; i < n2; i++) {
            for (auto j = 0; j < m2; j++)
                res[i][j + m1] = matrix2[i][j];
        }
    } else
        throw std::invalid_argument("axis must be 0 or 1");
    return res;
}

Matrix ero_swap(const Matrix &matrix, size_t r1, size_t r2) {
    if (is_empty(matrix))
        return matrix;
    size_t n = matrix.size();
    if (r1 >= n || r2 >= n)
        throw std::logic_error("row index out of bound");
    Matrix res(matrix);
    std::swap(res[r1], res[r2]);
    return res;
}

Matrix ero_multiply(const Matrix &matrix, size_t r, double c) {
    if (is_empty(matrix))
        return matrix;
    size_t n = matrix.size();
    if (r >= n)
        throw std::logic_error("row index out of bound");
    Matrix res(matrix);
    for (auto &e : res[r])
        e *= c;
    return res;
}

Matrix ero_sum(const Matrix &matrix, size_t r1, double c, size_t r2) {
    if (is_empty(matrix))
        return matrix;
    size_t n = matrix.size(), m = matrix[0].size();
    if (r1 >= n || r2 >= n)
        throw std::logic_error("row index out of bound");
    Matrix res(matrix);
    for (auto j = 0; j < m; j++)
        res[r2][j] += res[r1][j] * c;
    return res;
}

Matrix upper_triangular(const Matrix &matrix) {
    if (is_empty(matrix))
        return matrix;
    size_t n = matrix.size();
    check_square(matrix);
    if (n == 1)
        return matrix;
    Matrix res(matrix);
    for (auto i = 0; i < n; i++) {
        double diagonal_elem = matrix[i][i];
        for (auto j = i + 1; j < n; j++) {
            double elem = res[j][i];
            res = ero_sum(res, i, -elem / diagonal_elem, j);
        }
    }
    return res;
}

}; // namespace algebra