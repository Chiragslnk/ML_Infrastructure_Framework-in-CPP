#include<matrix.h>
#include<iostream>
using namespace std;
int main()
{
matrix im=matrix::identity(5);
auto rows=im.rows();
auto columns=im.columns();
for(int r=0;r<rows;++r)
{
for(int c=0;c<columns;++c)
{
cout<<im.get(r,c)<<" ";
}
cout<<endl;
}
cout<<"-------------------"<<endl;
im.set(0,1,1.0);
for(int r=0;r<rows;++r)
{
for(int c=0;c<columns;++c)
{
cout<<im.get(r,c)<<" ";
}
cout<<endl;
}
return 0;
}
