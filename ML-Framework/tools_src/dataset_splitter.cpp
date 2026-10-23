#include<dataset_utils.h>
#include<stdlib.h>
#include<matrix.h>
#include<iostream>
using namespace std;
int main(int argc,char *argv[])
{
// required
// filename
// percentage
// number of lines in header
// first split filename
// second split filename
if(argc!=6)
{
cout<<"Invalid number of arguments"<<endl; 
cout<<"This data splitter required 6 argument"<<endl; 
cout<<"It will create two files, first file will contain number of rows as per specified percentage"<<endl; 
cout<<"First argument : Name of file that contains data to split"<<endl; 
cout<<"Second argument : What percent data to put in first file"<<endl; 
cout<<"Third argument : Number of lines which are part of header, if none pass 0"<<endl; 
cout<<"Fourth argument : Name of the first file that is to be created"<<endl; 
cout<<"Fifth argument : Name of the second file that is to be created"<<endl;
return EXIT_FAILURE;
}
matrix matrix_1;
matrix matrix_2;
try
{
string file_1(argv[4]);
string file_2(argv[5]);
string dataset_file=string(argv[1]); 
dataset_utils::split(dataset_file,(uint8_t)atoi(argv[2]),(uint32_t)atoi(argv[3]),file_1,file_2);
}catch(ml_exception &ex)
{
cout<<ex.what()<<endl;
return EXIT_FAILURE;
}
return EXIT_SUCCESS;
}
