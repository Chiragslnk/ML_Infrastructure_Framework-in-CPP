#include<math.h>
#include<ml_math.h>
#include<ml_exception.h> 
double ml_math::mean_squared_error(const matrix &y_hat, const matrix &y)
{
if(y._rows==0 || y._columns!=1 || y._columns!=y_hat._columns || y._rows!=y_hat._rows)
{
throw ml_exception("y and y_hat are of invalid dimensions");
}
double sum=0.0;
uint64_t rows=y_hat.rows(); 
//for(uint64_t i=0;i<rows;++i) sum+=pow(y_hat[i][0]-y[i][0],2.0);
// we are not doing whatever is written above, to avoid
// object creation time and function call time
const double *y_ptr=y.collection.data();
const double *y_hat_ptr=y_hat.collection.data(); 
for(uint64_t i=0;i<rows;++i) sum+=pow(y_hat_ptr[i]-y_ptr[i],2.0);
return sum/(double)rows;
}
double ml_math::r2score(matrix &y,matrix &y_hat)
{
if(y._rows==0 || y._columns!=1 || y._columns!=y_hat._columns || y._rows!=y_hat._rows)
{
throw ml_exception("y and y_hat are of invalid dimensions");
}
double y_mean;
uint64_t r;
const double *y_ptr=y.collection.data();
const double *y_hat_ptr=y_hat.collection.data(); 
for(y_mean=0.0,r=0;r<y._rows;++r) y_mean+=y_ptr[r];
y_mean/=y._rows;
double variance,error;
double variance_squared_sum=0.0;
double error_squared_sum=0.0;
for(r=0;r<y._rows;++r)
{
variance=y_ptr[r]-y_mean;
variance_squared_sum+=(variance*variance);
error=y_hat_ptr[r]-y_ptr[r];
error_squared_sum+=(error*error);
}
return 1.0-(error_squared_sum/variance_squared_sum);
}
double ml_math::scale_min_max(double value,double min,double max)
{
if(max==min) return value;
return (value-min)/(max-min);
}
