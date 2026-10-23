#include<ml_exception.h>
#include<iostream>
#include<scaler.h>
#include<matrix.h>
using namespace std;
matrix scaler::min_max_scaler(matrix &mat)
{
uint64_t r,c;
if(mat._rows<=1)
{
throw ml_exception("Nothing to scale");
}
matrix tmp(2,mat._columns);
double min,max;
double value;
double scaled_value;
double divide_by_value;
for(c=0;c<mat._columns;++c)
{
value=mat[0][c];
tmp[0][c]=value;
tmp[1][c]=value;
}
for(r=1;r<mat._rows;++r)
{
for(c=0;c<mat._columns;++c)
{
value=mat[r][c];
if(value<tmp[0][c])
{
tmp[0][c]=value;
}
if(value>tmp[1][c])
{
tmp[1][c]=value;
}
}
}
for(r=0;r<mat._rows;++r)
{
for(c=0;c<mat._columns;++c)
{
value=mat[r][c];
min=tmp[0][c];
max=tmp[1][c];
divide_by_value=max-min;
if(divide_by_value!=0)
{
scaled_value=(value-min)/divide_by_value;
mat[r][c]=scaled_value;
}
}
}
return tmp;
}
void scaler::min_max_scaler(matrix &mat,matrix &min_max)
{
double min,max,value,scaled_value,divide_by_value;
uint64_t r,c; 
if(min_max._columns!=mat._columns || min_max._rows!=2)
{
throw ml_exception("Invalid min max matrix");
}
for(r=0;r<mat._rows;++r)
{
for(c=0;c<mat._columns;++c)
{
value=mat[r][c];
min=min_max[0][c];
max=min_max[1][c];
divide_by_value=max-min;
if(divide_by_value!=0)
{
scaled_value=(value-min)/divide_by_value;
mat[r][c]=scaled_value;
}
}
}
}
