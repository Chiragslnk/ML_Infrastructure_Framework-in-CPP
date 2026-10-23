#include<random>
#include<random_engine.h> 
void random_engine::fill(matrix &m,double lower_bound,double upper_bound)
{
random_device r; 
uniform_real_distribution<double> d(lower_bound,upper_bound);
mt19937 engine(r());
double *ptr=m.collection.data();
size_t sz=m.collection.size();
for(int i=0;i<sz;++i)
{
ptr[i]=d(engine);
}
}
