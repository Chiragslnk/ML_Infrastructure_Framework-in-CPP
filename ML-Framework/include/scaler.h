#pragma once
#include<matrix.h>
#include<vector>
class scaler
{
private:
scaler();
public:
static matrix min_max_scaler(matrix &mat); 
static matrix min_max_scaler(matrix &mat,vector<uint32_t> columns_to_scale); 
static void min_max_scaler(matrix &mat,matrix &min_max); 
static void min_max_scaler(matrix &mat,matrix &min_max,vector<uint32_t> columns_to_scale);
static matrix standard_scaler(matrix &mat); 
static matrix standard_scaler(matrix &mat,vector<uint32_t> columns_to_scale); 
static void standard_scaler(matrix &mat,matrix &mean_std_dv); 
static void standard_scaler(matrix &mat,matrix &mean_std_dv,vector<uint32_t> columns_to_scale);
// do it yourself for robust scaling
};
