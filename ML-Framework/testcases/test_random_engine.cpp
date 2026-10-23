#include<iostream>
#include<random_engine.h>
#include<matrix.h>
using namespace std;
int main()
{
matrix m(4,5);
random_engine::fill(m,0.0,1.0);
for(int rr=0;rr<m.rows();++rr)
{
for(int cc=0;cc<m.columns();++cc)
{
//printf("%f ",m[rr][cc]); // never ever do this
cout<<m[rr][cc]<<" ";
}
cout<<endl;
}
return 0;
}
