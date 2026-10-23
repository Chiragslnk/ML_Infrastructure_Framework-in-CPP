#pragma once
#include<iostream>
#include<vector>
using namespace std;
class encoder
{
private:
encoder();
public: static void encode_one_hot(char *source,char *target,vector<uint32_t> columns_to_encode,bool has_header); 
static void encode_one_hot(char *source,char *target,vector<string> columns_to_encode);
};
