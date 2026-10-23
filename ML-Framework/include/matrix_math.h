#pragma once
#include<iostream>
#include<cstdint>
using namespace std;
class matrix_math
{
	matrix_math(){}
public:
	//matrix to matrix 
	static void add(double *target,const double *left,const double *right,const size_t sz); 
	static void subtract(double *target,const double *left,const double *right,const size_t sz); 
	static void multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double *right,const uint64_t right_rows,const uint64_t right_columns); 

	//matrix with scalar math(matrix on left and scaler on rignt)
	static void add(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns); 
	static void subtract(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns); 
	static void multiply(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns); 
	static void divide(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns);

	//scaler with matrix math(scaler on left and matrix on right) 
	static void add(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns); 
	static void subtract(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns); 
	static void multiply(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns); 
	static void divide(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns);
};

