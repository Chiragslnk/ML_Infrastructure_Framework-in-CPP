#pragma once
#include<cost_function_enum.h>
#include<matrix.h>
#include<iostream>
using namespace std;
class linear_regression
{
private:
linear_regression();
public: static matrix batch_gradient_fit(const matrix &x,const matrix &y,double learning_rate,double regularization_parameter,uint64_t number_of_epoches,cost_function_enum cost_function_type, int (*on_epoch)(double,uint64_t,const matrix &,matrix &,matrix &,double,double,uint64_t)); 
/*the parameters of the call back function (event handler)
---------------------
for batch gradient
---------------------
cost
epoch number
matrix x
matrix y_hat
matrix model
learning_rate
regularization parameter
number of epoches
*/ 
static matrix mini_batch_gradient_fit(void (*data_provider)(uint32_t,matrix &,matrix &),double learning_rate,double regularization_parameter,uint64_t number_of_epoches,cost_function_enum cost_function_type, int (*on_mini_batch_processed)(double,uint64_t,matrix &,matrix &,matrix &,double,double,uint64_t)); 
/*the parameters of the call back function (event handler)
---------------------
for mini batch gradient
---------------------
batch_number
cost
epoch number
matrix x
matrix y_hat
matrix model
learning_rate
regularization parameter
number of epoches
*/
//do it yourself stochastic gradient
};
