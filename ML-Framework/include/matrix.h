#pragma once
#include<iostream>
#include<vector>
#include<cstdint>
#include<string>
#include<cstring>
using namespace std;
#define IDENTITY 2
enum FileFormat
{
FMT_BINARY,
FMT_CSV
};
class matrix
{
private:
uint32_t flags;
vector<double> collection;
uint64_t _rows;
uint64_t _columns; 
matrix(uint64_t _rows,uint64_t _columns,uint32_t flags);
public:
class matrix_row;
// inner class
class matrix_cell
{
private:
uint64_t row_index;
uint64_t column_index;
matrix *mat; 
matrix_cell(matrix *mat,uint64_t row_index,uint64_t columns_index);
public:
operator double() const;
matrix_cell & operator=(double value);
friend class matrix::matrix_row;
};
class matrix_row
{
private:
uint64_t row_index;
matrix *mat;
matrix_row(matrix *mat,uint64_t row_index);
public:
matrix_cell operator[](uint64_t column_index);
friend class matrix;
};
matrix();
matrix(uint64_t _rows,uint64_t _columns);
matrix(const matrix &other); // copy constructor
matrix(matrix &&other); // move constructor
~matrix(); //destructor 
matrix & operator=(const matrix &other); // copy assignment 
matrix & operator=(matrix &&other); // move assignment
uint64_t rows() const;
uint64_t columns() const; 
void set(uint64_t row_index,uint64_t column_index,double value); 
double get(uint64_t row_index,uint64_t column_index) const;
matrix as_transposed() const;
private:
void save_in_binary_format(string filename) const;
void save_in_csv_format(string filename) const;
public:
void save(string filename,uint8_t flags) const;
matrix operator+(const matrix &other) const;
matrix operator-(const matrix &other) const;
matrix operator*(const matrix &other) const;
matrix operator+(double right) const;
matrix operator-(double right) const;
matrix operator*(double right) const;
matrix operator/(double right) const;
matrix operator++(); // prefix
matrix operator++(int); // postfix
matrix operator--(); // prefix
matrix operator--(int); // postfix
matrix operator-=(const matrix &other);
matrix operator+=(const matrix &other);
matrix operator*=(const matrix &other);
matrix operator+=(double value);
matrix operator-=(double value);
matrix operator*=(double value);
matrix operator/=(double value);
matrix operator-() const;
matrix operator^(uint32_t value) const;
static matrix identity(uint64_t rows); 
friend matrix operator+(double left,const matrix &right); 
friend matrix operator-(double left,const matrix &right); 
friend matrix operator*(double left,const matrix &right); 
friend matrix operator/(double left,const matrix &right);
friend class csv;
friend class scaler;
friend class random_engine; // one more method required for computing element wise power
matrix_row operator[](uint64_t row_index);
friend class ml_math;
friend class dataset_utils;
//friend int main();
};
matrix operator+(double left,const matrix &right);
matrix operator-(double left,const matrix &right);
matrix operator*(double left,const matrix &right);
matrix operator/(double left,const matrix &right);
