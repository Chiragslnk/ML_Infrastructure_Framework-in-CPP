#include<ml.h>
using namespace std;
int test_add_matrix_with_matrix()
{
	matrix b(5,5),c(5,5);
	matrix a;
	int rr,cc;
	double value1=1.0;
	double value2=10.0;
	for(rr=0;rr<5;++rr)
	{
		for(cc=0;cc<5;++cc)
		{
			b.set(rr,cc,value1++);
			c.set(rr,cc,value2++);
		}
	}
	a=b+c;
	for(rr=0;rr<5;++rr)
	{
		for(cc=0;cc<5;++cc)
		{
			cout<<a.get(rr,cc)<<" ";
		}
		cout<<endl;
	}
return 0;
}
int test_subtract_matrix_from_matrix()
{
	matrix b(5,5),c(5,5);
	matrix a;
	int rr,cc;
	double value1=1.0;
	double value2=10.0;
	for(rr=0;rr<5;++rr)
	{
		for(cc=0;cc<5;++cc)
		{
			b.set(rr,cc,value1++);
			c.set(rr,cc,value2++);
		}
	}
	a=b-c;
	for(rr=0;rr<5;++rr)
	{
		for(cc=0;cc<5;++cc)
		{
			cout<<a.get(rr,cc)<<" ";
		}
		cout<<endl;
	}
return 0;
}
int test_multiply_matrix_from_matrix()
{
	matrix b(4,5),c(5,8);
	matrix a;
	int rr,cc;
	double value=1.0;
	for(rr=0;rr<4;++rr)	
	{
		for(cc=0;cc<5;++cc)
		{
			b.set(rr,cc,value++);
		}
	}
	value=1.0;
	for(rr=0;rr<5;++rr)
	{
		for(cc=0;cc<8;++cc)
		{
			c.set(rr,cc,value++);
		}
	}
	a=b*c;
	for(rr=0;rr<4;++rr)
	{
		for(cc=0;cc<8;++cc)
		{
			cout<<a.get(rr,cc)<<" ";
		}
		cout<<endl;
	}
return 0;
}
int main()
{
	test_add_matrix_with_matrix();
	test_subtract_matrix_from_matrix();
	test_multiply_matrix_from_matrix();
return 0;
}
