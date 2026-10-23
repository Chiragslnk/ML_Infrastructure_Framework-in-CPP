#pragma once
#include<iostream>
#include<matrix.h>
using namespace std;
class ml_math
{
private:
ml_math(){}
public: 
static double mean_squared_error(const matrix &y_hat, const matrix &y);
static double r2score(matrix &y,matrix &y_hat);
static double scale_min_max(double value,double min,double max);
};
