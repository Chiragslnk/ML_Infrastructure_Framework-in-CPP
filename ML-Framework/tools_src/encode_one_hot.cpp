#include<cstdint>
#include<string.h>
#include<encoder.h>
#include<iostream>
#include<set>
#include<vector>
#include<cstdint>
using namespace std;
int main(int cc,char *vv[])
{
if(cc<=6) // program_name source target yes|no column-numbers|column-names . . . . . .
{
cout<<"usage [encode_one_hot source_file target_file has_header column-numbers 1 2 3 4 ...]"<<endl;
cout<<"or"<<endl; 
cout<<"usage [encode_one_hot source_file target_file has_header column-names one two three ...]"<<endl;
return 1;
}
if(strcmp(vv[4],"column-numbers")!=0 && strcmp(vv[4],"column-names")!=0)
{
cout<<"usage [encode_one_hot source_file target_file has_header column-numbers 1 2 3 4 ...]"<<endl;
cout<<"or"<<endl; 
cout<<"usage [encode_one_hot source_file target_file has_header column-names one two three ...]"<<endl;
return 1;
}
bool has_header=(strcmp(vv[3],"yes")==0);
if(strcmp(vv[4],"column-numbers")==0)
{
vector<uint32_t> columns_to_encode;
set<int> columns_set; 
for(int i=5;i<cc;++i) columns_set.insert(atoi(vv[i])-1); 
// -1 because 0 indexed internally for(auto x:columns_set) columns_to_encode.push_back(x);
try
{
encoder::encode_one_hot(vv[1],vv[2],columns_to_encode,has_header); 
cout<<"Encoded dataset "<<vv[2]<<" created"<<endl;
}catch(invalid_argument &ia)
{
cout<<ia.what()<<endl;
return 1;
}
}
return 0;
}
