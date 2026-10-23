#include<ml.h>
#define TRAINING_CYCLES 20000
#define LEARNING_RATE 0.01
int main()
{
try
{
uint64_t r,c;
FILE *f;
double mse,cost;
double learning_rate=LEARNING_RATE;
matrix x,y;
csv::load_x_y("training_icecream_sales.csv",x,y,1);
matrix min_max=scaler::min_max_scaler(x); 
f=fopen("icecream_sales_scaled_training_data.csv","w");
size_t rows_sz=x.rows();
size_t columns_sz=x.columns();
for(r=0;r<rows_sz;++r)
{
for(c=1;c<columns_sz;++c)
{
fprintf(f,"%f,",x.get(r,c));
}
fprintf(f,"%f\n",y.get(r,0));
}
fclose(f);
matrix x_transposed=x.as_transposed();
matrix y_hat;
matrix model(x.columns(),1);
matrix error;
random_engine::fill(model,0.0,1.0);
f=fopen("icecream_sales_cost.csv","w");
uint64_t cycle_number;
uint64_t x_rows=x.rows(); 
for(cycle_number=1;cycle_number<=TRAINING_CYCLES;++cycle_number)
{
y_hat=x*model;
mse=ml_math::mean_squared_error(y_hat,y); 
cost=mse/2.0; //we have modified cost as mse/2 instead of mse
fprintf(f,"%d,%f\n",cycle_number,cost);
error=y_hat-y; 
model=model-(learning_rate/x_rows)*(x_transposed * error);
if(cycle_number%1000==0)
{
cout<<"Cycle : "<<cycle_number<<",Cost : "<<cost<<endl;
}
}
fclose(f);
// save model
model.save("icecream_sales_model.csv",FMT_CSV);
// save min_max 
min_max.save("icecream_sales_min_max.csv",FMT_CSV);
// save for plotting line
f=fopen("icecream_sales_line_data.csv","w");
rows_sz=x.rows();
columns_sz=x.columns();
for(r=0;r<rows_sz;++r)
{
for(c=1;c<columns_sz;++c)
{
fprintf(f,"%f,",x.get(r,c));
}
fprintf(f,"%f\n",y_hat.get(r,0));
}
fclose(f);
// calculate r2score
matrix x_test,y_test; 
csv::load_x_y("testing_icecream_sales.csv",x_test,y_test,1);
scaler::min_max_scaler(x_test,min_max);
matrix y_hat_test(y_test.rows(),1);
y_hat_test=x_test*model;
double r2score=ml_math::r2score(y_test,y_hat_test);
cout<<"R2Score : "<<r2score<<endl;
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
}
return 0;
}
