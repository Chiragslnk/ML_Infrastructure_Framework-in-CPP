#include<ml_exception.h>
#include<matrix_math.h>
#include<string.h>
#include<vector>
#include<thread>
#include<omp.h> 

void matrix_math::add(double *target,const double *left,const double *right,const size_t sz)
{
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]+right[i];
}

void matrix_math::subtract(double *target,const double *left,const double *right,const size_t sz)
{
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]-right[i];
}

void matrix_math::multiply(double *target,const double *left,const uint64_t left_rows,const uint64_t left_columns,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
uint64_t target_collection_size=left_rows*right_columns;
// populate target with 0.0 
memset(target,0.0,target_collection_size*sizeof(double));
uint64_t r,c,c1;
#pragma omp parallel for private(c,c1)
for(r=0;r<left_rows;++r)
{
for(c=0;c<left_columns;++c)
{
for(c1=0;c1<right_columns;++c1)
{
//target[r][c1]+=(left[r][c]*right[c][c1]); 
target[r*right_columns+c1]+=(left[r*left_columns+c]*right[c*right_columns+c1]);
}
}
}
}

void matrix_math::add(double *target,const double *left,double right,const uint64_t left_rows,const
uint64_t left_columns)
{
auto sz=left_rows*left_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]+right;
}

void matrix_math::subtract(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns)
{
auto sz=left_rows*left_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]-right;
}

void matrix_math::multiply(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns)
{
auto sz=left_rows*left_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]*right;
}

void matrix_math::divide(double *target,const double *left,double right,const uint64_t left_rows,const uint64_t left_columns)
{
if(right==0.0)
{
throw ml_exception("Cannot divide by zero");
}
auto sz=left_rows*left_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left[i]/right;
}

void matrix_math::add(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
auto sz=right_rows*right_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left+right[i];
}

void matrix_math::subtract(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
auto sz=right_rows*right_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left-right[i];
}

void matrix_math::multiply(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
auto sz=right_rows*right_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left*right[i];
}

void matrix_math::divide(double *target,double left,const double *right,const uint64_t right_rows,const uint64_t right_columns)
{
auto sz=right_rows*right_columns;
int64_t i;
#pragma omp parallel for
for(i=0;i<sz;++i) target[i]=left/right[i];
}
