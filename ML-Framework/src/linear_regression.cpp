#include<ml_math.h>
#include<matrix.h>
#include<linear_regression.h>
#include<iostream>
#include<random_engine.h>
using namespace std;
linear_regression::linear_regression()
{
}
matrix linear_regression::batch_gradient_fit(const matrix &x,const matrix &y,double learning_rate,double regularization_parameter,uint64_t number_of_epoches,cost_function_enum cost_function_type, int (*on_epoch)(double,uint64_t,const matrix &,matrix &,matrix &,double,double,uint64_t))
{
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
// apply validation (do it yourself)
matrix model(x.columns(),1);
matrix y_hat(y.rows(),1);
matrix error(y.rows(),1);
matrix x_transposed=x.as_transposed();
random_engine::fill(model,0.000001,0.999999);
double cost;
uint64_t training_cycle_number;
auto x_rows=x.rows();
int continue_training; 
for(training_cycle_number=1;training_cycle_number<number_of_epoches;++training_cycle_number)
{
y_hat=x*model; 
if(cost_function_type==MEAN_SQUARED_ERROR)
{
cost=ml_math::mean_squared_error(y_hat,y)/2.0;
if(on_epoch!=NULL)
{
continue_training=on_epoch(cost,training_cycle_number,x,y_hat,model,learning_rate,regularization_parameter,number_of_epoches);
if(!continue_training) break;
}
error=y_hat-y; 
model=model-(learning_rate/x_rows)*(x_transposed*error);
}
}
return model;
}
