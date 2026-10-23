#include<dataset_utils.h>
#include<ml_exception.h>
#include<matrix.h>
#include<iostream>
#include<cstdint>
#include<stdlib.h>
#include<algorithm>
#include<random>
#include<fcntl.h>
#include<unistd.h>
#include<vector>
#include<sys/stat.h>
using namespace std;
#define BUFFER_SIZE 4096 
void dataset_utils::split(string dataset_name,uint8_t matrix_1_percentage,uint32_t skip_lines,matrix &matrix_1,matrix &matrix_2)
{
if(matrix_1_percentage==0 || matrix_1_percentage>=100) throw ml_exception(string("Invalid matrix 1 percentage"));
struct stat s;
if(stat(dataset_name.c_str(),&s)<0)
{
throw ml_exception(string("Unable to open ")+dataset_name);
}
if(s.st_size==0)
{
throw ml_exception(string("Invalid dataset : ")+dataset_name);
}
int file_descriptor; 
file_descriptor=open(dataset_name.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0)
{
throw ml_exception(string("Unable to open ")+dataset_name);
}

unsigned char buffer[BUFFER_SIZE];
uint64_t rows,columns,line_count;
uint64_t comma_count;
uint64_t index;
uint32_t bytes_fetched;
line_count=0;
comma_count=0;
unsigned char last_char=(char)0;
int pos=0; 
int pos_of_first_byte_of_data_row=0; // =0 essential if skip_lines is zero


while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
last_char=buffer[bytes_fetched-1];
for(index=0;index<bytes_fetched;++index)
{
++pos;
if(buffer[index]=='\n')
{
++line_count; 
if(line_count==skip_lines) pos_of_first_byte_of_data_row=pos;
}
else
{
if(line_count==skip_lines)
{
if(buffer[index]==',') ++comma_count;
}
}
}
}


if(last_char!='\n') ++line_count;
if(line_count<=skip_lines)
{
close(file_descriptor); 
throw ml_exception(string("Invalid dataset : ")+dataset_name);
}


rows=line_count-skip_lines;
columns=comma_count+1;
uint64_t matrix_1_rows;
uint64_t matrix_2_rows;
matrix_1_rows=(rows*matrix_1_percentage)/100;
matrix_2_rows=rows-matrix_1_rows;
matrix_1.collection.resize(matrix_1_rows*columns);
matrix_1._rows=matrix_1_rows;
matrix_1._columns=columns;
matrix_1.flags=0;
matrix_2.collection.resize(matrix_2_rows*columns);
matrix_2._rows=matrix_2_rows;
matrix_2._columns=columns;
matrix_2.flags=0;
vector<uint64_t> v(rows);


for(index=0;index<rows;++index) v[index]=index;
random_device r_device;
mt19937 engine(r_device());
shuffle(v.begin(),v.end(),engine); 
lseek(file_descriptor,pos_of_first_byte_of_data_row,SEEK_SET);
char str[101];
uint8_t str_i=0;
double value;
unsigned char cc;
uint64_t line_number=0;
uint64_t place_at_index=v[0];
uint32_t column_index=0;
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
//write(1,buffer,bytes_fetched); // for testing only
for(index=0;index<bytes_fetched;++index)
{
cc=buffer[index];
if(cc==',')
{
str[str_i]='\0';
value=atof(str);
str_i=0; 	
if(place_at_index<matrix_1_rows) // send to matrix_1
{
matrix_1.collection[place_at_index*columns+column_index]=value;
}
else	// send to matrix_2
{
matrix_2.collection[(place_at_index-matrix_1_rows)*columns+column_index]=value;
}
++column_index;
}else if(cc=='\n')
{
str[str_i]='\0';
value=atof(str);
str_i=0; 	
if(place_at_index<matrix_1_rows) // send to matrix_1
{
matrix_1.collection[place_at_index*columns+column_index]=value;
}
else	// send to matrix_2
{
matrix_2.collection[(place_at_index-matrix_1_rows)*columns+column_index]=value;
}
column_index=0;
++line_number; 
if(line_number<v.size()) place_at_index=v[line_number];
}else 
{
str[str_i++]=cc;
}
} // inner for loop ends
}// while(1) ends
 


