#pragma once
#include<iostream>
#include<cstdint>
#include<string>
#include<matrix.h>
using namespace std;
class csv
{
private:
csv(){}
public:
static void load_x_y(string filename,matrix &x,matrix &y,uint32_t skip_lines);
};
