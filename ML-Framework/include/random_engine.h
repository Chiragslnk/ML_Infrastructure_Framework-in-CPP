#pragma once
#include<iostream>
#include<matrix.h>
using namespace std;
class random_engine
	{
		private:
			random_engine(){}
		public: 
			static void fill(matrix &m,double lower_bound,double upper_bound);
	};