if(last_char!='\n')
{
str[str_i]='\0';
value=atof(str);
if(place_at_index<matrix_1_rows) // send to matrix_1
{
matrix_1.collection[place_at_index*columns+column_index]=value;
}
else	// send to matrix_2
{
matrix_2.collection[(place_at_index-matrix_1_rows)*columns+column_index]=value;
}
}
close(file_descriptor);
}


void dataset_utils::split(string dataset_name,uint8_t file_1_percentage,uint32_t number_of_lines_in_header,const string &file_1,const string &file_2)
{
if(file_1_percentage==0 || file_1_percentage>=100) throw ml_exception(string("Invalid percentage specified"));
struct stat s;
if(stat(dataset_name.c_str(),&s)<0)
{
throw ml_exception(string("Unable to open ")+dataset_name);
}
if(s.st_size==0)
{
throw ml_exception(string("Invalid dataset : ")+dataset_name);
}
int file_descriptor; 
file_descriptor=open(dataset_name.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0)
{
throw ml_exception(string("Unable to open ")+dataset_name);
}
int file_1_descriptor;
int file_2_descriptor; 
file_1_descriptor=open(file_1.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,S_IREAD | S_IWRITE);
if(file_1_descriptor<0)
{
close(file_descriptor); 
throw ml_exception(string("Unable to create ")+file_1);
}
file_2_descriptor=open(file_2.c_str(),O_WRONLY | O_CREAT | O_TRUNC | O_BINARY,S_IREAD | S_IWRITE);
if(file_2_descriptor<0)
{
close(file_descriptor);
close(file_1_descriptor); 
throw ml_exception(string("Unable to create ")+file_2);
}
unsigned char buffer[BUFFER_SIZE];
uint64_t rows,line_count;
uint64_t index;
uint32_t bytes_fetched;
line_count=0;
unsigned char last_char=(char)0;
int fattest_line;
vector<int> length_of_each_line;
fattest_line=0;
int len=0;
while(1)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
if(bytes_fetched==0) break;
last_char=buffer[bytes_fetched-1];
for(index=0;index<bytes_fetched;++index)
{
len++;
if(buffer[index]=='\n')
{
++line_count;
length_of_each_line.push_back(len);
if(len>fattest_line) fattest_line=len;
len=0;
}
}
}
if(last_char!='\n')
{
length_of_each_line.push_back(len);
++line_count;
if(len>fattest_line) fattest_line=len;
}
if(line_count<=number_of_lines_in_header)
{
close(file_descriptor);
close(file_1_descriptor);
close(file_2_descriptor); 
throw ml_exception(string("Invalid dataset : ")+dataset_name);
}
rows=line_count-number_of_lines_in_header;
uint64_t file_1_rows;
uint64_t file_2_rows;
file_1_rows=(rows*file_1_percentage)/100;
file_2_rows=rows-file_1_rows;
vector<uint64_t> v(rows);
for(index=0;index<rows;++index) v[index]=index;
random_device r_device;
mt19937 engine(r_device());
shuffle(v.begin(),v.end(),engine);
lseek(file_descriptor,0,SEEK_SET);
unsigned char *line_buffer;
line_buffer=(unsigned char *)malloc(sizeof(unsigned char)*fattest_line);
uint64_t place_at_index=v[0];
int row_index=0;
index=0;
for(auto length:length_of_each_line)
{
bytes_fetched=read(file_descriptor,line_buffer,length); // reading 1 line at a time
if(index<number_of_lines_in_header)
{
// header
write(file_1_descriptor,line_buffer,length);
write(file_2_descriptor,line_buffer,length);
}
else if(place_at_index<file_1_rows)
{
write(file_1_descriptor,line_buffer,length);
row_index++;
if(row_index<v.size()) place_at_index=v[row_index];
}
else
{
write(file_2_descriptor,line_buffer,length);
row_index++;
if(row_index<v.size()) place_at_index=v[row_index];
}
index++;
} // loop ends


//if(last_char!='\n')
//{
//int length=length_of_each_line[length_of_each_line.size()-1];
//if(place_at_index<file_1_rows)
//{
//write(file_1_descriptor,line_buffer,length);
//}
//else
//{
//write(file_2_descriptor,line_buffer,length);
//}
//}
free(line_buffer);
close(file_descriptor);
close(file_1_descriptor);
close(file_2_descriptor);
}
