#pragma once
#include<iostream>
#include<string>
using namespace std;
class ml_exception:public runtime_error
{
	public:
 	ml_exception(string msg);
};
