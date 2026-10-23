#include<iomanip>
#include<ml.h>
#define TRAINING_CYCLES 20000
#define LEARNING_RATE 0.01
#define REGULARIZATION_PARAMETER 0.0001
plotter cost_graph;
plotter line_fit_graph; 
int on_epoch_callback(double cost,uint64_t training_cycle_number,const matrix &x,matrix &y,matrix &model,double learning_rate,double regularization_parameter,uint64_t number_of_epoches)
{
double px1,py1,px2,py2,m,c; 
if(training_cycle_number%20==0 || training_cycle_number<100)
{
cout<<"After training cycle number : "<<training_cycle_number<<", cost is "<<setprecision(17)<<cost<<endl;
cost_graph.add(point(training_cycle_number,cost));
cost_graph.plot();
line_fit_graph.remove('L');
c=model.get(0,0);
m=model.get(1,0);
px1=-0;
py1=m*px1+c;
px2=1;
py2=m*px2+c; 
line_fit_graph.add(line(point(px1,py1),point(px2,py2)));
line_fit_graph.plot();
}
return 1; // continue training
}
int main()
{
try
{
uint64_t r,c;
FILE *f;
double learning_rate=LEARNING_RATE; 
double regularization_parameter=REGULARIZATION_PARAMETER;
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
cost_graph.set_title("Cost graph");
cost_graph.set_x_range({1,10000});
cost_graph.set_y_range({0,600});
cost_graph.set_x_label("Cost");
cost_graph.set_y_label("Training cycles");
cost_graph.set_size({600,500});
cost_graph.set_location({10,10});
cost_graph.set_terminal_number(1);
line_fit_graph.set_title("Fitting line");
line_fit_graph.set_x_range({-2,2});
line_fit_graph.set_y_range({-2,120});
line_fit_graph.set_x_label("Temperature");
line_fit_graph.set_y_label("Revenue");
line_fit_graph.set_size({600,500});
line_fit_graph.set_location({700,10});
line_fit_graph.set_terminal_number(2);
auto x_rows=x.rows();
for(int i=0;i<x_rows;++i)
{
line_fit_graph.add(point(x.get(i,1),y.get(i,0)));
}
matrix model=linear_regression::batch_gradient_fit(x,y,learning_rate,regularization_parameter,10000,MEAN_SQUARED_ERROR,on_epoch_callback);
// save model
model.save("icecream_sales_model.csv",FMT_CSV);
// save min_max min_max.save("icecream_sales_min_max.csv",FMT_CSV);
//calculate r2score
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
