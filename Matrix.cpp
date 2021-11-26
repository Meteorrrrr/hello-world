#include "Matrix.hpp"

template <typename T>
Matrix<T>::Matrix(string str)
{
    string out;
    for (int i = 0; i < str.size(); i++) {
        int tmp = (int) str[i];
        if (tmp < 48 || tmp > 57) {
            continue;
        } else {
            out.append(str, i, 1);
        }
    }
    int size = stoi(out);
    this->rows = size;
    this->cols = size;
    this->mat = new T[rows * cols];
    printf("The matrix of size %d rows & %d colsumns has been built successfully with equal rows and cols!\n", rows, cols);

    //input the matrix
    ifstream myfile(str);
    for (int i = 0; i < size * size; i++)
    {
        myfile >> mat[i];
    }
}

template <typename T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols)
{
    this->mat = new T[rows * cols];
    for(int i = 0;i<rows * cols;i++){
        this->mat[i] = 0;
    }
    printf("The matrix of size %d rows & %d columns has been built successfully without data!\n", rows, cols);
}

// template <typename T>
// Matrix<T>::Matrix(const Matrix<T> &m)
// {
//     this->rows = m.rows;
//     this->cols = m.cols;
//     this->mat = m.mat;
//     printf("Using copy constructor\n");
// }

template <typename T>
Matrix<T>::Matrix(int rows, int cols, string matrixSource) : rows(rows), cols(cols)
{
    //input the matrix
    this->mat = new T[rows * cols];
    ifstream myfile(matrixSource);
    for (int i = 0; i < rows * cols; i++)
    {
        myfile >> mat[i];
    }
    printf("The matrix of size %d rows & %d columns has been built from source with special rows and cols!\n", rows, cols);
}

template <typename T>
int Matrix<T>::getRows() const
{
    return this->rows;
}

template <typename T>
int Matrix<T>::getCols() const
{
    return this->cols;
}

template <typename T>
T *Matrix<T>::getMat() const
{
    return this->mat;
}

template <typename T> // overload operator "+"
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m)
{
    if (this->cols != m.getRows() || this->cols != m.getCols())
    {
        cout << "The size of two input matrices is not proper!!" << endl;
        Matrix<T> Wrong(0, 0);
        return Wrong;
    }
    else
    {
        Matrix<T> result(this->rows, this->cols);
        for (int i = 0; i < this->rows * this->cols; i++)
        {
            result.getMat()[i] = this->mat[i] + m.getMat()[i];
            // cout<<result.getMat()[i]<<"="<<this->mat[i]<<"+"<<m.getMat()[i]<<endl;
        }
        return result;
    }
}

template <typename T> // overload operator "+"
Matrix<T> Matrix<T>::operator*(const Matrix<T> &m)
{
    omp_set_num_threads(8);
    if (this->cols != m.getRows())
    {
        cout << "The size of two input matrices is not proper!!" << endl;
        Matrix<T> Wrong(0, 0);
        return Wrong;
    }
    else
    {
        Matrix<T> result(this->rows, m.getCols());
        #pragma omp parallel for
        for (int i = 0; i < this->rows; i++) {
            for (int k = 0; k < this->cols; k++) {
                float s = this->mat[i*this->rows+k];
                // float s = *(A->matrix + (i * A->n + k));
                for (int j = 0; j < m.cols; j++) {
                    result.getMat()[i*this->rows+j]+=s*m.getMat()[k*this->cols+j];
                    printf("C(%d,%d) += A(%d,%d) * B(%d,%d); %.2f * %.2f\n", i*this->rows, j, i*this->rows, k, k*this->rows, j, s, m.getMat()[k*this->rows+j]);
                }
            }
        }
        return result;
    }
}

template <typename T> // overload operator "-"
Matrix<T> Matrix<T>::operator-(const Matrix<T> &m)
{
    if (this->rows != m.getRows() || this->cols != m.getCols())
    {
        cout << "The size of two input matrices is not proper!!" << endl;
        Matrix<T> Wrong(0, 0);
        return Wrong;
    }
    else
    {
        Matrix<T> result(this->rows, this->cols);

        for (int i = 0; i < this->rows * this->cols; i++)
        {
            result.getMat()[i] = this->mat[i] - m.getMat()[i];
            // cout<<result.getMat()[i]<<"="<<this->mat[i]<<"+"<<m.getMat()[i]<<endl;
        }
        return result;
    }
}

template <typename T> // overload operator "="
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &m)
{
    this->rows = m.getRows();
    this->cols = m.getCols();
    if (this->mat != NULL)
    {
        this->mat = m.mat;
    }
    return *this;
}

template <typename T> // print the matrix
ostream &operator<<(ostream &os, Matrix<T> &m)
{
    for (int i = 0; i < m.getRows() * m.getCols(); i++)
    {
        os <<m.getMat()[i] <<" ";
        if ((i + 1) % m.getCols() == 0)
        {
            os << endl;
        }
    }
    return os;
}

template <typename T> // overload operator "=="
bool Matrix<T>::operator==(const Matrix<T> &m)
{
    if (this->row != m.getRow() || this->col != m.getCol())
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < this->row * this->col; i++)
        {
            if (this->mat[i] != m.getMat()[i])
            {
                return 0;
            }
        }
        return 1;
    }
}

// template <typename T>
// Matrix<T>::~Matrix(){
//     delete []this->mat;
// }